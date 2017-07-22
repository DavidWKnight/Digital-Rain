
#ifndef DROPLETS_H_
#define DROPLETS_H_

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define SHORT_DROPLET //use short for droplet struct, saves memory
//#define TASTE_THE_RAINBOW //droplet trail color dependent on length
                          //keep terminal height to >=24 rows

#ifdef SHORT_DROPLET
struct droplet{
    unsigned short length, col;
    short row;   //signed so droplets can start above screen
    unsigned short frames_per_row;
};

#else //SHORT_DROPLET
struct droplet{
    unsigned int length, col;
    int row;    //signed so droplets can start above screen
    unsigned int frames_per_row;
};

#endif //SHORT_DROPLET

int init_droplets();
void cleanup();
int draw_rain();
static int draw_droplet(struct droplet *drop);
static int gen_droplet_props(struct droplet *drop);
static int droplets_realloc();

extern unsigned int refresh_rate;
extern char multicolor_trail;
extern char velocity;
extern char primary_color;
extern char secondary_color;

#endif // DROPLETS_H_
