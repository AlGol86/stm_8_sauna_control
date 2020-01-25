#include "iostm8s103f3.h"
#include "timers_and_buttons.h"
#include "iic.h"
#include "wh1604a.h"
#include "measurement.h"
#include "sound.h"

void init_tim1(void) // minutes counter
{
  TIM1_PSCRH=7; //prescaler 0-255  div=pscr+1
  TIM1_PSCRL=207; //prescaler 0-255 
  TIM1_ARRH=234; //preload registr high bits (shadow register)
  TIM1_ARRL=96;
         //TIM1_IER_CC1IE=1;//enable interrupt by compare
 	//TIM1_CCR1H=100;//capture/compare registr high bits
	//TIM1_CCR1L=100;
	TIM1_CNTRH=0;
	TIM1_CNTRL=0;
      	asm("rim");
        TIM1_CR1_CEN=1;
        TIM1_IER_UIE=1;
    }

 void init_tim4(void) //start requests temperature(aht10,lm75a), other measurings and requests buttons by interrupt handlers
                      // buttons();get_lm75a(0x48); get_aht10_partX();
 {
 asm("rim");
        TIM4_IER_UIE=1;
        TIM4_PSCR=6; //1 event every 0,5 ms
        TIM4_ARR=0xff;
	TIM4_CNTR=0;
        TIM4_CR1_CEN=1;
 }

void init_buttons(void)
{
  extern buttons_struct s_buttons;
  PA_DDR&=~(BIT_right_button | BIT_left_button );  // =>input
  PB_DDR&=~(BIT_up_button | BIT_down_button );     // =>input
  PA_CR1&=~(BIT_right_button | BIT_left_button );  // =>without pull-up
  PB_CR1&=~(BIT_up_button | BIT_down_button );     // =>without pull-up
  PA_CR2&=~(BIT_right_button | BIT_left_button ); //interrupts disabled
  PB_CR2&=~(BIT_up_button | BIT_down_button );    //interrupts disabled
  s_buttons.long_push=LONG_PUSH;
}

//*********************************BUTTONS*******************************//
void buttons(void)
{
  extern buttons_struct s_buttons;
  
  if ((((PA_IDR & BIT_left_button)==0)||((PA_IDR&BIT_right_button)==0)||((PB_IDR&BIT_up_button)==0)||((PB_IDR&BIT_down_button)==0))&&(s_buttons.count < (s_buttons.long_push+5))) s_buttons.count++;
   else 
     if (s_buttons.count>3) s_buttons.count=3; 
       else if (s_buttons.count>0) s_buttons.count--;  
        
 if ((s_buttons.count>2)&&(s_buttons.b_event==0))
 {
   if((PA_IDR&BIT_left_button)   ==0) {s_buttons.b_left=1; s_buttons.b_event=1;}
   if ((PA_IDR&BIT_right_button) ==0) {s_buttons.b_right=1;s_buttons.b_event=1;}
   if ((PB_IDR&BIT_up_button)    ==0) {s_buttons.b_up=1;   s_buttons.b_event=1;}
   if ((PB_IDR&BIT_down_button)  ==0) {s_buttons.b_down=1; s_buttons.b_event=1;}
 }
 if ((s_buttons.count > s_buttons.long_push)&&(s_buttons.b_ev_long==0)) 
 {
  if ((PA_IDR&BIT_left_button) ==0)
   {s_buttons.b_left_long=1;  s_buttons.b_left=1;s_buttons.b_left_long=0; }
  if ((PA_IDR&BIT_right_button)==0) 
   {s_buttons.b_right_long=1;s_buttons.b_right=1;s_buttons.b_right_long=0;}
  if ((PB_IDR&BIT_up_button)   ==0)
   {s_buttons.b_up_long=1;  s_buttons.b_up=1;s_buttons.b_up_long=0;}
  if ((PB_IDR&BIT_down_button) ==0)
   {s_buttons.b_down_long=1;s_buttons.b_down=1;s_buttons.b_down_long=0;}
  s_buttons.b_ev_long=1;
  s_buttons.long_push=SMART_PUSH;
  s_buttons.count=5;
 }
 if  (s_buttons.count<2)   {s_buttons.b_event=0;s_buttons.long_push=LONG_PUSH;}
 if  (s_buttons.count<5)  {s_buttons.b_ev_long=0;}
}

