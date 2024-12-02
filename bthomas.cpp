// bthomas.cpp

#include<GL/glx.h>
#include<iostream>
#include"src/setup/Global.h"
#include"src/setup/fonts.h"
#include"src/setup/bthomas.h"
#include<cmath>
#include "src/setup/melvir.h"

float button_width = g.xres * 0.1;
float button_height = g.yres * 0.15;
Rect r;
bool start = false;
bool info = false;
extern bool start, info;


void render_menu() 
{  
    if (!start) {		 
        glEnable(GL_TEXTURE_2D);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_ALPHA_TEST);
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, g.tex.MenuTexture);
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
         
        float title_width = g.xres * 0.35;
        float title_height = g.yres * 0.3;
        static float shakeTime = 0.0f;
        shakeTime += 0.02f;
        
        float shakeAmount = 10.0f;
        float Tilt = sin(shakeTime) * shakeAmount;
        
        float centerX = g.xres/2;
        float centerY = g.yres/1.5;

        glBindTexture(GL_TEXTURE_2D, g.tex.TitleTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); 
        glVertex2i(centerX - title_width, 
        centerY - title_height + Tilt);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]);
        glVertex2i(centerX - title_width, 
        centerY + title_height + Tilt);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]);
        glVertex2i(centerX + title_width,
        centerY + title_height - Tilt);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); 
        glVertex2i(centerX + title_width, 
        centerY - title_height - Tilt); 
	    glEnd();
        






        
        float button_width = g.xres * 0.1;
        float button_height = g.yres * 0.15;
        static float floatTime = 0.0f;
        floatTime += 0.03f;
        float floatOffset = sin(floatTime) * 5.0f;
    
        glBindTexture(GL_TEXTURE_2D, g.tex.playbuttonTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]);
        glVertex2i(g.xres/2 - button_width, 
        (g.yres/4 - button_height) + floatOffset);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); 
        glVertex2i(g.xres/2 - button_width, 
        (g.yres/4 + button_height) + floatOffset);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); 
        glVertex2i(g.xres/2 + button_width, 
        (g.yres/4 + button_height) + floatOffset);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); 
        glVertex2i(g.xres/2 + button_width, 
        (g.yres/4 - button_height) + floatOffset); 
	    glEnd();

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_ALPHA_TEST);
    }
}
extern void render_menu();

void render_Pmenu() 
{
    if (g.paused) { 
        glEnable(GL_TEXTURE_2D);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor3f(1.0, 1.0, 1.0);
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
    
        float title_width = g.xres * 0.25;
        float title_height = g.yres * 0.2;
    
        glBindTexture(GL_TEXTURE_2D, g.tex.PTitleTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); 
        glVertex2i(g.xres/2 - title_width, 
                    g.yres/1.5 - title_height);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]);
        glVertex2i(g.xres/2 - title_width, 
                    g.yres/1.5 + title_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]);
        glVertex2i(g.xres/2 + title_width,
                    g.yres/1.5 + title_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]);
        glVertex2i(g.xres/2 + title_width,
                    g.yres/1.5 - title_height); 
	    glEnd();
          
        float button_width = g.xres * 0.05;
        float button_height = g.yres * 0.1;
        static float floatTime = 0.0f;
        floatTime += 0.03f;
        float floatOffset = sin(floatTime) * 5.0f;
       
        glBindTexture(GL_TEXTURE_2D, g.tex.playbuttonMiniTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); 
        glVertex2i(g.xres/2 - button_width, 
                    (g.yres/4 - button_height) + floatOffset);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); 
        glVertex2i(g.xres/2 - button_width, 
                    (g.yres/4 + button_height) + floatOffset);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]);
        glVertex2i(g.xres/2 + button_width, 
                    (g.yres/4 + button_height) + floatOffset);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); 
        glVertex2i(g.xres/2 + button_width, 
                     (g.yres/4 - button_height) + floatOffset); 
	    glEnd();

        float Nbutton_width = g.xres * 0.05;
        float Nbutton_height = g.yres * 0.1;

        glBindTexture(GL_TEXTURE_2D, g.tex.nextbuttonTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); 
        glVertex2i(g.xres/4 - Nbutton_width, 
                    (g.yres/4 - Nbutton_height) + floatOffset);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]);
        glVertex2i(g.xres/4 - Nbutton_width, 
                    (g.yres/4 + Nbutton_height) + floatOffset);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]);
        glVertex2i(g.xres/4 + Nbutton_width,
                    (g.yres/4 + Nbutton_height) + floatOffset);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]);
        glVertex2i(g.xres/4 + Nbutton_width,
                    (g.yres/4 - Nbutton_height) + floatOffset); 
	    glEnd();
        glPopMatrix();
    }
}
extern void render_Pmenu();

