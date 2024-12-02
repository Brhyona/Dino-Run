/*
    Jorge Pompa
    CMPS 3650
    10/26/2024

    Platform and Coin logic.
*/

#include "src/setup/imageLoader.h"
#include "src/setup/Global.h"
#include "src/setup/jpompa.h"
#include "src/setup/melvir.h"
#include "src/setup/health.h"

#include <GL/glx.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <new>

#include <stdio.h>
#include <string>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef USE_OPENAL_SOUND
#include "src/setup/oal.h"
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

Timers timers;
Sound sound;

#ifdef USE_OPENAL_SOUND

void Sound::initSound()
{
	#ifdef USE_OPENAL_SOUND
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	g.alBufferCoin = alutCreateBufferFromFile("./src/sounds/coin.wav");
	g.alBufferJump = alutCreateBufferFromFile("./src/sounds/jump.wav");
	g.alBufferFalling = alutCreateBufferFromFile("./src/sounds/falling.wav");
	g.alBufferBones = alutCreateBufferFromFile("./src/sounds/bones.wav");
	//
	//Source refers to the sound.
	//Generate a source, and store it in a buffer.
	alGenSources(1, &g.alSourceCoin);
	alSourcei(g.alSourceCoin, AL_BUFFER, g.alBufferCoin);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(g.alSourceCoin, AL_GAIN, 1.0f);
	alSourcef(g.alSourceCoin, AL_PITCH, 1.0f);
	alSourcei(g.alSourceCoin, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	//Generate a source, and store it in a buffer.
	alGenSources(1, &g.alSourceJump);
	alSourcei(g.alSourceJump, AL_BUFFER, g.alBufferJump);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(g.alSourceJump, AL_GAIN, 1.0f);
	alSourcef(g.alSourceJump, AL_PITCH, 1.0f);
	alSourcei(g.alSourceJump, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	
    alGenSources(1, &g.alSourceFalling);
	alSourcei(g.alSourceFalling, AL_BUFFER, g.alBufferFalling);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(g.alSourceFalling, AL_GAIN, 1.0f);
	alSourcef(g.alSourceFalling, AL_PITCH, 1.0f);
	alSourcei(g.alSourceFalling, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	//Generate a source, and store it in a buffer.
	alGenSources(1, &g.alSourceBones);
	alSourcei(g.alSourceBones, AL_BUFFER, g.alBufferBones);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(g.alSourceBones, AL_GAIN, 1.0f);
	alSourcef(g.alSourceBones, AL_PITCH, 1.0f);
	alSourcei(g.alSourceBones, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
    
	#endif //USE_OPENAL_SOUND
}

void Sound::cleanupSound()
{
	#ifdef USE_OPENAL_SOUND
	//First delete the source.
	alDeleteSources(1, &g.alSourceCoin);
	alDeleteSources(1, &g.alSourceJump);
	//Delete the buffer.
	alDeleteBuffers(1, &g.alBufferCoin);
	alDeleteBuffers(1, &g.alBufferJump);
	//Close out OpenAL itself.
	//Get active context.
	ALCcontext *Context = alcGetCurrentContext();
	//Get device for active context.
	ALCdevice *Device = alcGetContextsDevice(Context);
	//Disable context.
	alcMakeContextCurrent(NULL);
	//Release context(s).
	alcDestroyContext(Context);
	//Close device.
	alcCloseDevice(Device);
	#endif //USE_OPENAL_SOUND
}

void Sound::playSound(ALuint source)
{
	#ifdef USE_OPENAL_SOUND
	alSourcePlay(source);
	#endif //USE_OPENAL_SOUND
}

#endif //USE_OPENAL_SOUND

PlatformNode::PlatformNode(Image* img, float xpos, float ypos, float w, float h)
    : x(xpos), y(ypos), width(w), height(h), next(nullptr), fallTimer(0.0f), isFalling(false) {
    
    float plankHeight = h * 0.1f;  // Set height of the plank (10% of platform height)
    float plankYOffset = -h / 2 + plankHeight / 2;  // Align hitbox with plank

    hitbox = {xpos - width / 2, ypos + plankYOffset, width, plankHeight};
 
    if (!img->data) {
        printf("Failed to load image:");
        return;
    }
    
    #ifdef DEBUG        
    printf("Image loaded: Width: %d, Height: %d\n", img->width, img->height);
    #endif

    if (!isFalling) {
        texture = g.tex.fallingPlatformTexture;
    } else {
        texture = g.tex.platformTexture;
    }

    //texture = g.tex.platformTexture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Platform::Platform(Image* img, float xpos, float ypos, float w, float h) 
    : platformHead(nullptr), lastPlatformX(xpos), lastPlatformY(ypos), platformCounter(0), spawnTimer(0.0f), spawnInterval(1.0f) {
    float initialSpacing = 600.0f;
    for(int i = 0; i < 4; i++) {  
        addPlatform(img, xpos + i * initialSpacing, ypos, w, h);

        //addPlatform(img, xpos, ypos, w, h);
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

void Platform::updatePlatforms(int windowWidth, int windowLength, float deltaTime, const Player& player) {
    spawnTimer += deltaTime;
    PlatformNode* temp = platformHead;
    float minPlatformSpacing = windowWidth * 0.001f;
    float maxPlatformSpacing = windowWidth * 0.001f;
    float verticalSpacing = windowLength * 0.12f;
    float maxGapThreshold = windowWidth * 0.001f;
    float fallingProb = 0.5f;

    while (temp) {
        // Check if the player is on this specific platform
        const Hitbox& platformHitbox = temp->hitbox;
        const Hitbox& playerHitbox = player.getPlayerHitbox();

        bool isPlayerOnThisPlatform =
            playerHitbox.x < platformHitbox.x + platformHitbox.width &&
            playerHitbox.x + playerHitbox.width > platformHitbox.x &&
            playerHitbox.y >= platformHitbox.y &&
            playerHitbox.y - 5 <= platformHitbox.y + platformHitbox.height;

        if (temp->isFalling && isPlayerOnThisPlatform) {
            // Start falling if the player is on this platform
            temp->fallTimer += deltaTime;
            temp->y -= 150.0f * deltaTime; // Falling speed
        } else {
            temp->x -= 1.0f; // Regular platform movement
        }

        // Update hitbox
        float plankHeight = temp->height * 0.1f;
        float plankWidth = temp->width * 0.8f;
        temp->hitbox.x = temp->x - plankWidth / 2;
        temp->hitbox.y = temp->y - plankHeight / 2;
        temp->hitbox.width = plankWidth;
        temp->hitbox.height = plankHeight;

        // Recycle platforms when they go off-screen
        if (temp->x + temp->width / 2 < 0 && spawnTimer >= spawnInterval) {
            spawnTimer = 0.0f;
            float randomSpacing = randomFloat(minPlatformSpacing, maxPlatformSpacing);
            float verticalOffset = randomFloat(-verticalSpacing, verticalSpacing);

            if (randomSpacing > maxGapThreshold) {
                randomSpacing = maxGapThreshold;
            } else if (randomSpacing < minPlatformSpacing) {
                randomSpacing = minPlatformSpacing;
            }

            temp->x = lastPlatformX + randomSpacing;
            temp->y = lastPlatformY + verticalOffset;

            if (temp->y < 75.0f) {
                temp->y = 75.0f;
            }

            if (temp->y > windowLength - 100.0f) {
                temp->y = windowLength - 100.0f;
            }

            temp->isFalling = (randomFloat(0.0f, 1.0f) < fallingProb);

            lastPlatformX = temp->x;
            lastPlatformY = temp->y;
        }

        temp = temp->next;
    }
}

void Platform::render() {

    PlatformNode * temp = platformHead;
    while (temp) {
 
        //glBindTexture(GL_TEXTURE_2D, temp->texture);
        glBindTexture(GL_TEXTURE_2D, temp->isFalling ? g.tex.fallingPlatformTexture : g.tex.platformTexture);
        glPushMatrix();
        glTranslatef(temp->x, temp->y, 0);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-temp->width / 2, -temp->height / 2);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-temp->width / 2, temp->height / 2);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(temp->width / 2, temp->height / 2);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(temp->width / 2, -temp->height / 2);
        glEnd();
        glPopMatrix();

        #ifdef DEBUG
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f); // Set hitbox color to red
        glBegin(GL_LINE_LOOP);
            glVertex2f(temp->hitbox.x, temp->hitbox.y); // Bottom-left
            glVertex2f(temp->hitbox.x, temp->hitbox.y + temp->hitbox.height); // Top-left
            glVertex2f(temp->hitbox.x + temp->hitbox.width, temp->hitbox.y + temp->hitbox.height); // Top-right
            glVertex2f(temp->hitbox.x + temp->hitbox.width, temp->hitbox.y); // Bottom-right
        glEnd();
        glPopMatrix();
        glEnable(GL_TEXTURE_2D); 

        //printf("Hitbox - x: %f, y: %f, width: %f, height: %f\n", 
        //       temp->hitbox.x, temp->hitbox.y, temp->hitbox.width, temp->hitbox.height);
        #endif
        
        glColor3f(1.0f, 1.0f, 1.0f);
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
    : coinHead(nullptr), lastCoinX(xpos), lastCoinY(ypos), coinCounter(0), spawnTimer(0.0f), spawnInterval(2.0f), platformPos(platformPos), maxCoins(5) {
    for(int i = 0; i < maxCoins; i++) {
        //addCoin(img, xpos, ypos, w, h);
        monitorCoins(img, xpos, ypos, w, h);
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
        //coinCounter++;
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
    coinCounter++;
}

void Coin::updateCoins(int windowWidth, int windowLength, float deltaTime) {
    spawnTimer += deltaTime;
    CoinNode * coinTemp = coinHead;
    
    float minCoinSpacing = windowWidth * 0.1f;  
    float maxCoinSpacing = windowWidth * 0.2f;   
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
 
    while (coinTemp) {
        coinTemp->x -= 1.0f;
        coinTemp->hitbox.x = coinTemp->x;
        coinTemp->hitbox.y = coinTemp->y;     

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
            /* 
            if (coinTemp->y < platformY) {
                coinTemp->y = platformY + 30.0f;
            }
            
            if (coinTemp->y > platformY) {
                coinTemp->y = platformY + 30.0f;
            }
            */       
            lastCoinX = coinTemp->x;
            lastCoinY = coinTemp->y;
                
        }
        coinTemp = coinTemp->next;   
    }

    //std::cout << "Coins on screen: " << coinCounter << std::endl;
   
}

void Coin::monitorCoins(Image* img, float xpos, float ypos, float w, float h) {
    // Ensure we only manage coins if we need more
    if (coinCounter >= maxCoins) {
        return;
    }

    float minCoinSpacing = 50.0f;  // Minimum distance between coins
    float maxCoinSpacing = 150.0f; // Maximum distance between coins
    float verticalSpacing = 50.0f; // Maximum vertical offset

    while (coinCounter < maxCoins) {
        // Generate random spacing and offsets
        float verticalOffset = randomFloat(-verticalSpacing, verticalSpacing);

        float platformY = platformPos->getLastPlatformY();
        float platformX = platformPos->getLastPlatformX();

        // Calculate new coin positions
        float newCoinX = (lastCoinX == -1) ? platformX + minCoinSpacing : lastCoinX + maxCoinSpacing;
        float newCoinY = (lastCoinY == -1) ? platformY + verticalOffset : platformY + verticalOffset;

        if (newCoinY < platformY + 50.0f) {  
            newCoinY = platformY + 50.0f;
        }

        // Ensure coins don't go off the screen vertically
        if (newCoinY < 50.0f) newCoinY = 50.0f;
        if (newCoinY > ypos - 50.0f) newCoinY = ypos - 50.0f;

        // Ensure coins don't go off the screen vertically
        if (newCoinY < 50.0f) newCoinY = 50.0f;
        if (newCoinY > ypos - 50.0f) newCoinY = ypos - 50.0f;

        // Add the new coin
        addCoin(img, newCoinX, newCoinY, w, h);

        // Update last coin position
        lastCoinX = newCoinX;
        lastCoinY = newCoinY;
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

CoinNode* Coin::getCoins() const {
    return coinHead;
}

void Coin::removeCoin(CoinNode* coin) {
    if (!coinHead || !coin) return;

    if (coinHead == coin) {
        CoinNode* temp = coinHead;
        coinHead = coinHead->next;
        delete temp;
        coinCounter--;
        return;
    }

    CoinNode* prev = coinHead;
    while (prev->next && prev->next != coin) {
        prev = prev->next;
    }

    if (prev->next == coin) {
        CoinNode* temp = prev->next;
        prev->next = temp->next;
        delete temp;
                coinCounter--;
    }
}

void Coin::checkCoinCollision(Player &player, Sound &sound) {
    CoinNode *coinTemp = coinHead;

    while (coinTemp) {
        CoinNode *nextCoin = coinTemp->next;

        const Hitbox &playerHitbox = player.getPlayerHitbox();
        const Hitbox &coinHitbox = coinTemp->hitbox;

        bool isColliding = playerHitbox.x < coinHitbox.x + coinHitbox.width &&
                           playerHitbox.x + playerHitbox.width > coinHitbox.x &&
                           playerHitbox.y < coinHitbox.y + coinHitbox.height &&
                           playerHitbox.y + playerHitbox.height > coinHitbox.y;

        if (isColliding) {
            //std::cout << "Player collided with coin (" << coinHitbox.x << ", " << coinHitbox.y << ")" << std::endl;
            #ifdef USE_OPENAL_SOUND
            sound.playSound(g.alSourceCoin);
            std::cout << "Coins sound played:" << coinsCollected << std::endl;
            #endif
            removeCoin(coinTemp);
            coinsCollected++;
            //std::cout << "Coins collected:" << coinsCollected << std::endl;
            //std::cout << "Coins created: " << coinCounter << std::endl;
        }
        coinTemp = nextCoin; 
    }
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

Arrow::Arrow(Image* img, float xpos, float ypos, float w, float h)
    : x(xpos), y(ypos), width(w), height(h), isActive(false), damage(50.0){

    arrowHitbox = {xpos - w / 2, y - h / 2, w, h};
    
    if (!img->data) {
        printf("Failed to load image:");
        return;
    }

    texture = g.tex.arrowTexture;
    //#ifdef DEBUG    
    printf("Arrow image loaded: Width: %d, Height: %d\n", img->width, img->height);
    //#endif
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    std::cout << "Arrow texture ID: " << g.tex.arrowTexture << std::endl;
    std::cout << "Arrow object texture ID: " << texture << std::endl;

}

float Arrow::randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

void Arrow::shootSniperArrow(Image *img, float xpos, float ypos, float w, float h, Player &player) {
    x = xpos;
    float arrowTriggerProb = 0.5;
    y = ypos;
    width = w;
    height = h;
    
    if (randomFloat(0.0f, 1.0f) < arrowTriggerProb) {
        x = -50.0f;
    } else {
        x = 100.0f;
    }

    float playerX = player.getX();
    float playerY = player.getY();
    //const Hitbox& playerHitbox = player.getPlayerHitbox();
    float dx = playerX - x;
    float dy = playerY - y;
    float distance = sqrt(dx * dx + dy * dy);

    velocityX = (dx / distance) * 200.0f; 
    velocityY = (dy / distance) * 200.0f;

    isActive = true; 

    arrowHitbox.x = x - width / 2;
    arrowHitbox.y = y - height / 2; 
    arrowHitbox.width = width;
    arrowHitbox.height = height;
    //printf("Arrow dimensions - x: %f, y: %f, width: %f, height: %f\n",
    //   hitbox.x, hitbox.y, hitbox.width, hitbox.height);

}

void Arrow::spawnArrowTrap(int xres, int yres, Arrow& arrows, Player &player) {
/*
    float playerX = player.getX();
    float playerY = player.getY() + 500.0f;
    int arrowCount = 4;
    float spacing = 25.0f;

    for (int i = 0; i < arrowCount; i++) {
        float offSetX = (i - arrowCount / 2) * spacing;
        Arrow arrow(nullptr, playerX + offSetX, playerY, 32.0f, 16.0f);
        arrow.velocityX = 0.0f; 
        arrow.velocityY = -300.0f; 
        arrow.isActive = true;
    
        isActive = true; 

        arrowHitbox.x = x - width / 2;
        arrowHitbox.y = y - height / 2;
        arrowHitbox.width = width;
        arrowHitbox.height = height;

    }
*/
}

void Arrow::updateArrow(int xres, int yres, float deltaTime, Player &player) {
    if(!isActive) { 
        return; 
    }

    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    arrowHitbox.x = x - width / 2;
    arrowHitbox.y = y - height / 2;

    const Hitbox& playerHitbox = player.getPlayerHitbox();
    bool isColliding = playerHitbox.x < arrowHitbox.x + arrowHitbox.width &&
                       playerHitbox.x + playerHitbox.width > arrowHitbox.x &&
                       playerHitbox.y < arrowHitbox.y + arrowHitbox.height &&
                       playerHitbox.y + playerHitbox.height > arrowHitbox.y;

    if (isColliding) {
        std::cout << "Arrow hit the player! Dealing " << damage << " damage.\n";
        player.takeDamage(damage);
        isActive = false;
    }

    if (y < -50 || x < -50 || x > xres + 50 || y > yres + 50) {
        isActive = false;
    }
}

void Arrow::renderArrow() {
    if (!isActive) {
        return;
    }

    glPushMatrix();
    glTranslatef(x, y, 0.0f); // Position the arrow
    glBindTexture(GL_TEXTURE_2D, g.tex.arrowTexture);

    // Draw the arrow quad
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-width / 2, -height / 2);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(width / 2, -height / 2);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(width / 2, height / 2);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-width / 2, height / 2);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    //glDisable(GL_TEXTURE_2D); 
    glColor3f(1.0f, 0.0f, 0.0f); 

    glBegin(GL_LINE_LOOP);
        glVertex2f(arrowHitbox.x, arrowHitbox.y); 
        glVertex2f(arrowHitbox.x, arrowHitbox.y + arrowHitbox.height); 
        glVertex2f(arrowHitbox.x + arrowHitbox.width, arrowHitbox.y + arrowHitbox.height); 
        glVertex2f(arrowHitbox.x + arrowHitbox.width, arrowHitbox.y); 
    glEnd();

    //glEnable(GL_TEXTURE_2D); // Re-enable textures
    glPopMatrix();

}

FireTrap::FireTrap(Image* img, float xpos, float ypos, float w, float h)
    : x(xpos), y(ypos), width(w), height(h), fireTimer(0.0f), fireCoolDown(5.0f), fireDamage(50.0f), fireTrapActive(false), platformToSpawnOn(nullptr){

    fireTrapHitbox = {xpos - w / 2, y - h / 2, w, h};
    
    if (!img->data) {
        printf("Failed to load image:");
        return;
    }

    texture = g.tex.fireTrapTexture;
    //#ifdef DEBUG    
    printf("FireTrap image loaded: Width: %d, Height: %d\n", img->width, img->height);
    //#endif
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void FireTrap::spawnFireTrap(int xres, int yres, Platform& platform, Player &player) {
    PlatformNode* temp = platform.getPlatformHead();
    float playerX = player.getX();

    while (temp) {
        if (temp->x > playerX && !temp->isFalling) {
            x = temp->hitbox.x + temp->hitbox.width / 2; 
            y = temp->hitbox.y + temp->hitbox.height;    
            
            fireTrapActive = true;
            platformToSpawnOn = temp;

            fireTrapHitbox.x = x - width / 2;
            fireTrapHitbox.y = y - height / 2;
            fireTrapHitbox.width = width;
            fireTrapHitbox.height = height;

            break; 
        }
        temp = temp->next;
    }
}

void FireTrap::updateFire(int xres, int yres, float deltaTime, Player& player) {
    if (!fireTrapActive || !platformToSpawnOn) {
        return;
    }

    // Align fire trap with the platform's movement
    x = platformToSpawnOn->hitbox.x + platformToSpawnOn->hitbox.width / 2;
    y = platformToSpawnOn->hitbox.y + platformToSpawnOn->hitbox.height;

    // Update the fire trap's hitbox
    fireTrapHitbox.x = x - width / 2;
    fireTrapHitbox.y = y - height / 2;

    // Animation logic
    int totalFrames = 14;
    timers.recordTime(&timers.timeCurrent);
    double fireTimeSpan = timers.timeDiff(&g.fireTrapTime, &timers.timeCurrent);

    if (fireTimeSpan > g.delay) {
        g.fireTrapFrame++;
        if (g.fireTrapFrame >= totalFrames) {
            g.fireTrapFrame = 0; // Reset frame loop
        }
        timers.recordTime(&g.fireTrapTime);
    }

    // Collision detection
    const Hitbox& playerHitbox = player.getPlayerHitbox();
    bool isColliding = playerHitbox.x < fireTrapHitbox.x + fireTrapHitbox.width &&
                       playerHitbox.x + playerHitbox.width > fireTrapHitbox.x &&
                       playerHitbox.y < fireTrapHitbox.y + fireTrapHitbox.height &&
                       playerHitbox.y + playerHitbox.height > fireTrapHitbox.y;

    if (isColliding) {
        std::cout << "Player hit by fire trap! Dealing " << fireDamage << " damage.\n";
        player.takeDamage(fireDamage);
    }

    // Deactivate fire trap if the platform moves off-screen
    if (platformToSpawnOn->x + platformToSpawnOn->width < 0) {
        fireTrapActive = false;
        platformToSpawnOn = nullptr;
    }
}

void FireTrap::renderFire() {
    if (!fireTrapActive) {
        return;
    }

    int totalFrames = 14;
    float frameWidth = 1.0f / totalFrames;

    float textureLeft = g.fireTrapFrame * frameWidth;
    float textureRight = textureLeft + frameWidth;
    
    glPushMatrix();
    glTranslatef(x, y, 0.0f); 
    glBindTexture(GL_TEXTURE_2D, g.tex.fireTrapTexture);

    glBegin(GL_QUADS);
        glTexCoord2f(textureLeft, 1.0f); glVertex2f(-width / 2, -height / 2);
        glTexCoord2f(textureRight, 1.0f); glVertex2f(width / 2, -height / 2);
        glTexCoord2f(textureRight, 0.0f); glVertex2f(width / 2, height / 2);
        glTexCoord2f(textureLeft, 0.0f); glVertex2f(-width / 2, height / 2);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glDisable(GL_TEXTURE_2D); 
    glColor3f(1.0f, 0.0f, 0.0f); 
    glBegin(GL_LINE_LOOP);
        glVertex2f(fireTrapHitbox.x, fireTrapHitbox.y); 
        glVertex2f(fireTrapHitbox.x, fireTrapHitbox.y + fireTrapHitbox.height); 
        glVertex2f(fireTrapHitbox.x + fireTrapHitbox.width, fireTrapHitbox.y + fireTrapHitbox.height); 
        glVertex2f(fireTrapHitbox.x + fireTrapHitbox.width, fireTrapHitbox.y); 
    glEnd();
    glEnable(GL_TEXTURE_2D); 
    glPopMatrix();

}

