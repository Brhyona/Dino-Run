#include "src/setup/imageLoader.h"
#include "src/setup/Global.h"
#include "src/setup/melvir.h"
#include "src/setup/health.h"
#include "src/setup/fonts.h"
#include "src/setup/jpompa.h"
#include <GL/glx.h>
#include <iostream>
#include <cmath>

constexpr float MOVE_SPEED = 5.0f;
constexpr float DASH_SPEED = 12.0f;
constexpr float GRAVITY = 200.0f;
constexpr float DASH_THRESHOLD = 0.30f;
constexpr float PI = 3.14159265358979323846f;
bool facingLeft = false;

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
    6  // SCAN
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
        x += DASH_SPEED; 
        facingLeft = true;
    } else if (key == XK_Right) {
        updateState(DASH);
        x += DASH_SPEED;
        facingLeft = false;
    }
}

void Player::handleInput(int key) {
    static float keyHoldTime = 0.0f;
    static int lastKey = 0; 
    
    velocity[0] = 0.0f;
    velocity[1] = 0.0f;

    if (key == XK_Left || key == XK_Right) {
        g.moving = true;
        if (lastKey == key) {
            keyHoldTime += 0.1f; // Increment hold time
        } else {
            keyHoldTime = 0.0f;
        } 
    } else {
        g.moving = false;
        keyHoldTime = 0.0f;
    }

    if(key == XK_Up){
        g.jumping = true;
        if (velocity[1] == 0.0f) { // Only allow jumping if not already jumping/falling
            g.jumping = true;
            updateState(JUMP);
            //velocity[1] = 20.0f - GRAVITY; // Initial upward velocity
        }
    } 

    if (keyHoldTime > DASH_THRESHOLD) {
        triggerDash(key);
    }

    switch (key) {
        case XK_Left: 
            updateState(MOVE);
            x -= MOVE_SPEED;
            facingLeft = true;
            break;
        case XK_Right: 
            updateState(MOVE);
            x += MOVE_SPEED;
            facingLeft = false;
            break;
        case XK_Up:
            updateState(JUMP);
            y += MOVE_SPEED;
            break;
        case XK_Down: 
            break;
    }
}

void Player::updatePlayer(float deltaTime) {
    position[0] += velocity[0] * deltaTime;
    position[1] += velocity[1] * deltaTime;
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
    glTranslatef(x, y, 0); 
        
    glBegin(GL_QUADS);
        glTexCoord2f(tx1, 1.0f); glVertex2f(-width / 2, -height / 2);
        glTexCoord2f(tx2, 1.0f); glVertex2f(width / 2, -height / 2);
        glTexCoord2f(tx2, 0.0f); glVertex2f(width / 2, height / 2);
        glTexCoord2f(tx1, 0.0f); glVertex2f(-width / 2, height / 2);
    glEnd();

    glPopMatrix();
    glDisable(GL_BLEND);
    healthMeter();
}

void drawHeart(float x, float y, float size, int filledSegments) 
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

void Player::healthMeter() {
    int currentHealth = playerHealth.GetCurrentHealth();
    int fullHearts = playerHealth.GetCurrentHealth() / 10;
    int remainingHealth = currentHealth % 10;

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

    //int fullHearts = playerHealth.GetCurrentHealth() / 10;
    //float remainingHealth = playerHealth.GetCurrentHealth() % 10;

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
}

void Player::handleFalling(const Platform& platforms) {
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
            this->position[1] = platformHitbox.y + platformHitbox.height;
            this->velocity[1] = 0.0f; // Stop falling
            break;
        }
        temp = temp->next;
    }

    if (!onPlatform) {
        this->velocity[1] -= GRAVITY * 0.016f; // Simulate gravity with deltaTime
        this->position[1] += this->velocity[1]; // Update player position

        if (this->position[1] < 0) {
            //playerHealth.isDead = true; // Mark the player as dead
            std::cout << "Player has fallen off the screen and died." << std::endl;
        }
    }

    updateHitbox(); // Ensure the hitbox reflects the updated position
}


float Player::getX() const { return position[0]; }
float Player::getY() const { return position[1]; }

