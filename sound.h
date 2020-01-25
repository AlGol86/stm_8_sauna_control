#include "iostm8s103f3.h"
//beep_type
#define BEEP_PUSH  1
#define BEEP_ALARM 100

// 4 octave
#define d_4 1
#define rb_4 2
#define r_4 3
#define mb_4 4
#define m_4 5
#define f_4 6
#define sb_4 7
#define s_4 8
#define lb_4 9
#define l_4 10
#define ib_4 11
#define i_4 12
// 5 octave
#define d_5 13
#define rb_5 14
#define r_5 15
#define mb_5 16
#define m_5 17
#define f_5 18
#define sb_5 19
#define s_5 20
#define lb_5 21
#define l_5 22
#define ib_5 23
#define i_5 24
#define d_6 25
#define pa 30

void beep( char note, char continuance);

#pragma vector = 15
__interrupt void time2(void);

#pragma vector = 16
__interrupt void time2cc(void);
