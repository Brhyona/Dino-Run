#ifndef BTHOMAS_H
#define BTHOMAS_H

#include "imageLoader.h"
#include "Global.h"
#include <GL/glx.h>
#include "melvir.h"

enum BatState {
 BAT_FLYING,
 NUM_BAT_STATES
};
class Player;
class Bat {
    private: 
    Player& player;
    GLuint texture;
    BatState currentState;
    float x,y;
    float width, height;
    float moveSpeed;
    bool facingLeft;
    int currentFrame;
    int totalFrames;
    struct timespec animationTimer;
    float frameDuration;

//add hit box

public:

    Bat(Image* img, float startx, float starty, float w, float h, Player& p);
    ~Bat();


    void loadTexture(Image* img);
    void updateAnimation();
    void updateState(BatState newState);
    void move();
    void render();

    GLuint getTexture() const;
    void setPosition(float newX, float newY);
    void setMoveSpeed(float speed);
    float getX() const;
    float getY() const;

//collision dection
}; 

#endif //BTHOMAS