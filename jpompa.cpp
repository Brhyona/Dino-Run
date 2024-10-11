#include "src/setup/imageLoader.h"
#include "src/setup/Global.h"
#include "src/setup/jpompa.h"
#include <GL/glx.h>
#include <cstdlib>
#include <ctime>
#include <new>

PlatformNode::PlatformNode(Image* img, float xpos, float ypos, float w, float h)
    : x(xpos), y(ypos), width(w), height(h), next(nullptr) {

    if (!img->data) {
        printf("Failed to load image:");
        return;
    }
        
    printf("Image loaded: Width: %d, Height: %d\n", img->width, img->height);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Platform::Platform(Image* img, float xpos, float ypos, float w, float h) 
    : head(nullptr), lastPlatformX(xpos), lastPlatformY(ypos), platformCounter(0), spawnTimer(0.0f), spawnInterval(3.5f) {
    for(int i = 0; i < 5; i++) {   
        addPlatform(img, xpos, ypos, w, h);
    }
    srand(static_cast<unsigned int>(time(0)));
}

Platform:: ~Platform() {
    deallocate(head);
}

PlatformNode * Platform::create(Image* img, float xpos, float ypos, float w, float h) {
    PlatformNode * newNode;
    try {
        newNode = new PlatformNode(img, xpos, ypos, w, h);
        platformCounter++;
        printf("PlatformNode created at (%f, %f) with width %f and height %f\n", xpos, ypos, w, h);
    } catch(std::bad_alloc& e) {
        newNode = nullptr;
    }
    return newNode;
}

void Platform::deallocate(PlatformNode * node) {
    if(node == nullptr) {
        return;
    }
    deallocate(node->next);
    printf("Deleting PlatformNode at (%f, %f)\n", node->x, node->y);
    delete node;
    platformCounter--;
}

float Platform::randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}


void Platform::setSpawnInterval(float interval) {
    spawnInterval = interval;
}

void Platform::addPlatform(Image* img, float xpos, float ypos, float w, float h) {

    PlatformNode * newNode = create(img, xpos, ypos, w, h);
        
    lastPlatformX = xpos;
    lastPlatformY = ypos;

    if(newNode == nullptr) {
        return;
    }
        
    PlatformNode * temp = head;
    if(temp == nullptr)
    {
        newNode->next = nullptr;
        head = newNode;
    } else {
        newNode->next = temp;
        head = newNode;
    }
}

void Platform::updatePlatforms(int windowWidth, int windowLength, float deltaTime) {
    spawnTimer += deltaTime;
    PlatformNode * temp = head;
    while (temp) {
        temp->x -= 1.5f;

        if (temp->x + temp->width / 2 < 0 && spawnTimer >= spawnInterval) {
            spawnTimer = 1.0f;
            //temp->x = g.xres + temp->width /2;
            float randomSpacing = randomFloat(100.0f, 150.0f);

            temp->x = windowWidth + randomSpacing;

            float verticalOffset = randomFloat(-100.0f, 150.0f);
            temp->y = lastPlatformY + verticalOffset;

            if(temp->y < 75.0f) {
                temp->y = 75.0f;
            }

            if(temp->y > windowLength - 100.0f) {
                temp->y = windowLength - 100.0f;
            }

            lastPlatformX = temp->x;
            lastPlatformY = temp->y;
                
        }
        temp = temp->next;   
    }
}

void Platform::render() {
    PlatformNode * temp = head;
    while (temp) {
        glPushMatrix();
        glTranslatef(temp->x, temp->y, 0);
        glBindTexture(GL_TEXTURE_2D, temp->texture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-temp->width / 2, -temp->height / 2);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-temp->width / 2, temp->height / 2);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(temp->width / 2, temp->height / 2);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(temp->width / 2, -temp->height / 2);
        glEnd();
        glPopMatrix();
        temp = temp -> next;
    }
}