//*********************************MENU SCREENS*******************************//
char alarm_screen(void)//ALARM screen
{
  extern out_struct s_out;
  extern sensor_struct s_sensor;
  extern buttons_struct s_buttons;
  extern settings_struct s_settings;

  if(s_settings.b_en_saund&&s_out.alarm_sound) for(char i=0;i<3;i++){beep(0,200);sys_del_mes(1000);} 
  else sys_del_mes(2000);   
     
  
    if(s_out.alarm_T_oven==1)
    {
        print_string_cyr("NHTDJUF  N(GTXB)","        d       ",1,1);
        print_nomber(s_sensor.lm75a_oven,6,2);
        sys_del_mes(2000);
         }
    
    if(s_out.alarm_T_sauna==1)
    {
        print_string_cyr("NHTDJUF N(CFEYF)","        d       ",1,1);
        print_nomber(s_sensor.lm75a_sauna,6,2);
        sys_del_mes(2000);
         }
    
    if(s_out.alarm_T_sauna_aht10==1)
    {
        print_string_cyr("NHTDJUF N(CFEYF)","FYN     d       ",1,1);
        print_nomber(s_sensor.aht10_temp,6,2);
        sys_del_mes(2000);
           }
    
        if(s_out.alarm_T_smoke==1)
    {
        print_string_cyr("NHTDJUF N(LSV)  ","        d       ",1,1);
        print_nomber(s_sensor.lm75a_smoke,6,2);
        sys_del_mes(2000);
          }
    
            if(s_out.alarm_T_transf==1)
    {
        print_string_cyr("NHTDJUF N(NHFYC)","        d       ",1,1);
        print_nomber(s_sensor.lm75a_transf,6,2);
        sys_del_mes(2000);
          }
    
     if((s_out.alarm_P_max==1)||(s_out.alarm_P_min==1))
    {
        print_string_cyr("NHTDJUF H       ","            RDN",1,1);
        print_string("<>   -",1,2); 
        print_nomber(s_settings.P_limit_min/100,4,2);
        print_nomber(s_settings.P_limit_max/100,8,2);
        sys_del_mes(2000);
     }
    
    return(0);
}

char main_screen(void)//MAIN SCRIIN (DEFAULT SCREEN)
{
  
  extern sensor_struct s_sensor;
  extern buttons_struct s_buttons;
  extern settings_struct s_settings;
  extern settings_struct set_0;
  extern out_struct s_out;
  char local_counter=0;
  
  
  write_wh_byte(0,8|4);// off blinking cursor
  print_string_cyr("Nc=  d h  %    m","Ng=   d Nl=   d",1,1);
  while(1)
  {
  if(s_buttons.b_left) {s_buttons.b_left=0; }
  if(s_buttons.b_up)   {s_buttons.b_up=0; reset_alarms();  } //reset alarms
  if(s_buttons.b_down) {s_buttons.b_down=0; set_0.b_en_sauna_lm75a^=1; write_eeprom_settings(&s_settings,&set_0);}
  if(s_buttons.b_right){s_buttons.b_right=0;beep(0,BEEP_PUSH); return (1); }//EXIT TO 1-st screen
  
  if (local_counter==0)  
   {
    
    if(s_settings.b_en_sauna_lm75a) print_nomber(s_sensor.lm75a_sauna,4,1); 
    else print_nomber(s_sensor.aht10_temp%100,4,1);
    print_nomber(s_sensor.aht10_humid%100,9,1);
    print_nomber(s_sensor.lm75a_oven,4,2);if(s_sensor.lm75a_oven<100) write_wh_byte(1,' ');
    print_nomber(s_sensor.lm75a_smoke,12,2);if(s_sensor.lm75a_smoke<100) write_wh_byte(1,' ');
    print_nomber(s_sensor.timer_min%1000,13,1);
     if(s_sensor.timer_min<100) 
      {
        write_wh_byte(1,' ');
        if(s_sensor.timer_min<10) write_wh_byte(1,' ');
      }
   
   }
  if((s_out.alarm==1)&&(local_counter==200)) return (100); //EXIT to alarm - screen
  local_counter++;
  }
  }
 
