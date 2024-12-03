#include "src/setup/imageLoader.h"
#include "src/setup/Global.h"
#include "src/setup/melvir.h"
#include "src/setup/health.h"
#include "src/setup/fonts.h"
#include "src/setup/jpompa.h"
#include <GL/glx.h>
#include <iostream>
#include <cmath>

constexpr float MOVE_SPEED = 9.0f;
constexpr float JUMP_SPEED = 8.0f;
constexpr float DASH_SPEED = 12.0f;
constexpr float GRAVITY = 7.5f;
constexpr float DASH_THRESHOLD = 0.30f;
constexpr float PI = 3.14159265358979323846f;
bool facingLeft = false;
bool fallingSoundPlayed = false;
bool bonesSoundPlayed = false;

const int framesPerAction[NUM_STATES] = {
    3, // AVOID
    3, // BITE
    6, // DASH
    4, // DEAD
    3, // HURT
    3, // IDLE
    4, // JUMP
    3, // KICK
    6, // MOVE
    6,  // SCAN
    4  // HATCHING 
};

Player::Player(Image* img[NUM_STATES], float x, float y, int w, int h) 
    : x(x), y(y), width(w), height(h), playerHealth(100) {
    updateHitbox();
    position[0] = x;
    position[1] = y;
    
    velocity[0] = 0.0f;
    velocity[1] = 0.0f;
    //playerHealth.TakeDamage(50);
    std::cout << "Player health initialized to: " << playerHealth.GetCurrentHealth() << std::endl;
    std::cout << "Current Health: " << playerHealth.GetCurrentHealth() << ", Max Health: " << playerHealth.GetMaxHealth() << std::endl;

}

Player::~Player() {
    glDeleteTextures(NUM_STATES, textures);
}

GLuint Player::getTexture() {
    return textures[currentState];
}  

