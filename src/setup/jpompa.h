#ifndef JPOMPA_H
#define JPOMPA_H

#include "imageLoader.h"
#include "Global.h"
#include "melvir.h"
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

    PlatformNode(Image* img, float xpos, float ypos, float w, float h);
};

struct CoinNode {
    GLuint texture;
    float x, y, width, height;
    Hitbox hitbox;
    CoinNode* next;

    CoinNode(Image* img, float xpos, float ypos, float w, float h);
};

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
    void updatePlatforms(int windowWidth, int windowLength, float deltaTime);
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
    void checkCoinCollision(Player &player);
    void removeCoin(CoinNode * coin);
    void renderCoins();
    ~Coin();
};

#endif
