// Game Over Screen, players damage over time, and speed the game after 30 seconds

#include <GL/glx.h>
#include <iostream>
#include "src/setup/Global.h"
#include "src/setup/fonts.h"
#include <cmath>
#include "src/setup/health.h"
#include "src/setup/melvir.h"
#include <chrono>
#include <vector>

struct GameObject {
   float positionX, positionY;
   float velocityX, velocityY;
};

struct HealthBoost {
   float positionX, positionY;
   int healthIncreaseAmount;


   HealthBoost(float x, float y, int amount)
       : positionX(x), positionY(y), healthIncreaseAmount(amount) {}
};

std::vector<GameObject> gameObjects;
std::vector<HealthBoost> healthBoosts;
float gameSpeedMultiplier = 1.0f;
float timeElapsed = 0.0f;
float damageTimer = 0.0f;
const float DAMAGE_INTERVAL = 1.0f;
bool gameOver = false;

void render_GameOverScreen() {
   glEnable(GL_TEXTURE_2D);
   glClear(GL_COLOR_BUFFER_BIT);
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_GREATER, 0.0f);
   glColor3f(1.0, 1.0, 1.0);

   // Background
   glBindTexture(GL_TEXTURE_2D, g.tex.MenuTexture);
   glPushMatrix();
   glBegin(GL_QUADS);
   glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]);
   glVertex2i(0, 0);
   glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]);
   glVertex2i(0, g.yres);
   glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]);
   glVertex2i(g.xres, g.yres);
   glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]);
   glVertex2i(g.xres, 0);
   glEnd();

   // Title
   float title_width = g.xres * 0.25;
   float title_height = g.yres * 0.2;
   glBindTexture(GL_TEXTURE_2D, g.tex.GameOverTitleTexture);
   glBegin(GL_QUADS);
   glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]);
   glVertex2i(g.xres / 2 - title_width, g.yres / 1.5 - title_height);
   glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]);
   glVertex2i(g.xres / 2 - title_width, g.yres / 1.5 + title_height);
   glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]);
   glVertex2i(g.xres / 2 + title_width, g.yres / 1.5 + title_height);
   glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]);
   glVertex2i(g.xres / 2 + title_width, g.yres / 1.5 - title_height);
   glEnd();

   // Quit button
   float Qbutton_width = g.xres * 0.05;
   float Qbutton_height = g.yres * 0.1;
   glBindTexture(GL_TEXTURE_2D, g.tex.quitButtonTexture);
   glBegin(GL_QUADS);
   glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]);
   glVertex2i(g.xres / 2 - Qbutton_width, g.yres / 4 - Qbutton_height);
   glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]);
   glVertex2i(g.xres / 2 - Qbutton_width, g.yres / 4 + Qbutton_height);
   glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]);
   glVertex2i(g.xres / 2 + Qbutton_width, g.yres / 4 + Qbutton_height);
   glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]);
   glVertex2i(g.xres / 2 + Qbutton_width, g.yres / 4 - Qbutton_height);
   glEnd();

   // Restart button
   float Rbutton_width = g.xres * 0.05;
   float Rbutton_height = g.yres * 0.1;
   glBindTexture(GL_TEXTURE_2D, g.tex.restartButtonTexture);
   glBegin(GL_QUADS);
   glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]);
   glVertex2i(g.xres / 4 - Rbutton_width, g.yres / 4 - Rbutton_height);
   glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]);
   glVertex2i(g.xres / 4 - Rbutton_width, g.yres / 4 + Rbutton_height);
   glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]);
   glVertex2i(g.xres / 4 + Rbutton_width, g.yres / 4 + Rbutton_height);
   glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]);
   glVertex2i(g.xres / 4 + Rbutton_width, g.yres / 4 - Rbutton_height);
   glEnd();


   glPopMatrix();
}

Health health(100);

std::chrono::steady_clock::time_point lastUpdateTime = std::chrono::steady_clock::now();

void initializeGameObjects() {
   GameObject obj1 = {0.0f, 0.0f, 1.0f, 0.5f};
   GameObject obj2 = {10.0f, 5.0f, -0.5f, 1.0f};
   gameObjects.push_back(obj1);
   gameObjects.push_back(obj2);
}

void initializeHealthBoosts() {
   healthBoosts.push_back(HealthBoost(200.0f, 150.0f, 20)); // Increases health by 20
   healthBoosts.push_back(HealthBoost(400.0f, 300.0f, 30)); // Increases health by 30
}

bool checkCollision(const Player& player, const HealthBoost& boost) {
   float playerX = player.getX();
   float playerY = player.getY();


   return (playerX < boost.positionX + 20 &&
           playerX + 20 > boost.positionX &&
           playerY < boost.positionY + 20 &&
           playerY + 20 > boost.positionY);
}


void updateGameObjects(float deltaTime) {
   auto it = gameObjects.begin();
   while (it != gameObjects.end()) {
       it->positionX += it->velocityX * deltaTime * gameSpeedMultiplier;
       it->positionY += it->velocityY * deltaTime * gameSpeedMultiplier;


       if (it->positionX < 0 || it->positionX > g.xres ||
           it->positionY < 0 || it->positionY > g.yres) {
           it = gameObjects.erase(it);
       } else {
           ++it;
       }
   }
}

void gameLoop() {
    if (gameOver) {
        render_GameOverScreen();
        return;
    }

    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;

    timeElapsed += elapsedTime.count();

    // Damage over time logic
    damageTimer += elapsedTime.count();
    if (damageTimer >= DAMAGE_INTERVAL) {
        // Calculate the damage based on the game speed
        int damageAmount = static_cast<int>(10.0f * gameSpeedMultiplier);
        health.TakeDamage(damageAmount);

        // Check if health drops to zero or below to trigger game over
        if (health.GetCurrentHealth() <= 0) {
            gameOver = true;
        }

        // Reset the timer after applying damage
        damageTimer = 0.0f;
    }

    // Increases the game speed every 30 seconds
    static int lastSpeedIncreaseTime = 0;
    if (static_cast<int>(timeElapsed) / 30 > lastSpeedIncreaseTime) {
        gameSpeedMultiplier = std::min(gameSpeedMultiplier + 0.1f, 3.0f);
        lastSpeedIncreaseTime = static_cast<int>(timeElapsed) / 30;
    }

    // Update game objects with the current time
    updateGameObjects(elapsedTime.count());
}
