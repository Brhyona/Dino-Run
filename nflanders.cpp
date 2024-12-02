#include <GL/glx.h>
#include <iostream>
#include "src/setup/Global.h"
#include "src/setup/fonts.h"

void renderGameOverScreen() {
    glEnable(GL_TEXTURE_2D);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor3f(1.0, 1.0, 1.0);
    
    // Background 
    glBindTexture(GL_TEXTURE_2D, g.tex.GameOverTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(0, 0);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(0, g.yres);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres, g.yres);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres, 0);
    glEnd();

    // Game Over Title
    float title_width = g.xres * 0.25;
    float title_height = g.yres * 0.2;
    glBindTexture(GL_TEXTURE_2D, g.tex.GameOverTitleTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres/2 - title_width, g.yres/1.5 - title_height);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres/2 - title_width, g.yres/1.5 + title_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres/2 + title_width, g.yres/1.5 + title_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres/2 + title_width, g.yres/1.5 - title_height); 
    glEnd();

    // Restart Button
    float button_width = g.xres * 0.1;
    float button_height = g.yres * 0.15;
    glBindTexture(GL_TEXTURE_2D, g.tex.restartButtonTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres/2 - button_width, g.yres/4 - button_height);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres/2 - button_width, g.yres/4 + button_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres/2 + button_width, g.yres/4 + button_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres/2 + button_width, g.yres/4 - button_height); 
    glEnd();

    // Quit Button
    glBindTexture(GL_TEXTURE_2D, g.tex.quitButtonTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[1]); glVertex2i(g.xres/2 - button_width, g.yres/4 - 2 * button_height);
    glTexCoord2f(g.tex.xc2[0], g.tex.yc[0]); glVertex2i(g.xres/2 - button_width, g.yres/4 - button_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[0]); glVertex2i(g.xres/2 + button_width, g.yres/4 - button_height);
    glTexCoord2f(g.tex.xc2[1], g.tex.yc[1]); glVertex2i(g.xres/2 + button_width, g.yres/4 - 2 * button_height); 
    glEnd();
}

//Input for game over screen, restart/quit
void handleGameOverInput(unsigned char key, int x, int y) {
    if (key == 'r' || key == 'R') {
        g.coin = 0;  // GOING TO CHANGE
        g.dead = 0; 
        g.paused = 0;
    } else if (key == 'q' || key == 'Q') {
        exit(0); 
    }
}

void gameLoop() {
    if (g.dead) {
        renderGameOverScreen(); 
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void handleKeyPress(unsigned char key, int x, int y) {
    if (g.dead) {
        handleGameOverInput(key, x, y);
    } else if (g.paused) {
        if (key == 'p' || key == 'P') {
            g.paused = 0;
            return;
        }
    } else {
        switch (key) {
            case 'q':
                exit(0);
                break;
        }
    }
}
