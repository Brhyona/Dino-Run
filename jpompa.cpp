/*
    Jorge Pompa
    CMPS 3650
    10/26/2024

    Platform and Coin logic.
*/

#include "src/setup/imageLoader.h"
#include "src/setup/Global.h"
#include "src/setup/jpompa.h"
#include <GL/glx.h>
#include <cstdlib>
#include <ctime>
#include <new>

Timers timers;

PlatformNode::PlatformNode(Image* img, float xpos, float ypos, float w, float h)
    : x(xpos), y(ypos), width(w), height(h), next(nullptr) {
    
    hitbox = {x, y, width, height};
    if (!img->data) {
        printf("Failed to load image:");
        return;
    }
    #ifdef DEBUG        
    printf("Image loaded: Width: %d, Height: %d\n", img->width, img->height);
    #endif
    texture = g.tex.platformTexture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Platform::Platform(Image* img, float xpos, float ypos, float w, float h) 
    : platformHead(nullptr), lastPlatformX(xpos), lastPlatformY(ypos), platformCounter(0), spawnTimer(0.0f), spawnInterval(2.0f) {
    for(int i = 0; i < 4; i++) {   
        addPlatform(img, xpos, ypos, w, h);
    }
    srand(static_cast<unsigned int>(time(0)));
}

Platform:: ~Platform() {
    deallocate(platformHead);
}

PlatformNode * Platform::create(Image* img, float xpos, float ypos, float w, float h) {
    PlatformNode * newNode;
    try {
        newNode = new PlatformNode(img, xpos, ypos, w, h);
        platformCounter++;
        #ifdef DEBUG
        printf("PlatformNode created at (%f, %f) with width %f and height %f\n", xpos, ypos, w, h);
        #endif 
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
        
    PlatformNode * temp = platformHead;
    if(temp == nullptr)
    {
        newNode->next = nullptr;
        platformHead = newNode;
    } else {
        newNode->next = temp;
        platformHead = newNode;
    }
}

void Platform::updatePlatforms(int windowWidth, int windowLength, float deltaTime) {
    spawnTimer += deltaTime;
    PlatformNode * temp = platformHead;
    float minPlatformSpaceing = windowWidth * 0.1f;
    float maxPlatformSpaceing = windowWidth * 0.3f;
    float verticalSpacing = windowLength * 0.1f;
    
    while (temp) {
        temp->x -= 1.0f;
        temp->hitbox.x = temp->x;
        temp->hitbox.y = temp->y;

        if (temp->x + temp->width / 2 < 0 && spawnTimer >= spawnInterval) {
            spawnTimer = 0.0f;
            float randomSpacing = randomFloat(minPlatformSpaceing, maxPlatformSpaceing);
            float verticalOffset = randomFloat(-verticalSpacing, verticalSpacing);

            temp->x = lastPlatformX + randomSpacing;
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
    PlatformNode * temp = platformHead;
    while (temp) {
        #ifdef DEBUG
        glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f); 
        glBegin(GL_LINE_LOOP);
            glVertex2f(temp->hitbox.x - temp->width / 2, temp->hitbox.y - temp->height / 2);
            glVertex2f(temp->hitbox.x - temp->width / 2, temp->hitbox.y + temp->height / 2);
            glVertex2f(temp->hitbox.x + temp->width / 2, temp->hitbox.y + temp->height / 2);
            glVertex2f(temp->hitbox.x + temp->width / 2, temp->hitbox.y - temp->height / 2);
        glEnd();
        glPopMatrix();
        #endif

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
    //glColor3f(1.0f, 1.0f, 1.0f);
}

CoinNode::CoinNode(Image* img, float xpos, float ypos, float w, float h)
    : x(xpos), y(ypos), width(w), height(h), next(nullptr) {

    hitbox = {x, y, width, height};
    
    if (!img->data) {
        printf("Failed to load image:");
        return;
    }

    #ifdef DEBUG    
    printf("Coin image loaded: Width: %d, Height: %d\n", img->width, img->height);
    #endif
    texture = g.tex.coinTexture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Coin::Coin(Image* img, float xpos, float ypos, float w, float h, Platform *platformPos) 
    : coinHead(nullptr), lastCoinX(xpos), lastCoinY(ypos), coinCounter(0), spawnTimer(0.0f), spawnInterval(2.0f), platformPos(platformPos) {
    for(int i = 0; i < 10; i++) {
        addCoin(img, xpos, ypos, w, h);
    }
    srand(static_cast<unsigned int>(time(0)));
}

Coin:: ~Coin() {
    deallocateCoins(coinHead);
}

CoinNode * Coin::createCoin(Image* img, float xpos, float ypos, float w, float h) {
    CoinNode * newCoin;
    try {
        newCoin = new CoinNode(img, xpos, ypos, w, h);
        coinCounter++;
        #ifdef DEBUG
        printf("CoinNode created at (%f, %f) with width %f and height %f\n", xpos, ypos, w, h);
        #endif
    } catch(std::bad_alloc& e) {
        newCoin = nullptr;
    }
    return newCoin;
}

void Coin::addCoin(Image* img, float xpos, float ypos, float w, float h) {

    CoinNode * newCoin = createCoin(img, xpos, ypos, w, h);
        
    lastCoinX = xpos;
    lastCoinY = ypos;

    if(newCoin == nullptr) {
        return;
    }
        
    CoinNode * temp = coinHead;
    if(temp == nullptr)
    {
        newCoin->next = nullptr;
        coinHead = newCoin;
    } else {
        newCoin->next = temp;
        coinHead = newCoin;
    }
}

void Coin::updateCoins(int windowWidth, int windowLength, float deltaTime) {
    spawnTimer += deltaTime;
    CoinNode * coinTemp = coinHead;
    
    float minCoinSpacing = windowWidth * 0.05f;  
    float maxCoinSpacing = windowWidth * 0.1f;   
    float verticalSpacing = windowLength * 0.1f;

    //Sprite sheet render using Gordons timers method.
    timers.recordTime(&timers.timeCurrent);
	double timeSpan = timers.timeDiff(&g.coinTime, &timers.timeCurrent);
	
    if (timeSpan > g.delay) {
		g.coinFrame++;
		if (g.coinFrame >= 2) {
			g.coinFrame = 0;
		}
		timers.recordTime(&g.coinTime);
	}

    //Hitbox setting
    coinTemp->hitbox.x = coinTemp->x;
    coinTemp->hitbox.y = coinTemp->y;     
    
    while (coinTemp) {
        coinTemp->x -= 1.0f;

        if (coinTemp->x + coinTemp->width / 2 < 0 && spawnTimer >= spawnInterval) {
            spawnTimer = 2.0f;
            float randomSpacing = randomFloat(minCoinSpacing, maxCoinSpacing);
            float verticalOffset = randomFloat(-verticalSpacing, verticalSpacing);
            float platformY = platformPos->getLastPlatformY();
            float platformX = platformPos->getLastPlatformX();   

            if(lastCoinX == -1) {
                coinTemp->x = platformX + randomSpacing;
                coinTemp->y = platformY + verticalOffset;
            } else {
                coinTemp->x = lastCoinX + randomSpacing;
                coinTemp->y = lastCoinY + verticalOffset;
            }

            if (coinTemp->y < platformY) {
                coinTemp->y = platformY + randomFloat(10.0f, 20.0f);
            }

            //if(coinTemp->y < 20.0f) {
            //    coinTemp->y = 15.0f;
            //}


            lastCoinX = coinTemp->x;
            lastCoinY = coinTemp->y;
                
        }
        coinTemp = coinTemp->next;   
    }
}

void Coin::renderCoins() {
    CoinNode * coinTemp = coinHead;
    int totalFrames = 25;
    float frameWidth = 1.0f / totalFrames;

    float textureLeft = g.coinFrame * frameWidth;
    float textureRight = textureLeft + frameWidth;
    
    while (coinTemp) {
        #ifdef DEBUG
        glPushMatrix();
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(coinTemp->hitbox.x - coinTemp->width / 2, coinTemp->hitbox.y - coinTemp->height / 2);
            glVertex2f(coinTemp->hitbox.x - coinTemp->width / 2, coinTemp->hitbox.y + coinTemp->height / 2);
            glVertex2f(coinTemp->hitbox.x + coinTemp->width / 2, coinTemp->hitbox.y + coinTemp->height / 2);
            glVertex2f(coinTemp->hitbox.x + coinTemp->width / 2, coinTemp->hitbox.y - coinTemp->height / 2);
        glEnd();
        glPopMatrix();
        #endif

        glPushMatrix();
        glTranslatef(coinTemp->x, coinTemp->y, 0);
        glBindTexture(GL_TEXTURE_2D, g.tex.coinTexture);
        glBegin(GL_QUADS);
            glTexCoord2f(textureLeft, 1.0f); glVertex2f(-coinTemp->width / 2, -coinTemp->height / 2);
            glTexCoord2f(textureLeft, 0.0f); glVertex2f(-coinTemp->width / 2, coinTemp->height / 2);
            glTexCoord2f(textureRight, 0.0f); glVertex2f(coinTemp->width / 2, coinTemp->height / 2);
            glTexCoord2f(textureRight, 1.0f); glVertex2f(coinTemp->width / 2, -coinTemp->height / 2);
        glEnd();
        glPopMatrix();
        
        coinTemp = coinTemp -> next;
    }
    //glColor3f(1.0f, 1.0f, 1.0f);
}

void Coin::setSpawnInterval(float interval) {
    spawnInterval = interval;
}

float Coin::randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void Coin::deallocateCoins(CoinNode * coinNode) {
    if(coinNode == nullptr) {
        return;
    }
    deallocateCoins(coinNode->next);
    printf("Deleting CoinNode at (%f, %f)\n", coinNode->x, coinNode->y);
    delete coinNode;
    coinCounter--;
}
 
