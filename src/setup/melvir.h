#pragma once

#ifndef MELVIR_H
#define MELVIR_H

#include "imageLoader.h"
#include "Global.h"
#include "health.h"
#include "jpompa.h" 
#include <GL/glx.h>

class Platform;


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
    HATCH,
    NUM_STATES
};

class Player {
private:
    GLuint textures[NUM_STATES];
    Vec position;
    Vec velocity;
    Hitbox hitbox;
    PlayerState currentState;
    float x, y, width, height, frameDuration;
    int frameIndex = 0;
    //Health playerHealth;
    bool dashing;
    bool dashCompleted;

public:
    Player(Image* img[NUM_STATES], float x, float y, int w, int h);
    Hitbox getPlayerHitbox() const { return this->hitbox; }
    //float getX() const{ return x;}
    //float getY() const {return y;}
    //Player(Image* img[NUM_STATES], float x, float y, int w, int h, int max_health);
    GLuint getTexture();
    float getX() const;
    float getY() const;
    void loadTextures(Image* img[NUM_STATES]);    
    void updateState(PlayerState newState);
    void updateHitbox();
    void updatePlayer(float deltaTime);
    void updateAnimationTimers();
    void triggerDash(int key);
    void handleInput(int key);
    void handleFalling(const Platform& platforms);
    void healthMeter();
    void drawHeart(float x, float y, float size, int filledSegments);
    void render();
     Health playerHealth;
     const Hitbox& getHitbox() const { return hitbox;}
    ~Player();
//
    float getWidth() const {
        return width;
    }

    float getHeight() const {
        return height;
    }
};

#endif // MELVIR_H