void render_controlInfo()
{
    if (info) {
        glEnable(GL_TEXTURE_2D);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, g.tex.MenuTexture);
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
 
        float info_back_width = g.xres * 0.45;
        float info_back_height = g.yres * 0.45;
        float info_back_Bheight = g.yres * 0.25;

        glBindTexture(GL_TEXTURE_2D, g.tex.info_backTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); 
        glVertex2i(g.xres/2 - info_back_width,
                    g.yres/2 - info_back_Bheight);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); 
        glVertex2i(g.xres/2 - info_back_width, 
                    g.yres/2 + info_back_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); 
        glVertex2i(g.xres/2 + info_back_width, 
                    g.yres/2 + info_back_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); 
        glVertex2i(g.xres/2 + info_back_width, 
                    g.yres/2 - info_back_Bheight);
	    glEnd();

        float Nbutton_width = g.xres * 0.05;
        float Nbutton_height = g.yres * 0.1;
        static float floatTime = 0.0f;
        floatTime += 0.03f;
        float floatOffset = sin(floatTime) * 5.0f;

        glBindTexture(GL_TEXTURE_2D, g.tex.nextbuttonTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); 
        glVertex2i(g.xres*3/4 + Nbutton_width, 
                    (g.yres/8 - Nbutton_height) + floatOffset);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); 
        glVertex2i(g.xres*3/4 + Nbutton_width,
                    (g.yres/8 + Nbutton_height) + floatOffset);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); 
        glVertex2i(g.xres*3/4 - Nbutton_width, 
                    (g.yres/8 + Nbutton_height) + floatOffset);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); 
        glVertex2i(g.xres*3/4 - Nbutton_width, 
                    (g.yres/8 - Nbutton_height) + floatOffset); 
	    glEnd();

        float Space_width = g.xres * 0.05;
        float Space_height = g.yres * 0.05;

        glBindTexture(GL_TEXTURE_2D, g.tex.esc_keyTexture);
        glPushMatrix();
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); 
        glVertex2i(g.xres*.6 - Space_width,
                    g.yres*5/7 - Space_height);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); 
        glVertex2i(g.xres*.6 - Space_width,
                    g.yres*5/7 + Space_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); 
        glVertex2i(g.xres*.6 + Space_width,
                    g.yres*5/7 + Space_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); 
        glVertex2i(g.xres*.6 + Space_width,
                    g.yres*5/7 - Space_height); 
	    glEnd();
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, g.tex.arrow_keyTexture);
        glPushMatrix();
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); 
        glVertex2i(g.xres*0.2 - Space_width, 
                    g.yres*5/7 - Space_height);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]);
        glVertex2i(g.xres*0.2 - Space_width,
                    g.yres*5/7 + Space_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); 
        glVertex2i(g.xres*0.2 + Space_width,
                    g.yres*5/7 + Space_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]);
        glVertex2i(g.xres*0.2 + Space_width,
                    g.yres*5/7 - Space_height); 
	    glEnd();
        glPopMatrix();
       
        float SpaceB_width = g.xres * 0.1;
        float SpaceB_height = g.yres * 0.05;
        
        glBindTexture(GL_TEXTURE_2D, g.tex.space_barTexture);
        glPushMatrix();
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); 
        glVertex2i(g.xres*0.2 - SpaceB_width, 
                    g.yres*4/7 - SpaceB_height);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); 
        glVertex2i(g.xres*0.2 - SpaceB_width, 
                    g.yres*4/7 + SpaceB_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); 
        glVertex2i(g.xres*0.2 + SpaceB_width,
                    g.yres*4/7 + SpaceB_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); 
        glVertex2i(g.xres*0.2 + SpaceB_width,
                    g.yres*4/7 - SpaceB_height); 
	    glEnd();
        glPopMatrix();
    
        glBindTexture(GL_TEXTURE_2D, g.tex.i_keyTexture);
        glPushMatrix();
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); 
        glVertex2i(g.xres*0.6 - Space_width,
                    g.yres*4/7 - Space_height);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]);
        glVertex2i(g.xres*0.6 - Space_width,
                    g.yres*4/7 + Space_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]);
        glVertex2i(g.xres*0.6 + Space_width,
                    g.yres*4/7 + Space_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); 
        glVertex2i(g.xres*0.6 + Space_width,
                    g.yres*4/7 - Space_height); 
	    glEnd();
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, g.tex.p_keyTexture);
        glPushMatrix();
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); 
        glVertex2i(g.xres*0.2 - Space_width, 
                    g.yres*3/7 - Space_height);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); 
        glVertex2i(g.xres*0.2 - Space_width, 
                    g.yres*3/7 + Space_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); 
        glVertex2i(g.xres*0.2 + Space_width,
                    g.yres*3/7 + Space_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); 
        glVertex2i(g.xres*0.2 + Space_width, 
                    g.yres*3/7 - Space_height); 
	    glEnd();
    
        float control_width = g.xres * 0.4;
        float control_height = g.yres * 0.15;
        
        glBindTexture(GL_TEXTURE_2D, g.tex.control_MTexture);
        glPushMatrix();
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); 
        glVertex2i(g.xres*0.5 - control_width, 
                    g.yres*6.5/8 -  control_height);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]);
        glVertex2i(g.xres*0.5 - control_width, 
                    g.yres*6.5/8 +  control_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]);
        glVertex2i(g.xres*0.5 + control_width, 
                    g.yres*6.5/8 +  control_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]);
        glVertex2i(g.xres*0.5 + control_width, 
                    g.yres*6.5/8 -  control_height); 
	    glEnd();

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_ALPHA_TEST);

        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_TEXTURE_2D);
        initialize_fonts();
    
        unsigned int c = 0x00FFFFFF;
	    r.bot = g.yres*4.5/7 + Space_height;
	    r.left = g.xres*0.28 + Space_width;
	    r.center = 0;
	    ggprint10(&r, 16, c, "Move the player ");
        r.bot = g.yres*3.5/7 + Space_height;
        ggprint10(&r, 16, c, "Throw 'something' ");
        r.bot = g.yres*2.5/7 + Space_height;
        ggprint10(&r, 16, c, "Press 'p' to pause ");
        r.bot = g.yres*4.5/7 + Space_height;
	    r.left = g.xres*0.65 + Space_width;
	    ggprint10(&r, 16, c, "Close the game");
        r.bot = g.yres*3.5/7 + Space_height;
        ggprint10(&r, 16, c, "Press 'i' to see controls");
    }
}
extern void render_controlInfo();


