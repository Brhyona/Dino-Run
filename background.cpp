// background.cpp

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
#include "src/setup/fonts.h"
#include "src/setup/imageLoader.h"
#include "src/setup/Global.h"
#include "src/setup/jpompa.h"
#include "src/setup/melvir.h"
#include "src/setup/bthomas.h"
#include "src/setup/health.h"
#include <vector>


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
	"src/assets/textures/player/hatch.png",
	"src/assets/textures/enemies/bat.png",
	"src/assets/textures/enemies/fire_ball.png"
	
};

Image Mimg[16] {
	"src/assets/textures/menu/pausedmenu.png",
	"src/assets/textures/menu/playbuttonMini.png",
	"src/assets/textures/menu/Hover.png",
	"src/assets/textures/menu/Title.png",
	"src/assets/textures/menu/Shadow.png",
	"src/assets/textures/menu/nextbutton.png",
	"src/assets/textures/menu/info_background.png",
	"src/assets/textures/menu/keys/esc_key.png",
	"src/assets/textures/menu/keys/arrow_key.png",
	"src/assets/textures/menu/keys/space_bar.png",
	"src/assets/textures/menu/keys/p_key.png",
	"src/assets/textures/menu/keys/i_key.png",
	"src/assets/textures/menu/keys/game_controls.png",
    "src/assets/textures/menu/GameOverTitle.png",
    "src/assets/textures/menu/Quit.png",
    "src/assets/textures/menu/Restart.png"
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
	&img[13],	
};

Global g;

Platform platform(&img[1], 320.0f, 100.0f, 641.0f, 231.0f);
Coin coin(&img[2], 100.0f, 200.0f, 32.0f, 32.0f, &platform); 
Player player(playerImages, 220.0f, 230.0f, 64.0f, 64.0f); // Player Position would be fixed here
Bat bat(&img[13],320.0f,240.0f,32.0f,32.0f, player);

float scrollSpeed = 0.0001;
extern bool start;
extern bool info;
extern bool gameOver;
extern void render_menu();
extern void render_Pmenu();
extern void render_controlInfo();
extern void render_GameOverScreen();

void cleanup_textures();
extern void check_button(ButtonType type, int mousex, int mousey, Bat& bat);

class X11_wrapper 
{
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
		cleanup_textures();
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
		exit(0);
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
void cleanup_textures(void);
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
	cleanup_textures();
	cleanup_fonts();
	x11.cleanupXWindows();
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
	
	// allow fonts 
	glEnable(GL_TEXTURE_2D);
    initialize_fonts();

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
	unsigned char *playerAvoidData = g.tex.playerAvoidImage->
									buildAlphaData(&img[3]);
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
	unsigned char *playerBiteData = g.tex.playerBiteImage->
									buildAlphaData(&img[4]);
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
	unsigned char *playerDashData = g.tex.playerDashImage->
									buildAlphaData(&img[5]);
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
	unsigned char *playerDeadData = g.tex.playerDeadImage->
									buildAlphaData(&img[6]);
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
	unsigned char *playerHurtData = g.tex.playerHurtImage->
									buildAlphaData(&img[7]);
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
	unsigned char *playerIdelData = g.tex.playerIdelImage->
									buildAlphaData(&img[8]);
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
	unsigned char *playerJumpData = g.tex.playerJumpImage->
									buildAlphaData(&img[9]);
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
	unsigned char *playerKickData = g.tex.playerKickImage->
									buildAlphaData(&img[10]);
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
	unsigned char *playerMoveData = g.tex.playerMoveImage->
									buildAlphaData(&img[11]);
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
	unsigned char *playerScanData = g.tex.playerScanImage->
									buildAlphaData(&img[12]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, playerScanData);
	free(playerScanData);

