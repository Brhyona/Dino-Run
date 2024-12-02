#ifndef JPOMPA_H
#define JPOMPA_H

#include "imageLoader.h"
#include "Global.h"
#include "melvir.h"
#include "oal.h"
#include <GL/glx.h>
#include <cstdlib>
#include <ctime>
#include <new>

class Player;

struct PlatformNode {
    GLuint texture;
    float x, y, width, height;
    Hitbox hitbox;
    PlatformNode* next;
    float fallTimer;
    bool isFalling;

    PlatformNode(Image* img, float xpos, float ypos, float w, float h);
};

struct CoinNode {
    GLuint texture;
    float x, y, width, height;
    Hitbox hitbox;
    CoinNode* next;

    CoinNode(Image* img, float xpos, float ypos, float w, float h);
};

#ifdef USE_OPENAL_SOUND
class Sound {
public:
    void initSound();
    void cleanupSound();
    void playSound(ALuint source);
};
extern Sound sound; 
#endif //USE_OPENAL_SOUND

class Platform {
private:
    PlatformNode* platformHead;
    float lastPlatformX;
    float lastPlatformY;
    int platformCounter;
    float spawnTimer;
    float spawnInterval;

    PlatformNode* create(Image* img, float xpos, float ypos, float w, float h);
    void deallocate(PlatformNode* node);
    float randomFloat(float min, float max);

public:
    Platform(Image* img, float xpos, float ypos, float w, float h);
    PlatformNode* getPlatformHead() const { return platformHead; }
    void setSpawnInterval(float interval);
    float getLastPlatformX() const {return lastPlatformX;}
    float getLastPlatformY() const {return lastPlatformY;}
    void addPlatform(Image* img, float xpos, float ypos, float w, float h);
    void updatePlatforms(int windowWidth, int windowLength, float deltaTime, const Player& player);
    void render();
    ~Platform();
};

class Coin {
private:
    CoinNode * coinHead;
    float lastCoinX;
    float lastCoinY;
    int coinCounter;
    float spawnTimer;
    float spawnInterval;
    Platform *platformPos;
    int maxCoins;
    int coinsCollected;

    CoinNode* createCoin(Image* img, float xpos, float ypos, float w, float h);
    void deallocateCoins(CoinNode* coinNode);
    float randomFloat(float min, float max);

public:
    Coin(Image* img, float xpos, float ypos, float w, float h, Platform *platformPos);
    CoinNode* getCoins() const;
    void setSpawnInterval(float interval);
    void addCoin(Image* img, float xpos, float ypos, float w, float h);
    void monitorCoins(Image* img, float xpos, float ypos, float w, float h);
    void updateCoins(int windowWidth, int windowLength, float deltaTime);
    void checkCoinCollision(Player &player, Sound &sound);
    void removeCoin(CoinNode * coin);
    void renderCoins();
    ~Coin();
};

class Arrow {
private:
    float x, y, width, height;
    float velocityX, velocityY;
    Hitbox arrowHitbox;
    GLuint texture;
    bool isActive;
    bool isSniperActive;
    bool isArrowTrapActive;
    float damage;
    float randomFloat(float min, float max);

public:
    Arrow(Image * img, float xpox, float ypox, float w, float h);
    void shootSniperArrow(Image* img, float xpos, float ypos, float w, float h, Player &player);
    void spawnArrowTrap(int xres, int yres, Arrow& arrows, Player &player);
    void updateArrow(int xres, int yres, float deltaTime, Player &player);
    void renderArrow();
    float getDamage() const { return damage; }  
    bool getIsActive() const { return isActive; }
    const Hitbox& getHitbox() const { return arrowHitbox; }
};

class FireTrap {
private:
    float x, y, width, height;
    float fireTimer, fireCoolDown, fireDamage;
    Hitbox fireTrapHitbox;
    GLuint texture;
    bool fireTrapActive;
    PlatformNode* platformToSpawnOn;

public:
    FireTrap(Image * img, float xpox, float ypox, float w, float h);
    void spawnFireTrap(int xres, int yres, Platform& platform, Player &player);
    void updateFire(int xres, int yres, float deltaTime, Player &player);
    void renderFire();
    float getFireDamage() const { return fireDamage; }  
    bool getIsFireActive() const { return fireTrapActive; }
    const Hitbox& getFireHitbox() const { return fireTrapHitbox; }
};

#endif