void check_button(ButtonType type, 
                int mousex, int mousey, Bat& bat) 
{
    Button button(0.1f, 0.1f);
    switch(type) {
        case START:
        button = Button(0.1f, 0.15f);
        if (button.isClicked(mousex, mousey)) {
            start = true;
            bat.resetTimer();
        }
        break;
         
        case PAUSE:
        button = Button(0.1f, 0.15f);
        if (button.isClicked(mousex, mousey)) {
            g.paused = !g.paused;
        }
        break;
         
        case INFO:
        button = Button(0.05f, 0.1f, 0.25f, 0.25f);
        if (button.isClicked(mousex, mousey)) {
            info = !info;
        }
        break;

        case CLOSE_INFO:
        button = Button(0.05f, 0.1f, 0.75f, 0.125f);
        if (button.isClicked(mousex, mousey)) {
            info = !info;
        }
        break;
    }
}
extern void check_button(ButtonType type, 
                        int mousex, int mousey,Bat& bat);
Bat::Bat(Image* img, float startx, float starty, float w, float h, Player& p)
        :player(p),
        texture(0),
        currentState(BAT_FLYING), 
        x(startx), 
        y(starty), 
        width(w),
        height(h),  
        moveSpeed(1.0f), 
        facingLeft(false),
        currentFrame(0),
        totalFrames(14),
        frameDuration(0.07f),
        isSpawned(false),
        isFollowing(true),
        lifeTimer(0.0f) {
    startTime = std::chrono::steady_clock::now();
    texture = g.tex.BatTexture;
    glGenTextures(1,&texture);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, img->width, 
                img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 }

