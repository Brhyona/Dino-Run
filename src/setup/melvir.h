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

class Health {
public:
    Health(int max_health);

    void TakeDamage(int damage);
    void Heal(int amount);

    bool IsDead() const;

    int GetCurrentHealth() const;
    void SetMaxHealth(int new_max);
    int GetMaxHealth() const;

private:
    int max_health;
    int current_health;

};

class Player {
private:
    GLuint textures[NUM_STATES];
    //Hitbox hitbox;
    PlayerState currentState;
    float x, y, width, height, frameDuration;
    int frameIndex = 0;
    Health health; 

public:
    Player(Image* img[NUM_STATES], float x, float y, int w, int h, int max_health);
    GLuint getTexture();
    void loadTextures(Image* img[NUM_STATES]);    
    void updateState(PlayerState newState);
    void updateHitbox();
    void updateAnimationTimers();
    void handleInput(int key);
    void handleFalling();
    void drawHearts(float x, float y, float size, int filledSegments);
    void healthMeter();
    void takeDamage(int amount);
    void heal(int amount);
    bool isDead() const;
    void render();
    ~Player();
};

#endif // MELVIR_H