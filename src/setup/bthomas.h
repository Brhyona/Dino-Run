// bthomas.h

#ifndef BTHOMAS_H
#define BTHOMAS_H

#include "imageLoader.h"
#include<vector>
#include "Global.h"
#include <GL/glx.h>
#include "melvir.h"
#include<chrono>

void check_buttons(int mousex, int mousey);

enum BatState {
    BAT_FLYING,
    NUM_BAT_STATES
};

enum ButtonType {
    START,
    PAUSE,
    INFO,
    CLOSE_INFO,
    RETURN,
    QUIT
};

class Player;

struct Button 
{
    int left, right, bottom, top;
    Button(float xresFactor, float yresFactor, float xOffset = 0.5f, 
        float yOffset = 0.25f) {
        left = g.xres * xOffset - (g.xres * xresFactor);
        right = g.xres * xOffset + (g.xres * xresFactor);
        bottom = g.yres * yOffset - (g.yres * yresFactor);
        top = g.yres * yOffset + (g.yres * yresFactor);
    }
    bool isClicked(int mousex, int mousey) const {
        return mousex >= left &&
                mousex <= right && 
                mousey >= bottom &&
                mousey <= top;
    }
};

class Fireball 
{
private:
    float x, y;
    float velocityX, velocityY;
    float width;
    float height;
    float gravity;
    void findContact(const Hitbox& box, float circleX, float circleY, 
                    float& closestX, float& closestY) const {
        closestX = std::max(box.x, std::min(circleX, box.x + box.width));
        closestY = std::max(box.y, std::min(circleY, box.y + box.height));
    }
    static constexpr float FRAME_DURATION = 0.05f;
    static constexpr int TOTAL_FRAMES = 15;
    
public:
    Fireball( float startX, float startY, float iniX, float iniY );
    ~Fireball();
    bool active; 
    bool hasDamaged = false;
    int currentFrame;
    float animTimer;
    GLuint texture;
    void update();
    void render_fire();
    bool isActive() const { return active;}
    void deactivate() { active = false; }
    bool checkCollision(const Player& player) const;
    float getCollisionRadius() const { return width / 6.0f;}
    static const int DAMAGE_AMOUNT = 2;
};

class Bat  
{
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
    static constexpr float DESPAWN_TIME = 10.0f;
     bool firstSpawn = true;
    float moveDirection = 1.0f;
    const float SCREEN_MARGIN = 50.0f;
    const float PATROL_SPEED = 2.0f;
    const float PATROL_HEIGHT = 400.0f;
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
    float originalX = 0.0f;
    Hitbox hitbox;
    void updateHitbox() {
        hitbox.x = x; 
        hitbox.y = y;
        hitbox.width = width*3;
        hitbox.height = height*3;
    }
    void performSwoop(float deltaTime);
    float targetSwoopX;
    float targetSwoopY;

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
#endif //BTHOMAS
