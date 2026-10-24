#include <nds.h>
#include <stdio.h>
#include "rhythm.h"

static u16 *screen;
static Game game;
static signed char tones[4][256] __attribute__((aligned(32)));
static const int frequencies[4]={262,330,392,523};
static const int buttons[4]={KEY_LEFT,KEY_DOWN,KEY_X,KEY_A};
static void rect(int x,int y,int w,int h,u16 color) {
    for(int j=y;j<y+h;j++) for(int i=x;i<x+w;i++)
        if(i>=0 && i<256 && j>=0 && j<192) screen[j*256+i]=color;
}
static void tone(int lane) {
    soundPlaySample(tones[lane], SoundFormat_8Bit, sizeof(tones[lane]),
                    frequencies[lane]*32, 75, 32+lane*64, false, 0);
}
static void draw(int frame,int flash,int feedback) {
    rect(0,0,256,192,RGB15(1,2,5)|BIT(15));
    for(int l=0;l<4;l++) {
        u16 color=RGB15(5+l*5,24-l*4,29)|BIT(15);
        rect(l*64+3,0,58,192,RGB15(2,4,8)|BIT(15));
        rect(l*64+7,157,50,4,color);
        if(flash&(1<<l)) rect(l*64+7,164,50,17,color);
    }
    for(int i=0;i<NOTE_COUNT;i++) {
        Note *n=&game.notes[i];
        int y=158-(n->frame-frame);
        if(!n->judged && y>-8 && y<192) {
            rect(n->lane*64+10,y-4,44,8,RGB15(28,16+n->lane*3,6+n->lane*7)|BIT(15));
            rect(n->lane*64+13,y-3,38,2,RGB15(31,31,31)|BIT(15));
        }
    }
    if(feedback>0) rect(0,189,256,3,RGB15(10,31,18)|BIT(15));
}
int main(void) {
    powerOn(POWER_ALL_2D);
    videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    int bg=bgInit(3,BgType_Bmp16,BgSize_B16_256x256,0,0);
    screen=bgGetGfxPtr(bg);
    consoleDemoInit();
    soundEnable();
    for(int l=0;l<4;l++) for(int i=0;i<256;i++)
        tones[l][i]=(i%32<16?55:-55)*(255-i)/255;
    DC_FlushRange(tones,sizeof(tones));
    int state=0,frame=0,feedback=0,feedbackTimer=0;
    game_init(&game);
    while(1) {
        swiWaitForVBlank(); scanKeys();
        int pressed=keysDown(), flash=0;
        if(pressed&KEY_START) {
            if(state==0 || state==3) { game_init(&game); frame=0; state=1; }
            else state=state==1?2:1;
        }
        if(state==1) {
            for(int l=0;l<4;l++) if(pressed&buttons[l]) {
                flash|=1<<l; feedback=game_hit(&game,l,frame); feedbackTimer=20;
                tone(l);
            }
            game_update(&game,frame);
            if(frame>=LEAD_IN && (frame-LEAD_IN)%BEAT_FRAMES==0) tone(((frame-LEAD_IN)/BEAT_FRAMES)%4);
            frame++;
            if(!game.remaining) state=3;
        }
        if(feedbackTimer>0) feedbackTimer--;
        draw(frame,flash,feedbackTimer?feedback:0);
        iprintf("\x1b[2J\x1b[1;7HNEON / PULSE\n\n");
        if(state==0) {
            iprintf("     A Nintendo DS rhythm game\n\n LEFT   DOWN    X      A\n   1      2     3      4\n\n Hit notes at the bright line.\n\n Perfect: +/- 3 frames\n Good:    +/- 7 frames\n\n       START to play\n\n     120 BPM / 96 notes\n");
        } else {
            iprintf(" Score  %06d\n Combo  %d   Best %d\n\n Perfect %d\n Good    %d\n Miss    %d\n\n",game.score,game.combo,game.best,game.perfect,game.good,game.miss);
            if(state==2) iprintf(" PAUSED - START to resume\n");
            else if(state==3) iprintf(" SONG COMPLETE!\n Accuracy: %d%%\n\n START to play again\n",game.score*100/(NOTE_COUNT*1000));
            else if(frame<LEAD_IN) iprintf(" Get ready: %d\n",(LEAD_IN-frame+59)/60);
            else iprintf(" %s\n",feedbackTimer?(feedback==2?"PERFECT!":feedback==1?"GOOD":"EMPTY HIT"):" ");
            iprintf("\n LEFT / DOWN / X / A\n START: pause\n");
        }
    }
}
