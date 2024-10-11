#include<GL/glx.h>
#include<iostream>
int paused = 0;
int start = 0;

/*
class Menu {
        public:
                Image *backImage;
                GLuint backTexture;
                float xc[2];
                float yc[2];
};
*/
class Global {
        public:
                int xres, yres;
                //Texture tex;
                Global() {
                        xres=640, yres=480;
                }
}bt_g;


void render_menu() {


       
        if (!start){
                std::cout<< "Press the space bar button still under contruction"<<std::endl;
   
                glClear(GL_COLOR_BUFFER_BIT);
                glColor3f(1.0f,0.0f,0.0f);
                glBegin(GL_QUADS);
                glVertex2i(bt_g.xres/2 + 400,bt_g.yres/2 - 250);
                glVertex2i(bt_g.xres/2 + 400,bt_g.yres/2 + 250);
                glVertex2i(bt_g.xres/2 - 400,bt_g.yres/2 + 250);
                glVertex2i(bt_g.xres/2 - 400,bt_g.yres/2 - 250);
                glEnd();
      
/*
                glColor3f(1.0f,0.0f,0.0f);
                glBegin(GL_QUADS);
                glVertex2i(bt_g.xres/3- 60,bt_g.yres/2 - 20);
                glVertex2i(bt_g.xres/3- 60,bt_g.yres/2 + 20);
                glVertex2i(bt_g.xres/3 + 60,bt_g.yres/2 + 20);
                glVertex2i(bt_g.xres/3 + 60,bt_g.yres/2 - 20);
                glEnd();

                glColor3f(1.0f,0.0f,0.0f);
                glVertex2i(bt_g.xres/2- 100,bt_g.yres/2 - 50);
                glVertex2i(bt_g.xres/2- 100,bt_g.yres/2 + 50);
                glVertex2i(bt_g.xres/2 + 100,bt_g.yres/2 + 50);
                glVertex2i(bt_g.xres/2 + 100,bt_g.yres/2 - 50);
                glBegin(GL_LINE_LOOP);
                glPopMatri
                
                               Rect r;
 
                r.bot = bt_g.yres - 20;
                r.left = 100;
                r.center = 0;

                ggprint8b(&r, 30, 0x0000000, "Start") ; */        
        }
        if (paused) {

                glColor3f(1.0, 1.0, 1.0);
                glBegin(GL_QUADS);
                glVertex2i(bt_g.xres/2- 60,bt_g.yres/2 - 20);
                glVertex2i(bt_g.xres/2- 60,bt_g.yres/2 + 20);
                glVertex2i(bt_g.xres/2 + 60,bt_g.yres/2 + 20);
                glVertex2i(bt_g.xres/2 + 60,bt_g.yres/2 - 20);
                glEnd();
        }

}