#ifndef SKYE_ROSE
#define SKYE_ROSE

#include "imageLoader.h"
#include <GL/glx.h>
#include "Global.h"
#include "melvir.h"
#include "jpompa.h"
#include <iostream>
#include <cmath>

enum EnemyAnimation {
    E_RUN,
    E_WHIP,
    NUM_EVIL_STATES
};

class Player;
class Platform;

class Evil {
    private:
    GLuint texture[NUM_EVIL_STATES];
    EnemyAnimation currentState;
    Hitbox archHitbox;
    //Vec position;
    //Vec velocity;
    float sx, sy;   // Archeologist's x and y position
    float sw, sh, frameduration; // archeologist's width and height
    int evilcurrentFrame; // Current Frame
    int framedex = 0;

    public:
    Evil(Image* img[NUM_EVIL_STATES], float sx, float sy, float sw, float sh);

    void updateAnimation(EnemyAnimation newstate);
    void updateAnimationTimers();
    void updateEnemy(float deltaTime);
    void movement();
    void render();   // Render the enemy sprite
    GLuint getTexture();
    void setPosition(float newX, float newY);
    //void setMoveSpeed(float speed);
    void loadTextures(Image* img[NUM_EVIL_STATES]);
    void follow(int x, int y, float deltatime, Player &player); //Position in relation to the player
    ~Evil();
};


#endif





