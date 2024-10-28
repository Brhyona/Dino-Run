#include "src/setup/imageLoader.h"
#include "src/setup/Global.h"
#include "src/setup/melvir.h"
#include <GL/glx.h>
#include <iostream>

constexpr float MOVE_SPEED = 2.0f;

Player::Player(Image* img[NUM_STATES], float x, float y, int w, int h) 
    : x(x), y(y), width(w), height(h) {
    for (int i = 0; i < NUM_STATES; i++) {
        if (!img[i]->data) {
            printf("Failed to load image:");
            return;
        }
        
        printf("Player Image loaded: Width: %d, Height: %d\n", img[i]->width, img[i]->height);

        glGenTextures(1, &textures[i]);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img[i]->width, img[i]->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img[i]->data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

Player::~Player() {
    glDeleteTextures(NUM_STATES, textures);
}

GLuint Player::getTexture() {
    return textures[currentState];
}  

void Player::updateState(PlayerState newState) {
    currentState = newState;
}

void Player::handleInput(int key) {
    switch (key) {
        case XK_Left: // Left arrow key
            updateState(MOVE);
            x -= MOVE_SPEED;
            break;
        case XK_Right: // Right arrow key
            updateState(MOVE);
            x += MOVE_SPEED;
            break;
        case XK_Up: // Up arrow key
            updateState(JUMP);
            x -= MOVE_SPEED;
            break;
        case XK_Down: // Down arrow key
            break;
    }
}

void Player::render() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[currentState]);

    //std::cout << "Current State: " << currentState << " Texture ID: " << textures[currentState] << std::endl;

    timers.recordTime(&timers.timeCurrent);
double timeSpan = timers.timeDiff(&g.IdelTime, &timers.timeCurrent);

    if (timeSpan > g.delay) {
g.playerFrame++;
if (g.playerFrame >= 2) {
g.playerFrame = 0;
}
timers.recordTime(&g.IdelTime);
}

    //glBindTexture(GL_TEXTURE_2D, textures[currentState]);

    // Render the quad with the texture coordinates
    glBegin(GL_QUADS);
    glTexCoord2f(g.playerFrame * 0.5f, 0.0f);          glVertex2f(x, y);
    glTexCoord2f((g.playerFrame + 1) * 0.5f, 0.0f);    glVertex2f(x + width, y);
    glTexCoord2f((g.playerFrame + 1) * 0.5f, 1.0f);    glVertex2f(x + width, y + height);
    glTexCoord2f(g.playerFrame * 0.5f, 1.0f);          glVertex2f(x, y + height);
    glEnd();
}