void Player::loadTextures(Image* img[NUM_STATES]) {
    for (int i = 0; i < NUM_STATES; i++) {
        if (!img[i]->data) {
            std::cerr << "Failed to load image data for state " << i << std::endl;
            textures[i] = 0;
            continue;
        }

        glGenTextures(1, &textures[i]);
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        unsigned char* rgbaData = img[i]->buildAlphaData(img[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img[i]->width, img[i]->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData);
        free(rgbaData);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        #ifdef DEBUG
        std::cout << "Player textures[" << i << "] ID: " << textures[i] << " loaded for state " << i 
                  << " Width: " << img[i]->width << " Height: " << img[i]->height << std::endl;
        #endif
    }
}

void Player::updateState(PlayerState newState) {
     if (currentState != newState) {
        currentState = newState;
    }
    /*
    if (g.spawnAnimation && currentState != HATCH) {
        currentState = HATCH;
    } else if (!g.spawnAnimation && currentState == HATCH) {
        currentState = IDLE;
    } else if (currentState != newState) {
        currentState = newState;
    }
    */
}

void Player::updateAnimationTimers() {
    int frameCount = framesPerAction[currentState];
    struct timespec* currentTimer = nullptr;
    
    switch (currentState) {
        case AVOID: currentTimer = &g.AvoidTime; 
            break;
        case BITE: currentTimer = &g.BiteTime; 
            break;
        case DASH: currentTimer = &g.DashTime; 
            break;
        case DEAD: currentTimer = &g.DeadTime; 
            break;
        case HURT: currentTimer = &g.HurtTime; 
            break;
        case IDLE: currentTimer = &g.IdelTime; 
            break;
        case JUMP: currentTimer = &g.JumpTime; 
            break;
        case KICK: currentTimer = &g.KickTime; 
            break;
        case MOVE: currentTimer = &g.MoveTime; 
            break;
        case SCAN: currentTimer = &g.ScanTime; 
            break;
        default: 
            break;
    }

    if (g.moving || g.jumping) {
        timers.recordTime(&timers.timeCurrent);
	    double timeSpan = timers.timeDiff(currentTimer, &timers.timeCurrent);
    
        if (timeSpan > g.delay) {
            g.playerFrame = (g.playerFrame + 1) % frameCount;
            timers.recordTime(currentTimer);
    	}
        
    } else {
        g.playerFrame = 0;
    }

}

void Player::triggerDash(int key) {
    if (key == XK_Shift_L) {
        updateState(DASH);
        position[0] += DASH_SPEED; 
        facingLeft = true;
    } else if (key == XK_Right) {
        updateState(DASH);
        position[0] += DASH_SPEED;
        facingLeft = false;
    }
}

void Player::handleInput(int key, Sound &sound) {
    this->velocity[0] = 0.0f; // Reset horizontal velocity unless overridden below

    // Jumping logic
    if (key == XK_Up) {
        if (!g.jumping || this->velocity[1] < 0.0f) { // Allow jump if not jumping or while airborne
            // Ensure maximum of 2 jumps
            if (this->velocity[1] <= 0.0f) { // Prevent jumping if already double jumped
                g.jumping = true;
                updateState(JUMP);

                // Reset vertical velocity for jump or double jump
                this->velocity[1] = JUMP_SPEED;

                // Ensure horizontal momentum for arc-like movement
                if (g.moving) {
                    // Enhance horizontal velocity during movement
                    this->velocity[0] = (facingLeft ? -MOVE_SPEED : MOVE_SPEED) * 1.5f;
                } else {
                    // Default horizontal push when stationary
                    this->velocity[0] = MOVE_SPEED * 1.0f; // Stronger push to emphasize the arc
                    facingLeft = false; // Force facing right for default jumps
                }

                // Debugging output
                std::cout << "Player jumped! VelocityY: " << this->velocity[1]
                          << ", VelocityX: " << this->velocity[0] << std::endl;
            }
        }
    }

    // Movement logic
    else if (key == XK_Left) {
        g.moving = true;
        this->velocity[0] = -MOVE_SPEED; // Move left
        facingLeft = true;
    } else if (key == XK_Right) {
        g.moving = true;
        this->velocity[0] = MOVE_SPEED * 0.5f; // Move right
        facingLeft = false;
    } else {
        this->velocity[0] = 0.0f; // No movement input
    }

    // Additional switch-based handling (if needed for future features)
    switch (key) {
        case XK_Left:
            updateState(MOVE);
            this->position[0] -= MOVE_SPEED;
            facingLeft = true;
            break;
        case XK_Right:
            updateState(MOVE);
            this->position[0] += MOVE_SPEED;
            facingLeft = false;
            break;
        case XK_Up:
            // Already handled above; keep here for compatibility
            updateState(JUMP);
            break;
        case XK_Down:
            // Placeholder for potential crouch or drop logic
            break;
    }

    // Ensure momentum during jumps
    if (g.jumping) {
        if (facingLeft) {
            this->velocity[0] = -MOVE_SPEED * 1.5f; // Maintain and boost leftward momentum
        } else {
            this->velocity[0] = MOVE_SPEED * 1.5f; // Maintain and boost rightward momentum
        }
    }
}

void Player::updatePlayer(float deltaTime) {
    
    this->position[0] += this->velocity[0] * deltaTime;
    this->position[1] += this->velocity[1] * deltaTime;

    // Update hitbox position
    this->hitbox.x = this->position[0] - this->width / 2;
    this->hitbox.y = this->position[1] - this->height / 2;

    //std::cout << "Updated player: x=" << this->position[0]
     //         << ", y=" << this->position[1]
     //         << ", velocityX=" << this->velocity[0]
     //         << ", velocityY=" << this->velocity[1] << std::endl;
    
    updateHitbox();
}

void Player::updateHitbox() {
    hitbox.x = position[0];
    hitbox.y = position[1];
    hitbox.width = width;
    hitbox.height = height;
}

void Player::render() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    updateAnimationTimers();
    int frameCount = framesPerAction[currentState];
    float frameWidth = 1.0f / frameCount;
    
    float tx1 = g.playerFrame * frameWidth;
    float tx2 = tx1 + frameWidth;

    if (facingLeft) {
        std::swap(tx1, tx2);
    }

    #ifdef DEBUG
    
    std::cout << "Rendering State: " << currentState 
              << " Frame: " << g.playerFrame 
              << " Texture ID: " << textures[currentState]
              << " tx1: " << tx1 << " tx2: " << tx2 << std::endl;

    glColor3f(1.0f, 0.0f, 0.0f);  // Red color for hitbox

    glPushMatrix();    
    glTranslatef(hitbox.x + (hitbox.width / 2), hitbox.y + (hitbox.height / 2), 0);  // Center hitbox
    
    glBegin(GL_LINE_LOOP);
        glVertex2f(-hitbox.width / 2, -hitbox.height / 2);
        glVertex2f(-hitbox.width / 2, hitbox.height / 2);
        glVertex2f(hitbox.width / 2, hitbox.height / 2);
        glVertex2f(hitbox.width / 2, -hitbox.height / 2);
    glEnd();
    glPopMatrix();

    #endif

    glBindTexture(GL_TEXTURE_2D, getTexture());
    glPushMatrix();
    glTranslatef(this->position[0], this->position[1], 0); 
        
    glBegin(GL_QUADS);
        glTexCoord2f(tx1, 1.0f); glVertex2f(-width / 2, -height / 2);
        glTexCoord2f(tx2, 1.0f); glVertex2f(width / 2, -height / 2);
        glTexCoord2f(tx2, 0.0f); glVertex2f(width / 2, height / 2);
        glTexCoord2f(tx1, 0.0f); glVertex2f(-width / 2, height / 2);
    glEnd();

    glPopMatrix();
    healthMeter();
    glDisable(GL_BLEND);
}

