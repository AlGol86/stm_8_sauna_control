#include "iostm8s103f3.h"


#define PIN_left_button  2  // pA
#define PIN_right_button 3  // pA
#define PIN_up_button    4  // pB
#define PIN_down_button  5  // pB


#define PIN_chanal_1     1  //port D- light
#define PIN_chanal_2     4  //port D- 1
#define PIN_chanal_3     5  //port D- 2
#define PIN_chanal_4     6  //port D- beeper

#define BIT_chanal_1    (1<<PIN_chanal_1)
#define BIT_chanal_2    (1<<PIN_chanal_2)
#define BIT_chanal_3    (1<<PIN_chanal_3)
#define BIT_chanal_4    (1<<PIN_chanal_4)


#define BIT_left_button   (1<<PIN_left_button)
#define BIT_right_button  (1<<PIN_right_button)
#define BIT_up_button     (1<<PIN_up_button)
#define BIT_down_button   (1<<PIN_down_button)

//buttons pushing
#define LONG_PUSH  100
#define SMART_PUSH 10

#define MAX_TIMER   360


#define eeprom_settings ((char*)0x4010)

typedef struct
{
  unsigned char b_left        : 1;
  unsigned char b_right       : 1;
  unsigned char b_up          : 1;
  unsigned char b_down        : 1;
  unsigned char b_left_long   : 1;
  unsigned char b_right_long  : 1;
  unsigned char b_up_long     : 1;
  unsigned char b_down_long   : 1;  
  unsigned char b_event       : 1;
  unsigned char b_ev_long     : 1;
  unsigned int count             ;
  unsigned int long_push         ;
  unsigned int sys_count         ;
} buttons_struct;

typedef struct
{
  unsigned char b_en_sauna_lm75a    : 1;  //indicate on main
  unsigned char b_en_saund          : 1;
  unsigned char address_lm75a_sauna    ;
  unsigned char address_lm75a_transf   ;
  unsigned char address_lm75a_oven     ;
  unsigned char address_lm75a_smoke    ;
  unsigned char sauna_temp_limit       ;
  unsigned char sauna_aht10_temp_limit       ;
  unsigned char transf_temp_limit      ;
  unsigned char oven_temp_limit        ;
  unsigned char smoke_temp_limit       ;
  unsigned int  P_limit_max            ;
  unsigned int  P_limit_min            ;
  
  unsigned int time_limit              ;

} settings_struct;



typedef struct
{
  unsigned char alarm_sound     :1;
  unsigned char alarm           :1;
  unsigned char alarm_P_max     :1;
  unsigned char alarm_P_min     :1;
  unsigned char alarm_T_transf  :1;
  unsigned char alarm_T_sauna   :1;
  unsigned char alarm_T_sauna_aht10   :1;
  unsigned char alarm_T_smoke   :1;
  unsigned char alarm_T_oven    :1;
  unsigned char out_1           :1; //heat
  unsigned char out_2           :1; 
  unsigned char alarm_counter     ;  
} out_struct;




#pragma vector = 13
__interrupt void time1(void);

#pragma vector = 25
__interrupt void time4(void);


void init_tim1(void);
void init_tim4(void);
void init_buttons(void);
void buttons( void);
void write_eeprom_settings(settings_struct* addr_eeprom,settings_struct* data);///save to eeprom

 
char alarm_screen(void); 
char main_screen(void);
char screen_1(void);
char screen_2_P_U_I(void);


void init_ULN2803A(void);
void switch_on_light(void);
void switch_off_light(void);
void switch_on_1(void);
void switch_off_1(void);
void switch_on_2(void);
void switch_off_2(void);
void switch_on_beeper(void);
void switch_off_beeper(void);