void Bat::updateTimer(float deltaTime) 
{
    if (!IsActive()) {
        return;
    } 
    lifeTimer += deltaTime;
    if (lifeTimer >= DESPAWN_TIME) {
        reset();
    }
}

bool Bat::IsActive() 
{
    auto currentTime = std::chrono::steady_clock::now();
    float pastSecs = std::chrono::duration<float>
                    (currentTime - startTime).count();
    return pastSecs >= SPAWN_DELAY;
}

Bat::~Bat() 
{  
    fireballs.clear();
    if (texture) {
        glDeleteTextures(1,&texture);
    }
}

void Bat::updateAnimation() 
{
    timers.recordTime(&timers.timeCurrent);
    double timeSpan = timers.timeDiff(&animationTimer, 
                                    &timers.timeCurrent);

    if (timeSpan > 0.15f) {
        currentFrame = (currentFrame + 1) % 3;
        timers.recordTime(&animationTimer);
    }    
}

void Bat::updateSwooping(float deltaTime) 
{
    if (!isFollowing || !IsActive()) {
        return;
    } 
    swoopTimer += deltaTime;
    if (!isSwooping && swoopTimer >= SWOOP_INTERVAL) {
        float dx = player.getX() - x;
        if(abs(dx) > 50.0f && y > player.getY()) {
            isSwooping = true;
            swoopTimer = 0.0f;
            swoopAngle = 0.0f;
            originalX = x;
            originalY = y;
            targetSwoopX = player.getX();
            targetSwoopY = player.getY();
        }
    }
    if (isSwooping) {
        performSwoop(deltaTime);
    }
}

void Bat::performSwoop(float deltaTime) 
{ 
    swoopAngle += SWOOP_SPEED * deltaTime;

    if (swoopAngle >= 2 * 3.14) {
        isSwooping = false;
        swoopAngle = 0.0f;
        return;
    }
   
    float targetX = player.getX();
    float targetY = player.getY();
    float progress = sin(swoopAngle * 0.5f);

    x = originalX + (targetX - originalX) * progress;
    y = originalY + (targetY - originalY) * progress;
    y -= (sin(progress * 3.14) * 200.0f);

    updateHitbox();
    const Hitbox& playerBox = player.getHitbox();

    if (hitbox.x < playerBox.x + playerBox.width &&
        hitbox.x + hitbox.width > playerBox.x &&
        hitbox.y < playerBox.y + playerBox.height &&
        hitbox.y + hitbox.height > playerBox.y) {

        player.playerHealth.TakeDamage(5);
        isSwooping = false;
    }
}

