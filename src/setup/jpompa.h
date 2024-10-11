#ifndef JPOMPA_H
#define JPOMPA_H

#include "imageLoader.h"
#include "Global.h"
#include <GL/glx.h>
#include <cstdlib>
#include <ctime>
#include <new>

struct Hitbox{
    float x, y, width, height;
};

struct PlatformNode {
    GLuint texture;
    float x, y, width, height;
    Hitbox hitbox;
    PlatformNode* next;

    PlatformNode(Image* img, float xpos, float ypos, float w, float h);
};

class Platform {
private:
    PlatformNode* head;
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
    void setSpawnInterval(float interval);
    void addPlatform(Image* img, float xpos, float ypos, float w, float h);
    void updatePlatforms(int windowWidth, int windowLength, float deltaTime);
    void render();
    ~Platform();
};

#endif
