#ifndef MELVIR_H
#define MELVIR_H

#include "imageLoader.h"
#include "Global.h"
#include <GL/glx.h>

enum PlayerState {
    AVOID,
    BITE,
    DASH,
    DEAD,
    HURT,
    IDLE,
    JUMP,
    KICK,
    MOVE,
    SCAN,
    NUM_STATES
};

class Player {
private:
    GLuint textures[NUM_STATES];
    //Hitbox hitbox;
    PlayerState currentState;
    float x, y, width, height, frameDuration;
    int frameIndex = 0;

public:
    Player(Image* img[NUM_STATES], float x, float y, int w, int h);
    GLuint getTexture();
    void loadTextures(Image* img[NUM_STATES]);    
    void updateState(PlayerState newState);
    void updateHitbox();
    void updateAnimationTimers();
    void handleInput(int key);
    void handleFalling();
    void render();
    ~Player();
};

#endif // MELVIR_H