void Bat::followPlayer() {

    if (isSwooping) {
        return;
    }

    float playerX = player.getX();
    float playerY = player.getY();
    float lagDistance = 100.0f;
    float targetX = playerX;
    
    if (playerX > x) {
    targetX = playerX - lagDistance;
    } else {
    targetX = playerX + lagDistance;
    }

    float verticalOffset = 200.0f;
    float dx = targetX - x;
    float dy = (playerY + verticalOffset) - y;
    float distance = sqrt(dx*dx + dy*dy);

    if (distance > 0) {
        dx /= distance;
        dy /= distance;
    }

    x += dx * moveSpeed;
    y += dy * moveSpeed;
    facingLeft = dx < 0;

    const float MIN_DISTANCE = 50.0f;
    if (distance < MIN_DISTANCE) {
    x -= dx * moveSpeed;
    y -= dy * moveSpeed;
    }
}

void Bat::patrolMovement() 
{
    if (isSwooping) {
        return;
    }
    x += moveDirection * PATROL_SPEED;
    y = PATROL_HEIGHT;

    if (x >= g.xres - SCREEN_MARGIN) {
        moveDirection = -1.0f;
        facingLeft = true;
    } else if (x <= SCREEN_MARGIN) {
        moveDirection = 1.0f;
        facingLeft = false;
    }
}

void Bat::move() 
{
    if (!IsActive()) {
        return;
    } 

    if (isFollowing) {
        followPlayer();
    } else {
        patrolMovement();
    }
}

