#include "iostm8s103f3.h"
#include "iic.h"
#include "wh1604a.h"
#include "timers_and_buttons.h"
#include "measurement.h"
#include "sound.h"


settings_struct s_settings @0x4100; //store setting variables (in EEPROM)
buttons_struct s_buttons;           //store button variables wich is used by interrupt handlers
sensor_struct s_sensor;             //store measured values variables wich is used by interrupt handlers
out_struct s_out;                   // state outputs (ULN2803A)(used by interrupt handlers)
settings_struct set_0;

int main()
{ 
    char stste_screen=0;
    
  
   
   set_0.b_en_sauna_lm75a =1;
   set_0.b_en_saund  =1;
   set_0.address_lm75a_sauna =0x48;
   set_0.address_lm75a_transf=0x49;
   set_0.address_lm75a_oven  =0x4b;
   set_0.address_lm75a_smoke =0x4f;
   set_0.sauna_temp_limit   =110;
   set_0.sauna_aht10_temp_limit=90;
   set_0.transf_temp_limit =80;
   set_0.oven_temp_limit   =110;
   set_0.smoke_temp_limit  =110;
   set_0.P_limit_max       =600;
   set_0.P_limit_min       =200;
   set_0.time_limit   =360;
    
  // write_eeprom_settings(&s_settings,&set_0);

    s_sensor.timer_min=s_settings.time_limit;
    init_tim1(); 
    init_ULN2803A();
    init_aht10();
    init_buttons();
    init_tim4();
    init1604(); 

    

 while(1)
 {  
switch(stste_screen)
{
case 0:    {stste_screen=main_screen();     break;}
case 1:    {stste_screen=screen_1();        break;}
case 2:    {stste_screen=screen_2_P_U_I();  break;}
case 100:  {stste_screen= alarm_screen();   break;}
}
 }
}
