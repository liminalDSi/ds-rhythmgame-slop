#include <assert.h>
#include <stdio.h>
#include "rhythm.h"
int main(void) {
 Game g; game_init(&g);
 assert(game_hit(&g,0,LEAD_IN-8)==0 && g.remaining==96);
 assert(game_hit(&g,0,LEAD_IN-3)==2 && g.score==1000);
 assert(game_hit(&g,0,LEAD_IN)==0 && g.remaining==95);
 assert(game_hit(&g,1,LEAD_IN+30+7)==1 && g.score==1500);
 game_update(&g,LEAD_IN+60+11);
 assert(g.miss==1 && g.combo==0);
 game_init(&g);
 for(int i=0;i<NOTE_COUNT;i++) assert(game_hit(&g,g.notes[i].lane,g.notes[i].frame)==2);
 assert(g.remaining==0 && g.score==96000 && g.best==96);
 game_init(&g); game_update(&g,10000);
 assert(g.miss==96 && g.remaining==0);
 puts("Timing, duplicate hits, scoring, misses, and full-song checks passed.");
}
