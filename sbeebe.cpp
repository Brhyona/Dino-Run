/*
 * AUTHOR: SKYE BEEBE
 *
 * DATE: October 2024
*/
#include "src/setup/sbeebe.h"
#include "src/setup/imageLoader.h"
#include "src/setup/melvir.h"
#include "src/setup/Global.h"
#include <chrono>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include <cstdio>
#include <GL/glx.h>

using namespace std;

Evil::Evil(Image* img, float sx, float sy, 
                               float sw, float sh, Player& p)
    : player(p), width(sw), height(sh), 
    evilcurrentFrame(E_RUN), animationTimer(0.0f) {
    texture = g.tex.enemyRunTexture;
    glGenTextures(1, &texture);
    
    /*
    unsigned char* rgbaData = img[i]->buildAlphaData(img[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img[i]->width, 
               img[i]->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
               rgbaData);
        free(rgbaData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 
                                            GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 
                                           GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
                                                 GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                                   GL_LINEAR);
  */
    
}

Evil::~Evil() {
   //if (texture) {
   // glDeleteTextures(1, texture);
   // }
}

void Evil::follow() {
    /*float playerX = player.getX();
    //float playerY = player.getY();
    float followbehind = 100.0f;
    float temp;

    if (playerX > x) {
        temp = playerX - followbehind;
    } else {
        temp = playerX + followbehind;
    }

    float sx = temp - x;
    float sy = playerY - y;
    float dist = sqrt(sx*sx + sy*sy);

    if (dist > 0) {
        sx /= dist;
        sy /= dist;
    }
    */

}

void Evil::getTexture() const {
    //return texture;
}

void Evil::render() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);

    // Compute texture coordinates
    float frameWidth = 1.0f / 15.0f;
    float tx1 = evilcurrentFrame * frameWidth;
    float tx2 = tx1 + frameWidth;
    
    // Render the current frame
    glPushMatrix();
    glTranslatef(x, y, 0);
    

    glBegin(GL_QUADS);
        glTexCoord2f(tx1, 1.0f); 
        glVertex2f(-width / 2, -height / 2); // Bottom-left
        glTexCoord2f(tx2, 1.0f);
        glVertex2f(width / 2, -height / 2);  // Bottom-right
        glTexCoord2f(tx2, 0.0f);
        glVertex2f(width / 2, height / 2);   // Top-right
        glTexCoord2f(tx1, 0.0f); 
        glVertex2f(-width / 2, height / 2);  // Top-left
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

//float Evil::getX() const {
//    return x;
//}

//float Evil::getY() const {
//    return y;
//}