void Player::drawHeart(float x, float y, float size, int filledSegments) 
{
    // Draw full or half heart
    if (filledSegments >= 1) 
    {
        glColor3f(1.0f, 0.0f, 0.0f); // Red for filled segments
        glBegin(GL_POLYGON);
        for (int i = 0; i < 100; i++) 
        {
            float angle = 2.0 * PI * float(i) / float(100);
            float dx = size * 16 * pow(sin(angle), 3);
            float dy = size * (13 * cos(angle) - 5 * cos(2 * angle) - 2 * cos(3 * angle) - cos(4 * angle));
            if ((filledSegments == 1 && dx <= 0) || filledSegments == 2) 
            {
                glVertex2f(x + dx, y + dy);
            }
        }
        glEnd();
    }

    // Draw empty segments
    if (filledSegments < 2) {
        glColor3f(0.5f, 0.5f, 0.5f); // Gray for empty segments
        glBegin(GL_POLYGON);
        for (int i = 0; i < 100; i++) {
            float angle = 2.0 * PI * float(i) / float(100);
            float dx = size * 16 * pow(sin(angle), 3);
            float dy = size * (13 * cos(angle) - 5 * cos(2 * angle) - 2 * cos(3 * angle) - cos(4 * angle));
            if ((filledSegments == 0) || (filledSegments == 1 && dx > 0)) {
                glVertex2f(x + dx, y + dy);
            }
        }
        glEnd();
    }
}

void Player::healthMeter() 
{   
    // Health GUI
    std::string healthText = "Health: " + std::to_string(playerHealth.GetCurrentHealth()) + "/" + std::to_string(playerHealth.GetMaxHealth());
    Rect r;
    r.bot = g.yres - 20; // Adjusted for top left alignment
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, healthText.c_str());

    int maxHearts = playerHealth.GetMaxHealth() / 10; // Each heart represents 10 health
    float heartSize = 0.7f; // Size of each heart
    float gap = 22.0f; // Gap between hearts

    float startX = 20; // Start position from the left edge
    float startY = g.yres - 30; // Start position from the top edge

    int fullHearts = playerHealth.GetCurrentHealth() / 10;
    float remainingHealth = playerHealth.GetCurrentHealth() % 10;

    //std::cout << "Rendering " << maxHearts << " hearts. Full: " << fullHearts 
    //          << ", Remaining Health: " << remainingHealth << std::endl;

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, g.xres, 0, g.yres, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
 
    for (int i = 0; i < maxHearts; i++) {
        float x = startX + i * (heartSize + gap);
        float y = startY;

        if (i < fullHearts) {
            drawHeart(x, y, heartSize, 2); // Full heart
        } else if (i == fullHearts) {
            int filledSegments = (remainingHealth + 2.5) / 5; // Number of filled segments (0 to 2)
            drawHeart(x, y, heartSize, filledSegments); // Partially filled heart
        } else {
            drawHeart(x, y, heartSize, 0); // Empty heart
        }
    }
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D); // Re-enable textures for the rest of the game
    glDisable(GL_BLEND); 
}

void Player::handleFalling(const Platform& platforms, Sound &sound) {
    bool onPlatform = false;

    PlatformNode* temp = platforms.getPlatformHead();
    while (temp) {
        const Hitbox& platformHitbox = temp->hitbox;
        const Hitbox& playerHitbox = this->hitbox;

        // Check if the player is on the platform
        if (playerHitbox.x < platformHitbox.x + platformHitbox.width &&
            playerHitbox.x + playerHitbox.width > platformHitbox.x &&
            playerHitbox.y >= platformHitbox.y &&
            playerHitbox.y - 5 <= platformHitbox.y + platformHitbox.height) {
            onPlatform = true;

            // Align the player with the platform
            if (!g.jumping && this->velocity[1] <= 0) {
                this->position[1] = platformHitbox.y + platformHitbox.height;
                this->velocity[1] = 0.0f; 
            }
            //std::cout << "Player landed on platform at y=" << this->position[1] << std::endl;
            break;
        }
        temp = temp->next;
    }

    if (!onPlatform) {
        this->velocity[1] -= GRAVITY * 0.016f;
    }
        
    this->position[1] += this->velocity[1]; 

    if (onPlatform) {
        g.jumping = false;
    }

    if (!g.jumping) {
        //std::cout << "Player is falling!" << std::endl;
    }

    if (this->position[1] < 0) {
        if (!fallingSoundPlayed) {
            #ifdef USE_OPENAL_SOUND
            sound.playSound(g.alSourceFalling);
            #endif
            fallingSoundPlayed = true; // Mark falling sound as played
        } else if (!bonesSoundPlayed) {
            #ifdef USE_OPENAL_SOUND
            sound.playSound(g.alSourceBones);
            #endif
            bonesSoundPlayed = true; // Mark bones sound as played
        }
        std::cout << "Player has fallen off the screen and died." << std::endl;
        g.isGameOver = true;
        playerHealth.TakeDamage(100);
        playerHealth.IsDead();
        
    }

    updateHitbox(); 
}

float Player::getX() const { return position[0]; }
float Player::getY() const { return position[1]; }
