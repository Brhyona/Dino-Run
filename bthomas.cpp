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



void render_menu() {  		 
         glEnable(GL_TEXTURE_2D);
          glClear(GL_COLOR_BUFFER_BIT);
          
          glEnable(GL_ALPHA_TEST);
          glAlphaFunc(GL_GREATER, 0.0f);
          glColor3f(1.0, 1.0, 1.0);
          
          glBindTexture(GL_TEXTURE_2D, g.tex.MenuTexture);
          glBegin(GL_QUADS);
          glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(0, 0);
          glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(0, g.yres);
          glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres, g.yres);
          glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres, 0);
	  glEnd();
         float title_width = g.xres * 0.35;
         float title_height = g.yres * 0.3;
          glBindTexture(GL_TEXTURE_2D, g.tex.TitleTexture);
          glBegin(GL_QUADS);
          glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres/2 - title_width, g.yres/1.5 - title_height);
          glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres/2 - title_width, g.yres/1.5 + title_height);
          glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres/2 + title_width, g.yres/1.5 + title_height);
          glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres/2 + title_width, g.yres/1.5 - title_height); 
	  glEnd();
          float button_width = g.xres * 0.1;
          float button_height = g.yres * 0.15;
         glBindTexture(GL_TEXTURE_2D, g.tex.playbuttonTexture);
          glBegin(GL_QUADS);
          glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres/2 - button_width, g.yres/4 - button_height);
          glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres/2 - button_width, g.yres/4 + button_height);
          glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres/2 + button_width, g.yres/4 + button_height);
          glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres/2 + button_width, g.yres/4 - button_height); 
	 glEnd();
  
    
}
extern void render_menu();

void render_Pmenu() {

    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    
    glBindTexture(GL_TEXTURE_2D, g.tex.MenuTexture);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(0, 0);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(0, g.yres);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres, g.yres);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres, 0);
	glEnd();
    
        float title_width = g.xres * 0.25;
        float title_height = g.yres * 0.2;
    
    glBindTexture(GL_TEXTURE_2D, g.tex.PTitleTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres/2 - title_width, g.yres/1.5 - title_height);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres/2 - title_width, g.yres/1.5 + title_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres/2 + title_width, g.yres/1.5 + title_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres/2 + title_width, g.yres/1.5 - title_height); 
	glEnd();
          float button_width = g.xres * 0.05;
          float button_height = g.yres * 0.1;
         glBindTexture(GL_TEXTURE_2D, g.tex.playbuttonMiniTexture);
        glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres/2 - button_width, g.yres/4 - button_height);
        glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres/2 - button_width, g.yres/4 + button_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres/2 + button_width, g.yres/4 + button_height);
        glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres/2 + button_width, g.yres/4 - button_height); 
	 glEnd();
      float Nbutton_width = g.xres * 0.05;
    float Nbutton_height = g.yres * 0.1;

    glBindTexture(GL_TEXTURE_2D, g.tex.nextbuttonTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres/4 - Nbutton_width, g.yres/4 - Nbutton_height);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres/4 - Nbutton_width, g.yres/4 + Nbutton_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres/4 + Nbutton_width, g.yres/4 + Nbutton_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres/4 + Nbutton_width, g.yres/4 - Nbutton_height); 
	glEnd();
     glPopMatrix();
}

extern void render_Pmenu();

