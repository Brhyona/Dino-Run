#include "src/setup/imageLoader.h"
#include "src/setup/Global.h"
#include <GL/glx.h>

const float MOVE_SPEED = 5.0f; // Speed of character movement

class Character {
public:
    GLuint texture;
    float x, y;
    int width, height;

    Character(Image* img, float x, float y, int w, int h) 
        : x(x), y(y), width(w), height(h) {
        
        if (!img->data) {
            printf("Failed to load image:");
            return;
        }
        
        printf("Image loaded: Width: %d, Height: %d\n", img->width, img->height);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    }
};

// Function to draw the character
void drawCharacter(const Character& character) {
    glBindTexture(GL_TEXTURE_2D, character.texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex2f(character.x, character.y);
    glTexCoord2f(1.0, 0.0); glVertex2f(character.x + character.width, character.y);
    glTexCoord2f(1.0, 1.0); glVertex2f(character.x + character.width, character.y + character.height);
    glTexCoord2f(0.0, 1.0); glVertex2f(character.x, character.y + character.height);
    glEnd();
}

// Function to cleanup resources
void cleanup(Character& character) {
    glDeleteTextures(1, &character.texture);
}

// Function to handle key events and update the character's position
void handleInput(XEvent& event, Character& character) {
    if (event.type == KeyPress) {
        KeySym key = XLookupKeysym(&event.xkey, 0);
        switch (key) {
            case XK_Left: // Left arrow key
                character.x -= MOVE_SPEED;
                if (character.x < 0) character.x = 0; // Prevent going off-screen
                break;
            case XK_Right: // Right arrow key
                character.x += MOVE_SPEED;
                if (character.x + character.width > WINDOW_WIDTH)
                    character.x = WINDOW_WIDTH - character.width; // Prevent going off-screen
                break;
            case XK_Up: // Up arrow key
                character.y += MOVE_SPEED;
                if (character.y + character.height > WINDOW_HEIGHT)
                    character.y = WINDOW_HEIGHT - character.height; // Prevent going off-screen
                break;
            case XK_Down: // Down arrow key
                character.y -= MOVE_SPEED;
                if (character.y < 0) character.y = 0; // Prevent going off-screen
                break;
            case XK_Escape: // ESC key to quit
                exit(0); // Exit the program
        }
    }
}
