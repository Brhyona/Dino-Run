#ifndef GLOBAL_H
#define GLOBAL_H

#include <GL/glx.h>
#include <time.h>
#include "imageLoader.h"

typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

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
	Image *coinImage;
	GLuint coinTexture;
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

	Vec box[20];

	float xc[2];
	float yc[2];
};

class Global {
public:
	int xres, yres, paused, coin, coinFrame;
	int avoiding, biting, dashing, dead, hurting, idel, jumping, kicking, moving, scanning, playerFrame;
	Texture tex;
	struct timespec coinTime;
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
	double delay;

	Global() {
		xres=550, yres=400, paused=0, coin=0, coinFrame=0, delay = 0.3;
		avoiding=0, biting=0, dashing=0, dead=0, hurting=0, idel=0, jumping=0, kicking=0, moving=0, scanning=0, playerFrame=0;

		timers.recordTime(&coinTime);
		tex.coinImage=nullptr;
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
	}

};

extern Global g;
#endif //GLOBAL_H