char screen_1(void)
{
  extern sensor_struct s_sensor;
  extern buttons_struct s_buttons;
  extern settings_struct s_settings;
  extern out_struct s_out;  
  extern settings_struct set_0;
  int local_counter=0;
  char internal_f=0;
  
  if(s_out.out_1) 
    {
    print_string_cyr("YFUHTD |        ","   min | DRK. ",1,1);// nagrev| xxx , xxx  | vkl. 
    }
       else print_string_cyr("YFUHTD |        ","   min | JNRK. ",1,1);  
  
  while(1)
  {
  if(s_buttons.b_left) //left
   {
   s_buttons.b_left=0; beep(0,BEEP_PUSH); 
   if (!internal_f) return (0); 
    else internal_f=0;
   }
  if(s_buttons.b_up)   //up
   {
   beep(0,BEEP_PUSH);
   s_buttons.b_up=0;
   if (internal_f!=1) 
   {if(s_sensor.timer_min<MAX_TIMER) {s_sensor.timer_min++;} else s_sensor.timer_min=1;} //
    else 
    {
      if(s_out.out_1==0) 
       { 
       s_out.out_1=1;
       print_string_cyr("YFUHTD |        ","   min | DRK. ",1,1);    // nagrev| xxx , xxx  | vkl. 
       PD_ODR&=~BIT_chanal_2; //switch on heater
       set_0.time_limit=s_sensor.timer_min; write_eeprom_settings(&s_settings,&set_0);
       init_tim1(); 
       }
      else 
      {
        s_out.out_1=0;
        print_string_cyr("YFUHTD |        ","   min | JNRK. ",1,1);          // nagrev| xxx , xxx  | otkl.
        PD_ODR|=BIT_chanal_2; //switch off heater
        TIM1_CR1_CEN=0;
      }
    } 
   }
  if(s_buttons.b_down)  //down
   {
   beep(0,BEEP_PUSH);
   s_buttons.b_down=0;
   if (internal_f!=1) {if(s_sensor.timer_min>1) {s_sensor.timer_min--;} else s_sensor.timer_min=MAX_TIMER;}
   else 
     {
     
     if(s_out.out_1==0) 
       { 
       s_out.out_1=1;
       print_string_cyr("YFUHTD |        ","   min | DRK. ",1,1);    // nagrev| xxx , xxx  | vkl. 
       set_0.time_limit=s_sensor.timer_min; write_eeprom_settings(&s_settings,&set_0);
       PD_ODR&=~BIT_chanal_2; //switch on heater
       init_tim1(); 
       }
       else 
       {
        s_out.out_1=0;
        print_string_cyr("YFUHTD |        ","   min | JNRK. ",1,1);          // nagrev| xxx , xxx  | otkl.
        PD_ODR|=BIT_chanal_2; //switch off heater
        TIM1_CR1_CEN=0;
       }
      }
   }
  if(s_buttons.b_right) //right
  {
   s_buttons.b_right=0;beep(0,BEEP_PUSH); 
   if (internal_f) return (2); 
    else internal_f=1;
  }
  if(s_out.alarm==1) return (100); //EXIT to alarm - screen
 
  if ((local_counter&1023)==0)  
   {
   write_wh_byte(0,8|4);// off blinking cursor
   print_nomber(s_sensor.timer_min%1000,1,2);
   if(s_sensor.timer_min<100) 
   {
     write_wh_byte(1,' ');
     if(s_sensor.timer_min<10) write_wh_byte(1,' ');
   }
    if (internal_f==1) set_cursor(10,2); else set_cursor(4,2);
   write_wh_byte(0,8|5);//blinking cursor
   }
  local_counter++;
  }
}

