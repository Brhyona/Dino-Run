/*
 * AUTHOR: SKYE BEEBE
 *
 * DATE: October 2024
*/
#include "src/setup/sbeebe.h"
#include "src/setup/imageLoader.h"
#include "src/setup/melvir.h"
#include "src/setup/Global.h"
#include <chrono>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include <cstdio>
#include <GL/glx.h>

using namespace std;

Timers Enemytimers;

const int frameperact[NUM_EVIL_STATES] = {
  8, //run
  8  //whip
};

Evil::Evil(Image* img[NUM_EVIL_STATES], float sx, float sy, float sw, float sh)
    : sx(sx), sy(sy), sw(sw), sh(sh) {

        archHitbox = {sx - sw / 2, sy - sh / 2, sw, sh};
        /*position[0] = sx;
        position[1] = sy;

        velocity[0] = 0.0f;
        velocity[1] = 0.0f;
*/
 
}

Evil::~Evil() {
   //glDeleteTextures(1, texture);
}

GLuint Evil::getTexture() {
    return texture[currentState];
}

unsigned char* makeWhitePixelsTransparent(unsigned char* data, int width, int height) {
    int totalPixels = width * height;
    unsigned char* newData = (unsigned char*)malloc(totalPixels * 4); // Allocate space for RGBA data

    for (int i = 0; i < totalPixels; i++) {
        int index = i * 4;
        unsigned char r = data[i * 3 + 0]; // Red
        unsigned char g = data[i * 3 + 1]; // Green
        unsigned char b = data[i * 3 + 2]; // Blue

        newData[index + 0] = r; // Red
        newData[index + 1] = g; // Green
        newData[index + 2] = b; // Blue

        // Set alpha to 0 if the pixel is pure white, else 255
        newData[index + 3] = (r == 255 && g == 255 && b == 255) ? 0 : 255;
    }

    return newData;
}

void Evil::loadTextures(Image* img[NUM_EVIL_STATES]) {
    
    for (int i = 0; i < NUM_EVIL_STATES; i++) {
        if (!img[i]->data) {
            cerr << "Failed to load image data for state " << i << endl;
            texture[i] = 0;
            continue;
        }

        glGenTextures(1, &texture[i]);
        glBindTexture(GL_TEXTURE_2D, texture[i]);

        //float width = sw;
        //float height = sh;

/*        unsigned char* rgbaData = img[i]->buildAlphaData(img[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img[i]->width, img[i]->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData);
        free(rgbaData);
*/

        unsigned char* rgbaData = makeWhitePixelsTransparent(img[i]->data, img[i]->width, img[i]->height);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img[i]->width, img[i]->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData);

        free(rgbaData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
    }
}

void Evil::updateAnimation(EnemyAnimation newState) {
     if (currentState != newState) {
        currentState = newState;
    }
}

void Evil::updateAnimationTimers() {
    int frameCount = frameperact[currentState];
    struct timespec* currentTimer = nullptr;
    
    switch (currentState) {
        case E_RUN: currentTimer = &g.EnemyRunTime; 
            break;
        case E_WHIP: currentTimer = &g.EnemyWhipTime; 
            break;
        default: 
            break;
    }

if (currentTimer) {
        timers.recordTime(&timers.timeCurrent);
	    double timeSpan = timers.timeDiff(currentTimer, &timers.timeCurrent);
    
        if (timeSpan > g.delay) {
            g.enemyframe = (g.enemyframe + 1) % frameCount;
            timers.recordTime(currentTimer);
    	}
    } else {
        g.enemyframe = 0;
    }
   
}

/*void Evil::updateEnemy(float deltaTime) {
    
    this->position[0] += this->velocity[0] * deltaTime;
    this->position[1] += this->velocity[1] * deltaTime;
}*/

void Evil::follow(int x, int y, float deltaTime, Player &player) {
    float playerX = player.getX();
    float playerY = player.getY();
    const float followBehind = 100.0f;
    const float followSpeed = 80.0f;
    const Hitbox& playerHitbox = player.getPlayerHitbox();

    float targetX = playerX - (playerX > sx ? followBehind : -followBehind);
    float targetY = playerY;

    float dx = targetX - sx;
    float dy = targetY - sy;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance > 0.0f) {
        dx /= distance;
        dy /= distance;
    }
        sx += dx * followSpeed * deltaTime;
        sy += dy * followSpeed * deltaTime;
    //}

    if (archHitbox.x < playerHitbox.x + playerHitbox.width &&
        archHitbox.x + archHitbox.width > playerHitbox.x &&
        archHitbox.y < playerHitbox.y + playerHitbox.height &&
        archHitbox.y + archHitbox.height > playerHitbox.y) {
        // Collision detected, switch to whip animation and deal damage
        updateAnimation(E_WHIP); // Switch to whipping animation
        player.takeDamage(10);   // Deal 10 damage to the player
    }
}

void Evil::render() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.01f);
    glEnable(GL_BLEND);

    updateAnimationTimers();
    int frameCount = frameperact[currentState];
    float frameWidth = 1.0f / frameCount;

    float tx1 = g.enemyframe * frameWidth;
    float tx2 = tx1 + frameWidth;

    glBindTexture(GL_TEXTURE_2D, getTexture());
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Render the current frame
    glPushMatrix();
    glTranslatef(sx, sy, 0);
    

    glBegin(GL_QUADS);
        glTexCoord2f(tx1, 1.0f); 
        glVertex2f(-sw / 2, -sh / 2); // Bottom-left
        glTexCoord2f(tx2, 1.0f);
        glVertex2f(sw / 2, -sh / 2);  // Bottom-right
        glTexCoord2f(tx2, 0.0f);
        glVertex2f(sw / 2, sh / 2);   // Top-right
        glTexCoord2f(tx1, 0.0f); 
        glVertex2f(-sw / 2, sh / 2);  // Top-left
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

/*float Evil::getX() const {
    return x;
}

float Evil::getY() const {
    return y;
}*/
