#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "src/setup/imageLoader.h"
#include "src/setup/Global.h"
#include "src/setup/jpompa.h"
#include "src/setup/melvir.h"

Image img[16]= {
	"src/assets/textures/menu/background.png",
	"src/assets/textures/ground/platform.png",
	"src/assets/textures/ground/coin.png",
	"src/assets/textures/player/avoid.png",
	"src/assets/textures/player/bite.png",
	"src/assets/textures/player/dash.png",
	"src/assets/textures/player/dead.png",
	"src/assets/textures/player/hurt.png",
	"src/assets/textures/player/idle.png",
	"src/assets/textures/player/jump.png",
	"src/assets/textures/player/kick.png",
	"src/assets/textures/player/move.png",
	"src/assets/textures/player/scan.png",
	"src/assets/textures/menu/hover.png",
	"src/assets/textures/menu/title.png",
	"src/assets/textures/menu/shadow.png"
};

Image* playerImages[NUM_STATES] = {
	&img[3],
	&img[4],
	&img[5],
	&img[6],
	&img[7],
	&img[8],
	&img[9],
	&img[10],
	&img[11],
	&img[12],
};

Global g;

Platform platform(&img[1], 320.0f, 100.0f, 641.0f, 231.0f);
Coin coin(&img[2], 100.0f, 200.0f, 32.0f, 32.0f, &platform); 
Player player(playerImages, 320.0f, 100.0f, 64.0f, 64.0f, 100);

float scrollSpeed = 0.0001;
bool start = false;
extern void render_menu();

class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper() {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		setup_screen_res(640, 480);
		dpy = XOpenDisplay(NULL);
		if(dpy == NULL) {
			printf("\n\tcannot connect to X server\n\n");
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if(vi == NULL) {
			printf("\n\tno appropriate visual found\n\n");
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		XSetWindowAttributes swa;
		swa.colormap = cmap;
		swa.event_mask =
			ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
			ButtonPressMask | ButtonReleaseMask |
			StructureNotifyMask | SubstructureNotifyMask;
		win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
								vi->depth, InputOutput, vi->visual,
								CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
	}
	void cleanupXWindows() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void setup_screen_res(const int w, const int h) {
		g.xres = w;
		g.yres = h;
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, g.xres, 0, g.yres, -1, 1);
		set_title();
	}
	void set_title() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "~ Dino-Run! ~");
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
	void check_resize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != g.xres || xce.height != g.yres) {
			//Window size did change.
			reshape_window(xce.width, xce.height);
		}
	}
} x11;

void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics(void);
void render(void);

std::chrono::high_resolution_clock::time_point lastTime;