char screen_2_P_U_I(void)
{
  extern out_struct s_out;   
  extern sensor_struct s_sensor;
  extern buttons_struct s_buttons;
  char local_counter=0;
  char v1;
  
  
  write_wh_byte(0,8|4);// off blinking cursor
  print_string("P= .  kW",1,1); 
  print_string("U=   V I=  . A",1,2);

  while(1)
  {
  if(s_buttons.b_left) {s_buttons.b_left=0;beep(0,BEEP_PUSH); return (1); }//EXIT TO 1-st screen
  if(s_buttons.b_up)   {s_buttons.b_up=0;}
  if(s_buttons.b_down) {s_buttons.b_down=0;}
  if(s_buttons.b_right){s_buttons.b_right=0;beep(0,BEEP_PUSH); return (0); }//EXIT TO 3-st screen
  if(s_out.alarm==1) return (100); //EXIT to alarm - screen
  if (local_counter==0)  
   {
    if(s_sensor.adc_1<10) {print_nomber(s_sensor.adc_1,5,2); print_string("  ",3,2); }
     else if(s_sensor.adc_1<100) {print_nomber(s_sensor.adc_1,4,2);print_char(' ',3,2); }
      else print_nomber(s_sensor.adc_1,3,2);
     if(s_sensor.adc_2<10) {print_nomber(s_sensor.adc_2/10,11,2);print_char(' ',10,2);}
     else print_nomber(s_sensor.adc_2/10,10,2);
    print_nomber(s_sensor.adc_2%10,13,2);
    print_nomber(s_sensor.P/100,3,1);
    v1=s_sensor.P%100;
    if(v1<10) {print_char('0',5,1); print_nomber(v1,6,1);}
     else print_nomber(v1,5,1);
   }
  local_counter++;
  }
  }

void write_eeprom_settings(settings_struct* addr_eeprom,settings_struct* data)///save to eeprom
{ asm("sim");
  	if (!((FLASH_IAPSR) & (MASK_FLASH_IAPSR_DUL)))
    {  // unlock EEPROM
       FLASH_DUKR = 0xAE;
       FLASH_DUKR = 0x56;
    }
  asm("rim");
  
       while (!((FLASH_IAPSR) & (MASK_FLASH_IAPSR_DUL))) {;}
       *addr_eeprom=*data;
       FLASH_IAPSR &= ~(MASK_FLASH_IAPSR_DUL); // lock EEPROM
}




__interrupt void time1(void)
{
 extern sensor_struct s_sensor;
 extern out_struct s_out; 
 TIM1_SR1_UIF=0; //flag "0"
 if (s_out.out_1==1)
 {
   if(s_sensor.timer_min>1) s_sensor.timer_min--;
    else
    {
    s_sensor.timer_min--;
    PD_ODR|=BIT_chanal_2; //switch off heater
    TIM1_CR1_CEN=0;
    s_out.out_1=0;
    beep(1,0);
  }
 }
 }

__interrupt void time4(void)
{
 extern buttons_struct s_buttons;
 extern sensor_struct s_sensor;
 extern out_struct s_out;
 extern settings_struct s_settings;
 TIM4_SR_UIF=0; //flag "0"
 buttons();
 if((s_buttons.sys_count)==0) 
   get_aht10_part1();
 

 if((s_buttons.sys_count)==32768)
 { 
 get_aht10_part2(&(s_sensor.aht10_data1));
 }
 
  if((s_buttons.sys_count)==1024) 
  s_sensor.lm75a_sauna=get_lm75a(s_settings.address_lm75a_sauna);
 
  if((s_buttons.sys_count)==2048) 
  s_sensor.lm75a_oven=get_lm75a(s_settings.address_lm75a_oven);  
  
  if((s_buttons.sys_count)==4096) 
  s_sensor.lm75a_smoke=get_lm75a(s_settings.address_lm75a_smoke); 
  
  if((s_buttons.sys_count)==8192) 
  s_sensor.lm75a_transf=get_lm75a(s_settings.address_lm75a_transf); 
  
  if((s_buttons.sys_count)==40960) get_ADC_1();
  if((s_buttons.sys_count)==45056) get_ADC_2();
  if((s_buttons.sys_count)==49152) get_P();
 
  if(s_buttons.sys_count==53248)  get_aht10_part3();

  if(s_buttons.sys_count==53376) 
    get_alarm_data();
  
 s_buttons.sys_count+=128;

}
