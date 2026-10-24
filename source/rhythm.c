#include "rhythm.h"
#include <string.h>
#include <stdlib.h>
void game_init(Game *g) {
    static const int phrase[16] = {0,1,2,3,2,1,0,2,1,3,0,1,3,2,1,3};
    memset(g, 0, sizeof(*g));
    g->remaining = NOTE_COUNT;
    for (int i=0;i<NOTE_COUNT;i++) {
        g->notes[i].frame = LEAD_IN + i * BEAT_FRAMES;
        g->notes[i].lane = phrase[(i + (i/16)*3)%16];
    }
}
int game_hit(Game *g, int lane, int frame) {
    int best=-1, distance=GOOD_WINDOW+1;
    for (int i=0;i<NOTE_COUNT;i++) {
        Note *n=&g->notes[i];
        int d=abs(frame-n->frame);
        if (!n->judged && n->lane==lane && d<distance) { best=i; distance=d; }
    }
    if (best<0) { g->combo=0; return 0; }
    g->notes[best].judged=1; g->remaining--; g->combo++;
    if (g->combo>g->best) g->best=g->combo;
    if (distance<=PERFECT_WINDOW) { g->perfect++; g->score+=1000; return 2; }
    g->good++; g->score+=500; return 1;
}
void game_update(Game *g, int frame) {
    for (int i=0;i<NOTE_COUNT;i++) {
        Note *n=&g->notes[i];
        if (!n->judged && frame-n->frame>MISS_WINDOW) {
            n->judged=1; g->remaining--; g->miss++; g->combo=0;
        }
    }
}
