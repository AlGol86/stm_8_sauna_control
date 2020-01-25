#include "iostm8s103f3.h"
#define PIN_en 1 //pA
#define PIN_rs 3 //pC

#define PIN_db7 7 //pC
#define PIN_db6 6 //pC
#define PIN_db5 5 //pC
#define PIN_db4 4 //pC

#define BIT_en  (1<<PIN_en)
#define BIT_rs  (1<<PIN_rs)
#define BIT_db7 (1<<PIN_db7)
#define BIT_db6 (1<<PIN_db6)
#define BIT_db5 (1<<PIN_db5)
#define BIT_db4 (1<<PIN_db4)
#define MASK_db (BIT_db7|BIT_db6|BIT_db5|BIT_db4)

#define N_F_code   2 //n:2-line f:5*11font
#define SC_RL_code 2 //sc:setcursor/display moving rl:right/left
#define D_C_B_code 4 //display/cursor/blinking
#define ID_SH_code 2 //cursor mov/display shift


void write_wh(char rs,char db7_db4);
void write_wh_byte(char rs,char db7_db0);
void del_tim2(char delay,char E);
void del_WH(unsigned int del);
void proba_del_tim2(void);
void init1604(void);
char print_nomber(int n, char x,char y );
void print_string(char* pointer, char x, char y); 
void print_string_cyr(char* pointer_1,char* pointer_2,char x_1,char x_2); // if LED don't provide CYR letters
void set_CGRAM(char n_CGRAM,char* data );
void clear(void);
void set_cursor(char x,char y);
char print_symb(char s,char x,char y,char count); //if LED don't provide CYR letters
char print_by_CGRAM(char n_CGRAM, char* data, char x, char y);
void print_char(char ch, char x, char y);



