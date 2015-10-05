/* gcc -Wall bot.c -lX11 -lXtst -o bot */

/* 
    Kuku-kube bot to achive high scores.
    It's specially designed for kuku-kube.com,
    for any other version it might need some changes.
    Built and tested on Ubuntu 12.04 x64
    
    author: Vasilis Naskos 
    https://github.com/vnaskos
    designed for: kuku-kube.com
*/


//=========SETTINGS===============
#define START_X 413					//x coordinate of the game's top left corner on the screen
#define START_Y 206					//y left top
#define END_X 917					//x coordinate of the game's bottom right corner on the screen 
#define END_Y 706					//y bottom right
#define START_DELAY 3000000			//Delay brfore the bot begin
#define FIRST_LEVELS_DELAY 46500	//Delay after click for levels 1-17
#define CLICK_DELAY 42000			//Delay after click for levels > 17
//================================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

typedef struct {
	int x,y;
	XColor c;
} square;

int boxCnt	= 2;
int step;

int score = 1;
int boxUp[] = {2,3,4,6,8,11,17};

int isColorsEqual(XColor* lhs, XColor* rhs)
{
	return ((*lhs).red == (*rhs).red && (*lhs).green == (*rhs).green && (*lhs).blue == (*rhs).blue);
}

XColor getPixelColor(Display *dpy, int x, int y)
{
	XColor c;
	XImage *image;
	
	image = XGetImage (dpy, RootWindow (dpy, DefaultScreen (dpy)), x, y, 1, 1, AllPlanes, XYPixmap);
	c.pixel = XGetPixel (image, 0, 0);
	XFree (image);
	XQueryColor (dpy, DefaultColormap(dpy, DefaultScreen (dpy)), &c);
	
	return c;
}

void mouseClickAt(Display *dpy, int x, int y)
{
	XTestFakeMotionEvent(dpy, -1, x, y, 0);
	XTestFakeButtonEvent(dpy, Button1, 1, 0);
	XTestFakeButtonEvent(dpy, Button1, 0, 0);
}

square* getSamples(Display *dpy)
{
	int i,j;
	square cur;
	square* samples;
	
	samples = malloc(sizeof(square)*9*9);
	
	for(i=0; i<boxCnt; i++)
	{
		for(j=0; j<boxCnt; j++)
		{	
			cur.x = (START_X+(step/2)) + (i*step);
			cur.y = (START_Y+(step/2)) + (j*step);
			cur.c = getPixelColor(dpy, cur.x, cur.y);
			
			samples[(i*boxCnt)+j] = cur;
		}
	}
	
	return samples;
}

square getDiff(square* samples)
{
	int i;
	square cur;
	square prev	= samples[1];
	
	for(i=2; i<boxCnt*boxCnt; i++)
	{
		cur = samples[i];
		
		if(!isColorsEqual(&cur.c, &samples[0].c) && isColorsEqual(&cur.c, &prev.c))
			return samples[0];
		else if(isColorsEqual(&cur.c, &samples[0].c) && !isColorsEqual(&cur.c, &prev.c))
			return prev;
		else if(!isColorsEqual(&cur.c, &samples[0].c) && !isColorsEqual(&cur.c, &prev.c))
			return cur;
	}

	return samples[0];
}

void adjustStep() {
	unsigned int i;
	int levelUp;
	
	for(i=0; i<sizeof(boxUp)/sizeof(*boxUp); i++) {
		levelUp = boxUp[i];
		if(score == levelUp) {
			boxCnt++;
		}
	}
	
	step = (END_X - START_X)/boxCnt;
}

int main(int argc, char *argv[])
{
	square* samples;
	square diff;
	Display *dpy = XOpenDisplay(0);
	
	step = (END_X - START_X)/boxCnt;
	
	usleep(START_DELAY);
	printf("Start\n");
	
	time_t t1, t2;
	time(&t1);
	
	while(time(&t2) - t1 < 60)
	{
		samples = getSamples(dpy);
	
		diff = getDiff(samples);
		
		mouseClickAt(dpy, diff.x, diff.y);
		XSync(dpy, True);
		
		score++;
		if(score <= 17) {
		    adjustStep();
		    usleep(FIRST_LEVELS_DELAY);
		} else {
			usleep(CLICK_DELAY);
		}
		
    }
	
	XFlush(dpy);
	XCloseDisplay(dpy);

	return 0;
}