void render_controlInfo(){

    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    
    glBindTexture(GL_TEXTURE_2D, g.tex.MenuTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(0, 0);      
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(0, g.yres); 
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres, g.yres);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres, 0);
	glEnd();
 
    float info_back_width = g.xres * 0.45;
    float info_back_height = g.yres * 0.45;
    float info_back_Bheight = g.yres * 0.25;
    glBindTexture(GL_TEXTURE_2D, g.tex.info_backTexture);
          glBegin(GL_QUADS);
          glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres/2 - info_back_width, g.yres/2 - info_back_Bheight);
          glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres/2 - info_back_width, g.yres/2 + info_back_height);
          glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres/2 + info_back_width, g.yres/2 + info_back_height);
          glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres/2 + info_back_width, g.yres/2 - info_back_Bheight);
	  glEnd();

    float Nbutton_width = g.xres * 0.05;
    float Nbutton_height = g.yres * 0.1;
    glBindTexture(GL_TEXTURE_2D, g.tex.nextbuttonTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres*3/4 + Nbutton_width, g.yres/8 - Nbutton_height);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres*3/4 + Nbutton_width, g.yres/8 + Nbutton_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres*3/4 - Nbutton_width, g.yres/8 + Nbutton_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres*3/4 - Nbutton_width, g.yres/8 - Nbutton_height); 
	glEnd();

    float Space_width = g.xres * 0.05;
    float Space_height = g.yres * 0.05;
    glBindTexture(GL_TEXTURE_2D, g.tex.esc_keyTexture);
   glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres*.6 - Space_width, g.yres*5/7 - Space_height);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres*.6 - Space_width, g.yres*5/7 + Space_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres*.6 + Space_width, g.yres*5/7 + Space_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres*.6 + Space_width, g.yres*5/7 - Space_height); 
	glEnd();
    glPopMatrix();
    
    
    glBindTexture(GL_TEXTURE_2D, g.tex.arrow_keyTexture);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres*0.2 - Space_width, g.yres*5/7 - Space_height);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres*0.2 - Space_width, g.yres*5/7 + Space_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres*0.2 + Space_width, g.yres*5/7 + Space_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres*0.2 + Space_width, g.yres*5/7 - Space_height); 
	glEnd();
    glPopMatrix();
       
    float SpaceB_width = g.xres * 0.1;
    float SpaceB_height = g.yres * 0.05;
    glBindTexture(GL_TEXTURE_2D, g.tex.space_barTexture);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres*0.2 - SpaceB_width, g.yres*4/7 - SpaceB_height);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres*0.2 - SpaceB_width, g.yres*4/7 + SpaceB_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres*0.2 + SpaceB_width, g.yres*4/7 + SpaceB_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres*0.2 + SpaceB_width, g.yres*4/7 - SpaceB_height); 
	glEnd();
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D, g.tex.i_keyTexture);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres*0.6 - Space_width, g.yres*4/7 - Space_height);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres*0.6 - Space_width, g.yres*4/7 + Space_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres*0.6 + Space_width, g.yres*4/7 + Space_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres*0.6 + Space_width, g.yres*4/7 - Space_height); 
	glEnd();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, g.tex.p_keyTexture);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres*0.2 - Space_width, g.yres*3/7 - Space_height);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres*0.2 - Space_width, g.yres*3/7 + Space_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres*0.2 + Space_width, g.yres*3/7 + Space_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres*0.2 + Space_width, g.yres*3/7 - Space_height); 
	glEnd();
    
     float control_width = g.xres * 0.4;
    float control_height = g.yres * 0.15;
    glBindTexture(GL_TEXTURE_2D, g.tex.control_MTexture);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres*0.5 - control_width, g.yres*6.5/8 -  control_height);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres*0.5 - control_width, g.yres*6.5/8 +  control_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres*0.5 + control_width, g.yres*6.5/8 +  control_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres*0.5 + control_width, g.yres*6.5/8 -  control_height); 
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
    //glEnable(GL_TEXTURE_2D);
    //glEnable(GL_ALPHA_TEST);
}
extern void render_controlInfo();


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
frameDuration(0.07f)
 {
    texture = g.tex.BatTexture;
    glGenTextures(1,&texture);
     glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, img->width, 
                  img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //timers.recordTime(&animationTimer);
 }

 Bat::~Bat() {
    glDeleteTextures(1,&texture);
 }

 /*void Bat::loadTexture(Image* img) {

    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,g.tex.BatTexture);
    unsigned char* BatData = img->buildAlphaData(&img[13]);
    
    int size = img->width * img->height * 4;
    for(int i =3; i < size; i += 4) {
        if(BatData[i] > 0) {
        BatData[i] = 255;
        }
    }

   
free(BatData);
}*/

void Bat::updateAnimation() {
    //static float frameDelay = 0.5f;
    timers.recordTime(&timers.timeCurrent);
    double timeSpan = timers.timeDiff(&animationTimer, &timers.timeCurrent);

    if(timeSpan > 0.15f) {
        currentFrame = (currentFrame + 1) % 3;
        timers.recordTime(&animationTimer);
        }
        
}

void Bat::move() {
   
float playerX = player.getX();
float playerY = player.getY();

float dx = playerX - x;
float dy = playerY - y;

float distance = sqrt(dx*dx + dy*dy);
if (distance > 0) {
dx /= distance;
dy /= distance;
}
float verticalOffset = 100.0f;
if (y < playerY + verticalOffset) {
y += moveSpeed;
}

x += dx * moveSpeed;
y += dy * moveSpeed;
facingLeft = dx < 0;

const float MIN_DISTANCE = 50.0f;
if(distance < MIN_DISTANCE) {
x -= dx * moveSpeed;
y -= dy * moveSpeed;
}
}
/*    float newX = x + cos(time) *moveSpeed;
    float newY = y + sin(time* 2.0f) * moveSpeed * 0.5f;

    if(newX < x) facingLeft = true;
    if(newX > x) facingLeft = false;


    y = newY;
    x = newX;
}
  */      
void Bat::render() {
glEnable(GL_TEXTURE_2D);
glEnable(GL_ALPHA_TEST);
glEnable(GL_BLEND);
updateAnimation();
move();
float frameWidth =  1.0f / 15.0f;
float tx1 = currentFrame * frameWidth;
float tx2 = tx1 + frameWidth;

if(facingLeft) {
    std::swap(tx1,tx2);
}

glPushMatrix();
glTranslatef(x, y, 0);
glBindTexture(GL_TEXTURE_2D, g.tex.BatTexture);
glColor4f(1.0f ,1.0f ,1.0f , 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(tx1, 1.0f); glVertex2f(-width*3/ 2 , -height*3/ 2);
        glTexCoord2f(tx2, 1.0f); glVertex2f(width*3/ 2 , -height*3/ 2);
        glTexCoord2f(tx2, 0.0f); glVertex2f(width*3/ 2 , height*3/ 2);
        glTexCoord2f(tx1, 0.0f); glVertex2f(-width*3/ 2 , height*3/ 2);
    glEnd();
    glPopMatrix();

    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_TEXTURE);
 
}

GLuint Bat::getTexture() const {

return texture;
}

void Bat:: setPosition(float newX, float newY) {

    x = newX;
    y = newY;

}
float Bat::getX() const{
    return x;
}

float Bat::getY() const{
    return y;
}
        
