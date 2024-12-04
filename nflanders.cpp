// Game Over Screen, players damage over time, and speed the game after 30 seconds

#include <GL/glx.h>
#include <iostream>
#include "src/setup/Global.h"
#include "src/setup/health.h"
#include "src/setup/melvir.h"
#include "src/setup/bthomas.h"
#include <chrono>
#include <vector>
#include <algorithm>

extern Global g;
extern bool start;

void render_menu();

Health::Health(int max_hp) : max_health(max_hp), current_health(max_hp) {
    lastDamageTime = std::chrono::steady_clock::now();
}

void Health::TakeDamage(int damage) {
    current_health -= damage;
    if (current_health < 0) {
        current_health = 0;
    }

    if (current_health == 0 && gameOverCallback) {
        gameOverCallback();
    }
}

void Health::Heal(int amount) {
    current_health += amount;
    if (current_health > max_health) {
        current_health = max_health;
    }
}

void Health::ApplyTimeDamage(int damagePerSecond, float timeElapsed) {
    int damage = static_cast<int>(damagePerSecond * timeElapsed);
    TakeDamage(damage);
}

bool Health::IsDead() const {
    return current_health <= 0;
}

int Health::GetCurrentHealth() const {
    return current_health;
}

void Health::SetMaxHealth(int new_max) {
    max_health = new_max;
}

int Health::GetMaxHealth() const {
    return max_health;
}

std::chrono::steady_clock::time_point Health::GetLastDamageTime() const {
    return lastDamageTime;
}

void Health::SetLastDamageTime(std::chrono::steady_clock::time_point time) {
    lastDamageTime = time;
}

void Health::SetGameOverCallback(GameOverCallback callback) {
    gameOverCallback = callback;
}

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
    if (g.isGameOver) {
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
}

Health health(100);

std::chrono::steady_clock::time_point lastUpdateTime = std::chrono::steady_clock::now();

void initializeGameObjects() {
   GameObject obj1 = {0.0f, 0.0f, 1.0f, 0.5f};
   GameObject obj2 = {10.0f, 5.0f, -0.5f, 1.0f};
   gameObjects.push_back(obj1);
   gameObjects.push_back(obj2);

    health.SetGameOverCallback([]() {
       gameOver = true;
       g.isGameOver = true;
   });
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

    if (health.GetCurrentHealth() <= 0) {
        gameOver = true;
        g.isGameOver = true;
        render_GameOverScreen();
        return;
    }

    if (gameOver) {
        render_GameOverScreen();
        return;
    }

    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;

    timeElapsed += elapsedTime.count();

    damageTimer += elapsedTime.count();
    if (damageTimer >= DAMAGE_INTERVAL) {
        int damageAmount = static_cast<int>(10.0f * gameSpeedMultiplier);
        health.TakeDamage(damageAmount);

        damageTimer = 0.0f;
    }

    // Increases speed every 30 seconds
    static int lastSpeedIncreaseTime = 0;
    if (static_cast<int>(timeElapsed) / 30 > lastSpeedIncreaseTime) {
        gameSpeedMultiplier = std::min(gameSpeedMultiplier + 0.1f, 3.0f);
        lastSpeedIncreaseTime = static_cast<int>(timeElapsed) / 30;
    }

    updateGameObjects(elapsedTime.count());
}

bool isMouseOverButton(int mousex, int mousey, int buttonLeft, int buttonRight, int buttonBottom, int buttonTop) {
    return mousex >= buttonLeft && mousex <= buttonRight &&
           mousey >= buttonBottom && mousey <= buttonTop;
}

void check_buttons(int mousex, int mousey) {
    int quitButtonLeft = g.xres / 2 - (g.xres * 0.05);
    int quitButtonRight = g.xres / 2 + (g.xres * 0.05);
    int quitButtonBottom = g.yres / 4 - (g.yres * 0.1);
    int quitButtonTop = g.yres / 4 + (g.yres * 0.1);

    int restartButtonLeft = g.xres / 4 - (g.xres * 0.05);
    int restartButtonRight = g.xres / 4 + (g.xres * 0.05);
    int restartButtonBottom = g.yres / 4 - (g.yres * 0.1);
    int restartButtonTop = g.yres / 4 + (g.yres * 0.1);

    // Quit button is clicked
    if (isMouseOverButton(mousex, mousey, quitButtonLeft, quitButtonRight, quitButtonBottom, quitButtonTop)) {
        exit(0);
    }

    // Return button is clicked
    if (isMouseOverButton(mousex, mousey, restartButtonLeft, restartButtonRight, restartButtonBottom, restartButtonTop)) {
        g.isGameOver = false;
        gameOver = false;

        render_menu();

    }
}

void handleGameOverButtons(int mousex, int mousey) {
    if (g.isGameOver) {
        check_buttons(mousex, mousey);
    }
};
