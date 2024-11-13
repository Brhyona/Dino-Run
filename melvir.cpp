#include "src/setup/imageLoader.h"
#include "src/setup/Global.h"
#include "src/setup/melvir.h"
#include "src/setup/fonts.h"
#include <GL/glx.h>
#include <iostream>
#include <cmath>

constexpr float MOVE_SPEED = 4.5f;
constexpr float GRAVITY = 20.0f;
constexpr float PI = 3.14159265358979323846f;
bool facingLeft = false;

const int framesPerAction[NUM_STATES] = {
    3, // AVOID
    3, // BITE
    5, // DASH
    4, // DEAD
    3, // HURT
    3, // IDLE
    4, // JUMP
    3, // KICK
    6, // MOVE
    6  // SCAN
};


Health::Health(int max_health) : max_health(max_health), current_health(max_health) {}

void Health::TakeDamage(int damage) {
    current_health -= damage;
    if (current_health < 0) {
        current_health = 0;
    }
}

void Health::Heal(int amount) {
    current_health += amount;
    if (current_health > max_health) {
        current_health = max_health;
    }
}

bool Health::IsDead() const {
    return current_health <= 0;
}

int Health::GetCurrentHealth() const {
    return current_health;
}

void Health::SetMaxHealth(int new_max) {
    max_health = new_max;
    if (current_health > max_health) {
        current_health = max_health;
    }
}

int Health::GetMaxHealth() const {
    return max_health;
}


Player::Player(Image* img[NUM_STATES], float x, float y, int w, int h, int max_health) 
    : x(x), y(y), width(w), height(h), health(max_health){
}

Player::~Player() {
    //glDeleteTextures(NUM_STATES, textures);
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

void Player::handleInput(int key) {
    if (key == XK_Left || key == XK_Right) {
        g.moving = true;
    } else {
        g.moving = false;
    }

    if(key == XK_Up){
        g.jumping = true;
    } else {
        g.jumping = false;
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

/*
void Player::updateHitbox() {
    hitbox.x = x - (width / 2);
    hitbox.y = y - (height / 2);
    hitbox.width = width;
    hitbox.height = height;
}
*/

void Player::drawHearts(float x, float y, float size, int filledSegments) {
    // Draw full or half heart
    if (filledSegments >= 1) 
    {
        glColor3f(1.0f, 0.0f, 0.0f); // Red for filled segments
        glBegin(GL_POLYGON);

        for (int i = 0; i < 100; i++) 
        {
            float angle = 2.0 * PI * float(i) / float(100);
            float dx = size * 16 * pow(sin(angle), 3);
            float dy = size * (13 * cos(angle) - 5 * cos(2 * angle) - 2 * cos(3 * angle) - cos(4 * angle)); // Math to draw Heart
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
    std::string healthText = "Health: " + std::to_string(health.GetCurrentHealth()) + "/" + std::to_string(health.GetMaxHealth());
    Rect r;
    r.bot = g.yres - 20; // Adjusted for top left alignment
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, healthText.c_str());

    int maxHearts = health.GetMaxHealth() / 10; // Each heart represents 10 health
    float heartSize = 0.7f; // Size of each heart
    float gap = 22.0f; // Gap between hearts

    float startX = 20; // Start position from the left edge
    float startY = g.yres - 30; // Start position from the top edge

    int fullHearts = health.GetCurrentHealth() / 10;
    float remainingHealth = health.GetCurrentHealth() % 10;

    for (int i = 0; i < maxHearts; i++) 
    {
        float x = startX + i * (heartSize + gap);
        float y = startY;

        if (i < fullHearts) 
        {
            drawHearts(x, y, heartSize, 2); // Full heart
        } else if (i == fullHearts) 
        {
            int filledSegments = (remainingHealth + 2.5) / 5; // Number of filled segments (0 to 2)
            drawHearts(x, y, heartSize, filledSegments); // Partially filled heart
        } else {
            drawHearts(x, y, heartSize, 0); // Empty heart
        }
    }
}

void Player::takeDamage(int amount) {
    health.TakeDamage(amount);
    if (health.IsDead()) {
        updateState(DEAD); // Change state to DEAD if health reaches zero
    }
}

void Player::heal(int amount) {
    health.Heal(amount);
}

bool Player::isDead() const {
    return health.IsDead();
}

void Player::render() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    updateAnimationTimers();
    //updateHitbox();
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

    //glColor3f(1.0f, 0.0f, 0.0f);  // Red color for hitbox
/*
    glPushMatrix();    
    glTranslatef(hitbox.x + (hitbox.width / 2), hitbox.y + (hitbox.height / 2), 0);  // Center hitbox
    
    glBegin(GL_LINE_LOOP);
        glVertex2f(-hitbox.width / 2, -hitbox.height / 2);
        glVertex2f(-hitbox.width / 2, hitbox.height / 2);
        glVertex2f(hitbox.width / 2, hitbox.height / 2);
        glVertex2f(hitbox.width / 2, -hitbox.height / 2);
    glEnd();
    glPopMatrix();
*/
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

}
