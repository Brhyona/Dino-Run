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
    PlayerState currentState;
    float x, y, width, height, frameDuration;
    int frameIndex = 0;

public:
    Player(Image* img[NUM_STATES], float x, float y, int w, int h);
    void render();
    void updateState(PlayerState newState);
    void handleInput(int key);
    GLuint getTexture();
    ~Player();
};

#endif // MELVIR_H