int main()
{
	init_opengl();
	lastTime = std::chrono::high_resolution_clock::now();
	int done=0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		physics();
		render();
		x11.swapBuffers();
	}
	return 0;
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow texture maps
	glEnable(GL_TEXTURE_2D);
	
	//load the images file into a ppm structure.
	
	//Background
	g.tex.backImage = &img[0];
	glGenTextures(1, &g.tex.backTexture);
	int w = g.tex.backImage->width;
	int h = g.tex.backImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
							GL_RGB, GL_UNSIGNED_BYTE, g.tex.backImage->data);
	g.tex.xc[0] = 0.0;
	g.tex.xc[1] = 0.25;
	g.tex.yc[0] = 0.0;
	g.tex.yc[1] = 1.0;
	g.tex.xc2[0] = 0.0;
	g.tex.xc2[1] = 1.0;

	//Platform
	g.tex.platformImage = &img[1];
	w = g.tex.platformImage->width;
	h = g.tex.platformImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.platformTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	unsigned char *platformData = g.tex.platformImage->buildAlphaData(&img[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, platformData);
	free(platformData);

	//Coin
	g.tex.coinImage = &img[2];
	w = g.tex.coinImage->width;
	h = g.tex.coinImage->height;
	glGenTextures(1, &g.tex.coinTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.coinTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	unsigned char *coinData = g.tex.coinImage->buildAlphaData(&img[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, coinData);
	free(coinData);

	//Player Avoid
	g.tex.playerAvoidImage = &img[3];
	w = g.tex.playerAvoidImage->width;
	h = g.tex.playerAvoidImage->height;
	glGenTextures(1, &g.tex.playerAvoidTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.playerAvoidTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);	
	
	unsigned char *playerAvoidData = g.tex.playerAvoidImage->buildAlphaData(&img[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, playerAvoidData);
	free(playerAvoidData);

	//Player Bite
	g.tex.playerBiteImage = &img[4];
	w = g.tex.playerBiteImage->width;
	h = g.tex.playerBiteImage->height;
	glGenTextures(1, &g.tex.playerBiteTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.playerBiteTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	unsigned char *playerBiteData = g.tex.playerBiteImage->buildAlphaData(&img[4]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, playerBiteData);
	free(playerBiteData);

	//Player Dash
 	g.tex.playerDashImage = &img[5];
	w = g.tex.playerDashImage->width;
	h = g.tex.playerDashImage->height;
	glGenTextures(1, &g.tex.playerDashTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.playerDashTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	unsigned char *playerDashData = g.tex.playerDashImage->buildAlphaData(&img[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, playerDashData);
	free(playerDashData);

	//Player Dead
	g.tex.playerDeadImage = &img[6];
	w = g.tex.playerDeadImage->width;
	h = g.tex.playerDeadImage->height;
	glGenTextures(1, &g.tex.playerDeadTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.playerDeadTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	unsigned char *playerDeadData = g.tex.playerDeadImage->buildAlphaData(&img[6]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, playerDeadData);
	free(playerDeadData);

	//Player Hurt
	g.tex.playerHurtImage = &img[7];
	w = g.tex.playerHurtImage->width;
	h = g.tex.playerHurtImage->height;
	glGenTextures(1, &g.tex.playerHurtTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.playerHurtTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	unsigned char *playerHurtData = g.tex.playerHurtImage->buildAlphaData(&img[7]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, playerHurtData);
	free(playerHurtData);

	//Player Idel
	g.tex.playerIdelImage = &img[8];
	w = g.tex.playerIdelImage->width;
	h = g.tex.playerIdelImage->height;
	glGenTextures(1, &g.tex.playerIdelTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.playerIdelTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	unsigned char *playerIdelData = g.tex.playerIdelImage->buildAlphaData(&img[8]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, playerIdelData);
	free(playerIdelData);

	//Player Jump	
	g.tex.playerJumpImage = &img[9];
	w = g.tex.playerJumpImage->width;
	h = g.tex.playerJumpImage->height;
	glGenTextures(1, &g.tex.playerJumpTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.playerJumpTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	unsigned char *playerJumpData = g.tex.playerJumpImage->buildAlphaData(&img[9]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, playerJumpData);
	free(playerJumpData);

	//Player Kick
	g.tex.playerKickImage = &img[10];
	w = g.tex.playerKickImage->width;
	h = g.tex.playerKickImage->height;
	glGenTextures(1, &g.tex.playerKickTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.playerKickTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	unsigned char *playerKickData = g.tex.playerKickImage->buildAlphaData(&img[10]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, playerKickData);
	free(playerKickData);	

	//Player Move	
	g.tex.playerMoveImage = &img[11];
	w = g.tex.playerMoveImage->width;
	h = g.tex.playerMoveImage->height;
	glGenTextures(1, &g.tex.playerMoveTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.playerMoveTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//std::cout << "Loaded playerMoveTexture ID: " << g.tex.playerMoveTexture << std::endl;

	unsigned char *playerMoveData = g.tex.playerMoveImage->buildAlphaData(&img[11]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, playerMoveData);
	free(playerMoveData);

	//Player Scan	
	g.tex.playerScanImage = &img[12];
	w = g.tex.playerScanImage->width;
	h = g.tex.playerScanImage->height;
	glGenTextures(1, &g.tex.playerScanTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.playerScanTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	unsigned char *playerScanData = g.tex.playerScanImage->buildAlphaData(&img[12]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, playerScanData);
	free(playerScanData);

	player.loadTextures(playerImages);
	
	//play button
	g.tex.playbuttonImage = &img[13];
    glGenTextures(1, &g.tex.playbuttonTexture);
     w = g.tex.playbuttonImage->width;
     h = g.tex.playbuttonImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.playbuttonTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *buttonData = g.tex.playbuttonImage->buildAlphaData(&img[13]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, buttonData);
    free(buttonData);

	 //title
	 g.tex.TitleImage = &img[14];
    glGenTextures(1, &g.tex.TitleTexture);
     w = g.tex.TitleImage->width;
     h = g.tex.TitleImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.TitleTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char * titleData = g.tex.TitleImage->buildAlphaData(&img[14]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, titleData);
    free(titleData);

    //menu background 
	g.tex.MenuImage = &img[15];
	glGenTextures(1, &g.tex.MenuTexture);
	 w = g.tex.MenuImage->width;
	 h = g.tex.MenuImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.MenuTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
							GL_RGB, GL_UNSIGNED_BYTE, g.tex.MenuImage->data);
	
}

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type == ButtonRelease) {
		return;
	}

	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			int mousex = e->xbutton.x;
			int mousey = g.yres - e->xbutton.y;
            int buttonLeft =   g.xres/2 - (g.xres * 0.1);
			int buttonRight =  g.xres/2 + (g.xres * 0.1);
			int buttonBottom = g.yres/4 - (g.yres * 0.15);
			int buttonTop =    g.yres/4 + (g.yres * 0.15);
            if (mousex >=buttonLeft &&
			mousex <= buttonRight &&
			mousey >= buttonBottom &&
			mousey <= buttonTop) {
				start = true;
			}
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
}

int check_keys(XEvent *e)
{
	//Was there input from the keyboard?
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		player.handleInput(key);

		if (key == XK_p) {
			g.paused = !g.paused;
		}
		if (key == XK_Escape) {
			return 1;
		}
		
	}
	return 0;
}

void physics()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = currentTime - lastTime;
	float deltaTime = elapsed.count();
	lastTime = currentTime;
	if(!g.paused)
	{
		g.tex.xc[0] += scrollSpeed;
		g.tex.xc[1] += scrollSpeed;
		platform.updatePlatforms(g.xres, g.yres, deltaTime);
		coin.updateCoins(g.xres, g.yres, deltaTime);
	}
	
}

void render()
{
	
	glEnable(GL_TEXTURE_2D);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	
	float screenAspect = static_cast<float>(g.xres) / g.yres;
	float textureAspect = static_cast<float>(g.tex.backImage->width) / g.tex.backImage->height;
	float widthScale = 1.0f;
	float heightScale = 1.0f;

	if (screenAspect > textureAspect) {
    	widthScale = screenAspect / textureAspect;
	} else {
    	heightScale = textureAspect / screenAspect;
	}
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);	
	glBegin(GL_QUADS);
		glTexCoord2f(g.tex.xc[0], g.tex.yc[1]); glVertex2i(0, 0);
		glTexCoord2f(g.tex.xc[0], g.tex.yc[0]); glVertex2i(0, g.yres * heightScale);
		glTexCoord2f(g.tex.xc[1], g.tex.yc[0]); glVertex2i(g.xres * widthScale, g.yres * heightScale);
		glTexCoord2f(g.tex.xc[1], g.tex.yc[1]); glVertex2i(g.xres, 0);
	glEnd();
	glPopMatrix();
	
	//Transparent platform works now thanks to gordans explanation. 
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, g.tex.platformTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);	
    platform.render();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, g.tex.coinTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
    coin.renderCoins();
	glPopMatrix();

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, player.getTexture());
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
    player.render();
	player.healthMeter();
    glPopMatrix();
	
	if (!start) {
    render_menu();
	}

}
