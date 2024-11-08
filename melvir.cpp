#include "src/setup/imageLoader.h"
#include "src/setup/Global.h"
#include "src/setup/melvir.h"
#include <GL/glx.h>
#include <iostream>

constexpr float MOVE_SPEED = 4.5f;
constexpr float GRAVITY = 20.0f;
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

Player::Player(Image* img[NUM_STATES], float x, float y, int w, int h) 
    : x(x), y(y), width(w), height(h) {
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
