#ifndef RHYTHM_H
#define RHYTHM_H
#define NOTE_COUNT 96
#define PERFECT_WINDOW 3
#define GOOD_WINDOW 7
#define MISS_WINDOW 10
#define LEAD_IN 180
#define BEAT_FRAMES 30
 typedef struct { int frame, lane, judged; } Note;
typedef struct { Note notes[NOTE_COUNT]; int score, combo, best, perfect, good, miss, remaining; } Game;
void game_init(Game *g);
int game_hit(Game *g, int lane, int frame);
void game_update(Game *g, int frame);
#endif