	//Player Hatch	
	g.tex.playerHatchImage = &img[13];
	w = g.tex.playerHatchImage->width;
	h = g.tex.playerHatchImage->height;
	glGenTextures(1, &g.tex.playerHatchTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.playerHatchTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	unsigned char *playerHatchData = g.tex.playerHatchImage->
									buildAlphaData(&img[13]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, playerHatchData);
	free(playerHatchData);	

	player.loadTextures(playerImages);
	
	//play button
	g.tex.playbuttonImage = &Mimg[2];
    glGenTextures(1, &g.tex.playbuttonTexture);
    w = g.tex.playbuttonImage->width;
    h = g.tex.playbuttonImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.playbuttonTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *buttonData = g.tex.playbuttonImage->buildAlphaData(&Mimg[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, buttonData);
    free(buttonData);

	//title
	g.tex.TitleImage = &Mimg[3];
    glGenTextures(1, &g.tex.TitleTexture);
    w = g.tex.TitleImage->width;
    h = g.tex.TitleImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.TitleTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char * titleData = g.tex.TitleImage->buildAlphaData(&Mimg[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, titleData);
    free(titleData);

    //menu background 
	g.tex.MenuImage = &Mimg[4];
	glGenTextures(1, &g.tex.MenuTexture);
	w = g.tex.MenuImage->width;
	h = g.tex.MenuImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.MenuTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
							GL_RGB, GL_UNSIGNED_BYTE, g.tex.MenuImage->data);
	
	// pause title
	g.tex.PTitleImage = &Mimg[0];
    glGenTextures(1, &g.tex.PTitleTexture);
    w = g.tex.PTitleImage->width;
    h = g.tex.PTitleImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.PTitleTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char * PtitleData = g.tex.PTitleImage->buildAlphaData(&Mimg[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, PtitleData);
    free(PtitleData);
      
	// small play button
	g.tex.playbuttonMiniImage = &Mimg[1];
    glGenTextures(1, &g.tex.playbuttonMiniTexture);
    w = g.tex.playbuttonMiniImage->width;
    h = g.tex.playbuttonMiniImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.playbuttonMiniTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *MinibuttonData = g.tex.playbuttonMiniImage->
									buildAlphaData(&Mimg[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, MinibuttonData);
    free(MinibuttonData);
    
	// arrow button
	g.tex.nextbuttonImage = &Mimg[5];
    glGenTextures(1, &g.tex.nextbuttonTexture);
    w = g.tex.nextbuttonImage->width;
    h = g.tex.nextbuttonImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.nextbuttonTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *nextbuttonData = g.tex.nextbuttonImage->buildAlphaData(&Mimg[5]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                        GL_RGBA, GL_UNSIGNED_BYTE, nextbuttonData);
    free(nextbuttonData);
     
	// control backdrop 
	g.tex.info_backImage = &Mimg[6];
    glGenTextures(1, &g.tex.info_backTexture);
    w = g.tex.info_backImage->width;
    h = g.tex.info_backImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.info_backTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *info_backData = g.tex.info_backImage->buildAlphaData(&Mimg[6]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        				GL_RGBA, GL_UNSIGNED_BYTE, info_backData);
    free(info_backData);
	
	// space bar key
	g.tex.esc_keyImage = &Mimg[7];
    w = g.tex.esc_keyImage->width;
    h = g.tex.esc_keyImage->height;
	glGenTextures(1, &g.tex.esc_keyTexture);
	glBindTexture(GL_TEXTURE_2D, g.tex.esc_keyTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *esc_keyData = g.tex.esc_keyImage->buildAlphaData(&Mimg[7]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        				GL_RGBA, GL_UNSIGNED_BYTE, esc_keyData);
    free(esc_keyData);

	//arrow key
	g.tex.arrow_keyImage = &Mimg[8];
    w = g.tex.arrow_keyImage->width;
    h = g.tex.arrow_keyImage->height;
	glGenTextures(1, &g.tex.arrow_keyTexture);
	glBindTexture(GL_TEXTURE_2D, g.tex.arrow_keyTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *arrow_keyData = g.tex.arrow_keyImage->buildAlphaData(&Mimg[8]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        				GL_RGBA, GL_UNSIGNED_BYTE, arrow_keyData);
    free(arrow_keyData);
	
	// space bar
   	g.tex.space_barImage = &Mimg[9];
    w = g.tex.space_barImage->width;
    h = g.tex.space_barImage->height;
	glGenTextures(1, &g.tex.space_barTexture);
	glBindTexture(GL_TEXTURE_2D, g.tex.space_barTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *space_barData = g.tex.space_barImage->buildAlphaData(&Mimg[9]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        				GL_RGBA, GL_UNSIGNED_BYTE, space_barData);
    free(space_barData);
	
	//p key
	g.tex.p_keyImage = &Mimg[10];
    w = g.tex.p_keyImage->width;
    h = g.tex.p_keyImage->height;
	glGenTextures(1, &g.tex.p_keyTexture);
	glBindTexture(GL_TEXTURE_2D, g.tex.p_keyTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *p_keyData = g.tex.p_keyImage->buildAlphaData(&Mimg[10]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        				GL_RGBA, GL_UNSIGNED_BYTE, p_keyData);
    free(p_keyData);

	//i key
	g.tex.i_keyImage = &Mimg[11];
    w = g.tex.i_keyImage->width;
    h = g.tex.i_keyImage->height;
	glGenTextures(1, &g.tex.i_keyTexture);
	glBindTexture(GL_TEXTURE_2D, g.tex.i_keyTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *i_keyData = g.tex.i_keyImage->buildAlphaData(&Mimg[11]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        				GL_RGBA, GL_UNSIGNED_BYTE, i_keyData);
    free(i_keyData);
	
	//control menu back
	g.tex.control_MImage = &Mimg[12];
    w = g.tex.control_MImage->width;
    h = g.tex.control_MImage->height;
	glGenTextures(1, &g.tex.control_MTexture);
	glBindTexture(GL_TEXTURE_2D, g.tex.control_MTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *control_MData = g.tex.control_MImage->buildAlphaData(&Mimg[12]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        				GL_RGBA, GL_UNSIGNED_BYTE, control_MData);
    free(control_MData);

	// bat 
	g.tex.BatImage = &img[14];
	w = g.tex.BatImage->width;
	h = g.tex.BatImage->height;
	glGenTextures(1, &g.tex.BatTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.BatTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *BatData = g.tex.BatImage->buildAlphaData(&img[14]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, BatData);
	free(BatData);

	//fire balls
	g.tex.fireImage = &img[15];
	w = g.tex.fireImage->width;
	h = g.tex.fireImage->height;
	glGenTextures(1, &g.tex.fireTexture); 
	glBindTexture(GL_TEXTURE_2D, g.tex.fireTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	unsigned char *fireData = g.tex.fireImage->buildAlphaData(&img[15]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, fireData);
	free(fireData);

    //GameOverTitle
   g.tex.GameOverTitleImage = &Mimg[13];
   glGenTextures(1, &g.tex.GameOverTitleTexture);
    w = g.tex.GameOverTitleImage->width;
    h = g.tex.GameOverTitleImage->height;
   glBindTexture(GL_TEXTURE_2D, g.tex.GameOverTitleTexture);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   unsigned char * GameOverTitleData = g.tex.GameOverTitleImage->buildAlphaData(&Mimg[13]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                       GL_RGBA, GL_UNSIGNED_BYTE, GameOverTitleData);
   free(GameOverTitleData);


   //Quit button
   g.tex.quitButtonImage = &Mimg[14];
   glGenTextures(1, &g.tex.quitButtonTexture);
    w = g.tex.quitButtonImage->width;
    h = g.tex.quitButtonImage->height;
   glBindTexture(GL_TEXTURE_2D, g.tex.quitButtonTexture);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   unsigned char *quitButtonData = g.tex.quitButtonImage->buildAlphaData(&Mimg[14]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                       GL_RGBA, GL_UNSIGNED_BYTE, quitButtonData);
   free(quitButtonData);


   //Return Button
   g.tex.restartButtonImage = &Mimg[15];
   glGenTextures(1, &g.tex.restartButtonTexture);
    w = g.tex.restartButtonImage->width;
    h = g.tex.restartButtonImage->height;
   glBindTexture(GL_TEXTURE_2D, g.tex.restartButtonTexture);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   unsigned char *restartButtonData = g.tex.restartButtonImage->buildAlphaData(&Mimg[15]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                       GL_RGBA, GL_UNSIGNED_BYTE, restartButtonData);
   free(restartButtonData);

	player.loadTextures(playerImages);
}

void cleanup_textures() 
{
	glDeleteTextures(1, &g.tex.backTexture);
    glDeleteTextures(1, &g.tex.platformTexture);
    glDeleteTextures(1, &g.tex.coinTexture);
    glDeleteTextures(1, &g.tex.playerAvoidTexture);
    glDeleteTextures(1, &g.tex.playerBiteTexture);
    glDeleteTextures(1, &g.tex.playerDashTexture);
    glDeleteTextures(1, &g.tex.playerDeadTexture);
    glDeleteTextures(1, &g.tex.playerHurtTexture);
    glDeleteTextures(1, &g.tex.playerIdelTexture);
    glDeleteTextures(1, &g.tex.playerJumpTexture);
    glDeleteTextures(1, &g.tex.playerKickTexture);
    glDeleteTextures(1, &g.tex.playerMoveTexture);
    glDeleteTextures(1, &g.tex.playerScanTexture);
	glDeleteTextures(1, &g.tex.playerHatchTexture);
    glDeleteTextures(1, &g.tex.BatTexture);
    glDeleteTextures(1, &g.tex.fireTexture);
    glDeleteTextures(1, &g.tex.playbuttonTexture);
    glDeleteTextures(1, &g.tex.TitleTexture);
    glDeleteTextures(1, &g.tex.MenuTexture);
    glDeleteTextures(1, &g.tex.PTitleTexture);
    glDeleteTextures(1, &g.tex.playbuttonMiniTexture);
    glDeleteTextures(1, &g.tex.nextbuttonTexture);
    glDeleteTextures(1, &g.tex.info_backTexture);
    glDeleteTextures(1, &g.tex.esc_keyTexture);
    glDeleteTextures(1, &g.tex.arrow_keyTexture);
    glDeleteTextures(1, &g.tex.space_barTexture);
    glDeleteTextures(1, &g.tex.p_keyTexture);
    glDeleteTextures(1, &g.tex.i_keyTexture);
    glDeleteTextures(1, &g.tex.control_MTexture);
	cleanup_fonts();
	g.tex.backImage = nullptr;
}
void check_mouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;
	if (e->type == ButtonRelease) {
		return;
	}

	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
		
			int mousex = e->xbutton.x;
			int mousey = g.yres - e->xbutton.y;
			
			//XSync(Display, 0);

			if (!g.paused) {
				check_button(START, mousex, mousey, bat);
			}
			if (g.paused) {
				check_button(INFO, mousex, mousey, bat);
				check_button(PAUSE, mousex, mousey, bat);
			}
			if (info) {
				check_button(CLOSE_INFO, mousex, mousey, bat);
			}
            
            //Quit button
            int quitButtonLeft = g.xres / 2 - (g.xres * 0.05);
            int quitButtonRight = g.xres / 2 + (g.xres * 0.05);
            int quitButtonBottom = g.yres / 4 - (g.yres * 0.1);
            int quitButtonTop = g.yres / 4 + (g.yres * 0.1);
            if (mousex >= quitButtonLeft &&
                mousex <= quitButtonRight &&
                mousey >= quitButtonBottom &&
                mousey <= quitButtonTop) {
                exit(0);
            }

            // Return button
            int returnButtonLeft = g.xres / 4 - (g.xres * 0.05);
            int returnButtonRight = g.xres / 4 + (g.xres * 0.05);
            int returnButtonBottom = g.yres / 4 - (g.yres * 0.1);
            int returnButtonTop = g.yres / 4 + (g.yres * 0.1);
            if (mousex >= returnButtonLeft &&
                mousex <= returnButtonRight &&
                mousey >= returnButtonBottom &&
                mousey <= returnButtonTop) {
                g.isGameOver = false;
                start = true;
                return;
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
		if (key == XK_i) {
           info = !info;
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
	// Grace Period Start
	if (start && g.gracePeriod > 0.0f) {
		g.gracePeriod -= deltaTime;
		if (g.gracePeriod <= 0.0f) {
			g.IsGracePeriod = false;
			g.spawnAnimation = false; 
		}
		return;
	}

	if (g.spawnAnimation) {
		return;
	}
	
	if (!g.paused) {
		g.tex.xc[0] += scrollSpeed;
		g.tex.xc[1] += scrollSpeed;
		platform.updatePlatforms(g.xres, g.yres, deltaTime);
		coin.updateCoins(g.xres, g.yres, deltaTime);
		coin.checkCoinCollision(player);
		player.handleFalling(platform);
    	bat.updateTimer(deltaTime);
		bat.shootFireball();
		bat.checkFireCollision(player);
		bat.updateSwooping(deltaTime);

		for(auto& fireball : bat.fireballs) {
			fireball.update();
		}

	}	
	
}

void render() 
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
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
	glColor4ub(255,255,255,255);
    player.render();
	bat.render();
	glPopMatrix();
	
	glBindTexture(GL_TEXTURE_2D, g.tex.fireTexture);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);
	
	for (auto& fireball : bat.fireballs) {
		fireball.render_fire();
	}
   
    glDisable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
    glPopMatrix();

	if (start && g.spawnAnimation) {
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_ALPHA_TEST);

		int totalFrames = 4;
		float frameWidth = 1.0f / totalFrames;
		float tx1 = g.hatchFrame * frameWidth;
		float tx2 = tx1 + frameWidth;

		glBindTexture(GL_TEXTURE_2D, g.tex.playerHatchTexture);
		glColor4ub(255, 255, 255, 255);
		glPushMatrix();
		glTranslatef(player.getX(), player.getY(), 0);
		glBegin(GL_QUADS);
			glTexCoord2f(tx1, 1.0f); glVertex2f(-player.getWidth() / 2, -player.getHeight() / 2);
			glTexCoord2f(tx2, 1.0f); glVertex2f(player.getWidth() / 2, -player.getHeight() / 2);
			glTexCoord2f(tx2, 0.0f); glVertex2f(player.getWidth() / 2, player.getHeight() / 2);
			glTexCoord2f(tx1, 0.0f); glVertex2f(-player.getWidth() / 2, player.getHeight() / 2);
		glEnd();
		glPopMatrix();
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		struct timespec now;
		clock_gettime(CLOCK_REALTIME, &now);
		double timeDiff = timers.timeDiff(&g.hatchTime, &now);
		
		if (timeDiff > 0.3) {
			g.hatchFrame++;
			timers.recordTime(&g.hatchTime);
		}
		if (g.hatchFrame >= totalFrames) {
			g.hatchFrame = 0;
			g.spawnAnimation = false;
		}
		if (start && g.gracePeriod > 0.0f) {
			glBindTexture(GL_TEXTURE_2D, 0);
    		glEnable(GL_TEXTURE_2D);
			Rect r;
			initialize_fonts();
			r.bot = g.yres / 2;
			r.left = g.xres /2 - 40;
			r.center = 1;
			ggprint10(&r, 16, 0x00000000, "Get Ready!");
			glDisable(GL_TEXTURE_2D);
			return; 
		}
	}	
	
	render_menu();
    render_Pmenu();
	render_controlInfo();
	render_GameOverScreen();
	
}


