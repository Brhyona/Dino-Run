// bthomas.h

#ifndef BTHOMAS_H
#define BTHOMAS_H

#include "imageLoader.h"
#include<vector>
#include "Global.h"
#include <GL/glx.h>
#include "melvir.h"
#include<chrono>

enum BatState {
 BAT_FLYING,
 NUM_BAT_STATES
};
class Player;

class Fireball {
private:
    float x, y;
    float velocityX, velocityY;
    float width;
    float height;
    float gravity;
    void findContact(const Hitbox& box, float circleX, float circleY, float& closestX, float& closestY) const {
        closestX = std::max(box.x, std::min(circleX, box.x + box.width));
        closestY = std::max(box.y, std::min(circleY, box.y + box.height));
    }
    static constexpr float FRAME_DURATION = 0.05f;
    static constexpr int TOTAL_FRAMES = 15;
    
public:
    Fireball( float startX, float startY, float iniX, float iniY );
   // ~Fireball();
   bool active; 
    bool hasDamaged = false;
    int currentFrame;
    float animTimer;
    void update();
    void render_fire();
    bool isActive() const { return active;}
    void deactivate() { active = false; }
    bool checkCollision(const Player& player) const;
    float getCollisionRadius() const { return width / 6.0f;}
    static const int DAMAGE_AMOUNT = 2;
    
};

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
    float lastFireballTime;
    bool isSpawned;
    bool getIsSpawned();
    float spawnTimer;
    const float SPAWN_DELAY = 5.0f;
    bool timerStarted;
    static constexpr float DESPAWN_TIME = 30.0f;
     bool firstSpawn = true;
    float moveDirection = 1.0f;
    const float SCREEN_MARGIN = 50.0f;
    const float PATROL_SPEED = 1.0f;
    const float PATROL_HEIGHT = 700.0f;
    void followPlayer();
    void patrolMovement();
    bool isFollowing;
    float lifeTimer;
    bool isSwooping = false;
    float swoopTimer = 0.0f;
    float swoopAngle = 0.0f;
    const float SWOOP_INTERVAL = 2.0f;
    const float SWOOP_SPEED = 1.0f;
    float SWOOP_AMPLITUDE = 400.0f;
    float originalY = 0.0f;
    Hitbox hitbox;
    void updateHitbox() {
        hitbox.x = x- width/2;
        hitbox.y = y -height/2;
        hitbox.width = width;
        hitbox.height = height;
    }
    void performSwoop(float deltaTime);
    float targetSwoopX;
    float targetSwoopY;

//add hit box
    public:

    Bat(Image* img, float startx, float starty, float w, float h, Player& p);
    ~Bat();
    const Hitbox& getBatbox() const { return hitbox;}
     std::chrono::steady_clock::time_point startTime;
    bool IsActive();
    void updateSpawnTimer(float deltaTime);
    void updateAnimation();
    void updateState(BatState newState);
    void move();
    void toggleMovement(); 
    void render();
    std::vector<Fireball> fireballs;
    GLuint getTexture() const;
    void setPosition(float newX, float newY);
    void setMoveSpeed(float speed);
    float getX() const;
    float getY() const;
    void shootFireball();
    void updateFireball();
    void renderFireball();
    static constexpr float FIREBALL_SPEED = 0.01f;
    static constexpr size_t MAX_FIREBALLS = 20;
    std::vector<Fireball>& getFireballs() { return fireballs;}
    void cleanupFire();
    void checkFireCollision(Player& player);
    void updateTimer(float deltaTime);
    void reset();  
    void updateSwooping (float deltaTime);
    void resetTimer();

};

class Menu {
    private:


    public:

    
};
    #endif //BTHOMAS