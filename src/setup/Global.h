#ifndef GLOBAL_H
#define GLOBAL_H

#include <GL/glx.h>
#include <time.h>
#include "imageLoader.h"
#include "oal.h"

typedef double Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];

#ifdef USE_OPENAL_SOUND
#include <AL/al.h>
#include <AL/alc.h>
#endif

struct Hitbox{
    float x, y, width, height;
};

class Timers {
public:
	double physicsRate;
	double oobillion;
	struct timespec timeStart, timeEnd, timeCurrent;
	struct timespec walkTime;
	Timers() {
		physicsRate = 1.0 / 30.0;
		oobillion = 1.0 / 1e9;
	}
	double timeDiff(struct timespec *start, struct timespec *end) {
		return (double)(end->tv_sec - start->tv_sec ) +
				(double)(end->tv_nsec - start->tv_nsec) * oobillion;
	}
	void timeCopy(struct timespec *dest, struct timespec *source) {
		memcpy(dest, source, sizeof(struct timespec));
	}
	void recordTime(struct timespec *t) {
		clock_gettime(CLOCK_REALTIME, t);
	}
};

extern Timers timers;

class Texture {
public:
//Setup
	Image *backImage;
	GLuint backTexture;	
	Image *platformImage;
	GLuint platformTexture;
	Image *fallingPlatformImage;
	GLuint fallingPlatformTexture;
	Image *coinImage;
	GLuint coinTexture;
// Menu
	Image *MenuImage;
    GLuint MenuTexture;
	Image *TitleImage;
    GLuint TitleTexture;
	Image *playbuttonImage;
	GLuint playbuttonTexture;

	// Pause Menu
	Image *PTitleImage;
	GLuint PTitleTexture;
	Image *playbuttonMiniImage;
	GLuint playbuttonMiniTexture;
	Image *nextbuttonImage;
	GLuint nextbuttonTexture;
	Image *info_backImage;
	GLuint info_backTexture;
	Image *esc_keyImage;
	GLuint esc_keyTexture;
	Image *arrow_keyImage;
	GLuint arrow_keyTexture;
	Image *space_barImage;
	GLuint space_barTexture;
	Image *p_keyImage;
	GLuint p_keyTexture;
	Image *i_keyImage;
	GLuint i_keyTexture;
	Image *control_MImage;
	GLuint control_MTexture;

	//End Menu 
	Image *GameOverImage;
	GLuint GameOverTexture;
	Image *GameOverTitleImage;
	GLuint GameOverTitleTexture;
	Image *restartButtonImage;
	GLuint restartButtonTexture;
	Image *quitButtonImage;
	GLuint quitButtonTexture;

//Player
	Image *playerAvoidImage;
	GLuint playerAvoidTexture;
	Image *playerBiteImage;
	GLuint playerBiteTexture;
	Image *playerDashImage;
	GLuint playerDashTexture;
	Image *playerDeadImage;
	GLuint playerDeadTexture;
	Image *playerHurtImage;
	GLuint playerHurtTexture;
	Image *playerIdelImage;
	GLuint playerIdelTexture;
	Image *playerJumpImage;
	GLuint playerJumpTexture;
	Image *playerKickImage;
	GLuint playerKickTexture;
	Image *playerMoveImage;
	GLuint playerMoveTexture;
	Image *playerScanImage;
	GLuint playerScanTexture;
	Image *playerHatchImage;
	GLuint playerHatchTexture;

	//enemies
	Image *BatImage;
	GLuint BatTexture;
	Image *fireImage;
	GLuint fireTexture;

    Image *enemyRunImage;
    GLuint enemyRunTexture;
    Image *enemyWhipImage;
    GLuint enemyWhipTexture;

	//Traps
	Image *arrowImage;
	GLuint arrowTexture;

	Image *fireTrapImage;
	GLuint fireTrapTexture;

	Vec box[20];

	float xc[2];
	float yc[2];
	float xc2[2];
};

class Global {
public:
	int xres, yres, paused, coin, coinFrame, fireTrapFrame;
	int avoiding, biting, dashing, dead, hurting, idel, jumping, kicking, moving, scanning, playerFrame;
    int enemyrun, enemywhip, enemyframe;
	Texture tex;
	bool isGameOver = false;
	
	struct timespec coinTime;
	struct timespec fireTrapTime;
	struct timespec AvoidTime;
	struct timespec BiteTime;
	struct timespec DashTime;
	struct timespec DeadTime;
	struct timespec HurtTime;
	struct timespec IdelTime;
	struct timespec JumpTime;
	struct timespec KickTime;
	struct timespec MoveTime;
	struct timespec ScanTime;
    struct timespec EnemyRunTime;
    struct timespec EnemyWhipTime;
	double delay;

	float gracePeriod = 3.0f;
	bool IsGracePeriod = true;
	bool spawnAnimation = true;
	int hatchFrame = 0;
	struct timespec hatchTime;	

	//Sounds
	#ifdef USE_OPENAL_SOUND
	ALuint alBufferCoin, alBufferJump;
	ALuint alSourceCoin, alSourceJump;
	ALuint alBufferFalling, alBufferBones;
	ALuint alSourceFalling, alSourceBones;
	#endif //USE_OPENAL_SOUND
	
	Global() {
		xres=550, yres=400, paused=0, coin=0, coinFrame=0, fireTrapFrame=0, delay = 0.15;
		avoiding=0, biting=0, dashing=0, dead=0, hurting=0, idel=0, jumping=0, kicking=0, moving=0, scanning=0, playerFrame=0, enemyrun=0, enemywhip=0;
		isGameOver = false;

		timers.recordTime(&coinTime);
		tex.coinImage=nullptr;
		timers.recordTime(&fireTrapTime);
		tex.fireTrapImage=nullptr;
		timers.recordTime(&AvoidTime);
		tex.playerAvoidImage=nullptr;
		timers.recordTime(&BiteTime);
		tex.playerBiteImage=nullptr;
		timers.recordTime(&DashTime);
		tex.playerDashImage=nullptr;
		timers.recordTime(&DeadTime);
		tex.playerDeadImage=nullptr;
		timers.recordTime(&HurtTime);
		tex.playerHurtImage=nullptr;
		timers.recordTime(&IdelTime);
		tex.playerIdelImage=nullptr;
		timers.recordTime(&JumpTime);
		tex.playerJumpImage=nullptr;
		timers.recordTime(&KickTime);
		tex.playerKickImage=nullptr;
		timers.recordTime(&MoveTime);
		tex.playerMoveImage=nullptr;
		timers.recordTime(&ScanTime);
		tex.playerScanImage=nullptr;
		timers.recordTime(&EnemyRunTime);
		tex.enemyRunImage=nullptr;
		timers.recordTime(&EnemyWhipTime);
		tex.enemyWhipImage=nullptr;
	}
	struct {
float x;
float y;
float width;
float height;

}playButton; 

};

extern Global g;
extern void render_GameOverScreen();
#endif //GLOBAL_H
