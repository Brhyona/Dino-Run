#include<GL/glx.h>
#include<iostream>
#include"src/setup/Global.h"
#include"src/setup/fonts.h"



float button_width = g.xres * 0.1;
float button_height = g.yres * 0.15;

void render_menu() {  		 
         glEnable(GL_TEXTURE_2D);
          glClear(GL_COLOR_BUFFER_BIT);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