void Bat::render() 
{
    if (!IsActive()) {
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    updateAnimation();
    move();

    float frameWidth =  1.0f / 15.0f;
    float tx1 = currentFrame * frameWidth;
    float tx2 = tx1 + frameWidth;

    if (facingLeft) {
    std::swap(tx1,tx2);
    }

    glPushMatrix();
    glTranslatef(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, g.tex.BatTexture);
    glColor4f(1.0f ,1.0f ,1.0f , 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(tx1, 1.0f);
        glVertex2f(-width*3/ 2 , -height*3/ 2);
        glTexCoord2f(tx2, 1.0f); 
        glVertex2f(width*3/ 2 , -height*3/ 2);
        glTexCoord2f(tx2, 0.0f); 
        glVertex2f(width*3/ 2 , height*3/ 2);
        glTexCoord2f(tx1, 0.0f); 
        glVertex2f(-width*3/ 2 , height*3/ 2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
}

GLuint Bat::getTexture() const 
{
    return texture;
}

void Bat:: setPosition(float newX, float newY) 
{
    x = newX;
    y = newY;
}

float Bat::getX() const
{
    return x;
}

float Bat::getY() const
{
    return y;
}


Fireball::Fireball(float startX, float startY, float iniX, float iniY)
        :x(startX),
        y(startY),
        velocityX(iniX),
        velocityY(iniY),
        width(32.0f),
        height(32.0f),
        gravity(-0.01f),
        active(true),
        currentFrame(0),
        animTimer(0.0f),
        texture(0) {
}

Fireball::~Fireball() 
{
    if (texture) {
        glDeleteTextures(1, &texture);
        texture = 0;
    }
}

 void Fireball::update() 
 {
    if (!active) {   
        return;
    }

    x += velocityX;
    y += velocityY;
    velocityY += gravity;

    animTimer += 0.1f;
    if (animTimer >= FRAME_DURATION) {
        currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
        animTimer = 0;
    }
    if (x < 0 || x > g.xres || y < 0 || y > g.yres) {
        active = false;
    }
}
    
void Fireball::render_fire() 
{
    if (!active) {
        return;
    }
    GLboolean texEnable = glIsEnabled(GL_TEXTURE_2D);
    GLboolean blendEnable = glIsEnabled(GL_BLEND);
    GLboolean alphaEnable = glIsEnabled(GL_ALPHA_TEST);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glEnable(GL_BLEND);

    float frameWidth = 1.0f/TOTAL_FRAMES;
    float tx1 = currentFrame * frameWidth;
    float tx2 = tx1 + frameWidth;
    glBindTexture(GL_TEXTURE_2D, g.tex.fireTexture);
    glPushMatrix();
   
    glTranslatef(x, y, 0);
    glRotatef(270.0f, 0.0f, 0.0f, 1.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    glBegin(GL_QUADS);
        glTexCoord2f(tx1, 1.0f);
        glVertex2f(-width/ 2 , -height/ 2);
        glTexCoord2f(tx2, 1.0f); 
        glVertex2f(width/ 2 , -height/ 2);
        glTexCoord2f(tx2, 0.0f); 
        glVertex2f(width/ 2 , height/ 2);
        glTexCoord2f(tx1, 0.0f); 
        glVertex2f(-width/ 2 , height/ 2);
    glEnd();
    glPopMatrix();

    if (!texEnable) glDisable(GL_BLEND);
    if (!blendEnable) glDisable(GL_ALPHA_TEST);
    if (!alphaEnable) glDisable(GL_TEXTURE_2D);
}
double timespec_to_double(struct timespec *ts) 
{
    return (double)ts->tv_sec + (double)ts->tv_nsec * 1.0e-9;
}

void Bat::cleanupFire() 
{
    std::vector<Fireball> activeFireballs;

    for (const Fireball& f: fireballs) {
        if (f.isActive()) {
        activeFireballs.push_back(f);
        }
    }
    fireballs = activeFireballs;
}

void Bat::shootFireball() 
{
    if (!IsActive() || isSwooping) {
        return;
    }

    static struct timespec lastFireTime;
    struct timespec currentTime;
    clock_gettime(CLOCK_REALTIME, &currentTime);
    double timeDiff = timespec_to_double(&currentTime)
                     - timespec_to_double(&lastFireTime);

    if (timeDiff < 3.0f) {
    return;
    }

    if (fireballs.size() < MAX_FIREBALLS) {      
            float dx = player.getX() - x;
            float dy = player.getY() - y;
            float distance = sqrt(dx*dx + dy*dy);

        if (distance > 0) {
            dx /= distance;
            dy /= distance;

            Fireball newFireball(x, y, dx, dy);
            newFireball.hasDamaged = false;
            fireballs.push_back(newFireball);
            lastFireTime = currentTime;
        }
    }
}

void Bat::resetTimer() 
{
    startTime = std::chrono::steady_clock::now();
    isSpawned = false;
    fireballs.clear();
}

bool Fireball::checkCollision(const Player& player) const 
{
    if (!active || hasDamaged) {
        return false;
    }
    float circleX = x;
    float circleY = y;
    float radius = getCollisionRadius();

    const Hitbox& playerBox = player.getPlayerHitbox();
    float closestX, closestY;

    findContact(playerBox, circleX, circleY, closestX, closestY);

    float deltaX = circleX - closestX;
    float deltaY = circleY - closestY;
    float distanceSqrt = (deltaX * deltaX) + (deltaY * deltaY);
    return distanceSqrt <= (radius * radius);
}

void Bat::checkFireCollision(Player& player) 
{
    for (auto& fireball : fireballs) {
        if (fireball.isActive() && !fireball.hasDamaged && 
            fireball.checkCollision(player)) {
            player.playerHealth.TakeDamage(Fireball::DAMAGE_AMOUNT);
            fireball.hasDamaged = true;
            fireball.active = false;
        }    
    }
}

void Bat::reset() 
{
    lifeTimer = 0.0f;
    isSpawned = false;
    startTime = std::chrono::steady_clock::now();
    fireballs.clear();
    firstSpawn = false;
    toggleMovement();

    if (isFollowing) {
        x = g.xres/2;
        y = g.yres - 100;
    } else {
        x = SCREEN_MARGIN;
        y = PATROL_HEIGHT;
        moveDirection = 1.0f;
    }
    facingLeft = false;
}

void Bat::toggleMovement() 
{
    isFollowing = !isFollowing;
    if (!isFollowing) {
        x = SCREEN_MARGIN;
        y = PATROL_HEIGHT;
        moveDirection = 1.0f;
        facingLeft = false;
    }
}
