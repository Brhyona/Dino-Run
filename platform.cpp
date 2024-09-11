#include "imageLoader.h"
#include <GL/glx.h>

class Platform {
public:
    GLuint texture;
    float x, y, width, height;

    Platform(const char *filename, float xpos, float ypos, float w, float h)
        : x(xpos), y(ypos), width(w), height(h) {
            Image img(filename);

        if (!img.data) {
            printf("Failed to load image: %s\n", filename);
        return;
        }
        
        printf("Image loaded: %s, Width: %d, Height: %d\n", filename, img.width, img.height);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void render() {
        glPushMatrix();
        glTranslatef(x, y, 0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(-width / 2, -height / 2);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-width / 2, height / 2);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(width / 2, height / 2);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(width / 2, -height / 2);
        glEnd();
        glPopMatrix();
    }
};