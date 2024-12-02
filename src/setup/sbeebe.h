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
    NUM_EVIL_STATES
};

class Player;
class Platform;

class Evil {
    private:
    Player& player;
    GLuint texture;
    EnemyAnimation currentState;
    float x, y;   // Archeologist's x and y position
    float width, height; // archeologist's width and height
    int evilcurrentFrame;    // Current Frame
    float animationTimer; // Timer for frame updates
    void follow(); //Position in relation to the player

    public:
    Evil(Image* img, float sx, float sy, float sw, float sh, Player& p);
    ~Evil();

    void updateAnimation();
    void movement();
    void render();   // Render the enemy sprite
    void getTexture() const;
    void setPosition(float newX, float newY);
    void setMoveSpeed(float speed);
    float getX() const;
    float getY() const;
};


#endif





