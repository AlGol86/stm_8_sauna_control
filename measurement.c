#include "iostm8s103f3.h"
#include "measurement.h"
#include "iic.h"
#include "timers_and_buttons.h"
#include "wh1604a.h"
#include "sound.h"

void sys_del_mes(unsigned int del)//del=1ms
{ 
 
  for(unsigned int i=0;i<del;i++)
  {
    for(char j=0;j<150;j++){asm("nop");}
    for(char j=0;j<180;j++){asm("nop");}
  }
  asm("nop");
}

void get_alarm_data(void)
{
extern sensor_struct s_sensor;
extern out_struct s_out;
extern settings_struct s_settings;
static const char  count_lim=4;
if (((s_sensor.lm75a_oven>s_settings.oven_temp_limit) || (s_sensor.lm75a_sauna>s_settings.sauna_temp_limit)  || \
   (s_sensor.lm75a_smoke>s_settings.smoke_temp_limit) || (s_sensor.lm75a_transf>s_settings.transf_temp_limit)|| \
   (s_sensor.P>s_settings.P_limit_max) || (((PD_ODR&BIT_chanal_2)==0)&&(s_sensor.P<s_settings.P_limit_min)))&&\
   (s_out.alarm_counter<count_lim)) s_out.alarm_counter++; 
    else if(s_out.alarm_counter>0)s_out.alarm_counter--;
    
  if (s_out.alarm_counter>(count_lim>>1))
  {
    s_out.alarm_sound=1;
    if (s_sensor.lm75a_oven  > s_settings.oven_temp_limit)   
    {s_out.alarm_T_oven=1; s_out.alarm=1; s_out.out_1=0;  PD_ODR|=BIT_chanal_2;} //switch off heater
  
    if (s_sensor.lm75a_sauna >s_settings.sauna_temp_limit) 
    {s_out.alarm_T_sauna=1; s_out.alarm=1; s_out.out_1=0; PD_ODR|=BIT_chanal_2;} //switch off heater
    
    if (s_sensor.aht10_temp  >s_settings.sauna_aht10_temp_limit) 
    {s_out.alarm_T_sauna_aht10=1; s_out.alarm=1; s_out.out_1=0; PD_ODR|=BIT_chanal_2;} //switch off heater
    
    if (s_sensor.lm75a_smoke  >s_settings.smoke_temp_limit) 
    {s_out.alarm_T_smoke=1; s_out.alarm=1; s_out.out_1=0; PD_ODR|=BIT_chanal_2;} //switch off heater
    
    if (s_sensor.lm75a_transf  >s_settings.transf_temp_limit) 
    {s_out.alarm_T_transf=1; s_out.alarm=1; s_out.out_1=0; PD_ODR|=BIT_chanal_2;} //switch off heater
    
    if (s_sensor.P  >s_settings.P_limit_max) 
    {s_out.alarm_P_max=1; s_out.alarm=1; s_out.out_1=0; PD_ODR|=BIT_chanal_2;} //switch off heater
    
     if ((s_sensor.P  <s_settings.P_limit_max)&&((PD_ODR&BIT_chanal_2)==0))
    {s_out.alarm_P_min=1; s_out.alarm=1; s_out.out_1=0; PD_ODR|=BIT_chanal_2;} //switch off heater
   }
  
  else 
    s_out.alarm_sound=0;
}

void reset_alarms(void)
{
  extern out_struct s_out;
  s_out.alarm=0;
  s_out.alarm_P_max=0;
  s_out.alarm_P_min=0;
  s_out.alarm_T_oven=0;
  s_out.alarm_T_sauna=0;
  s_out.alarm_T_sauna_aht10=0;
  s_out.alarm_T_smoke=0;
  s_out.alarm_T_transf=0;
}


void get_ADC_1(void)
{
extern sensor_struct s_sensor;
char h=0;
char l=0;
PD_DDR&=~BIT__ADC_chanal_1;
PD_CR1&=~BIT__ADC_chanal_1;
PD_CR2&=~BIT__ADC_chanal_1;
  //ADC_CR2_ALIGN=1;
ADC_CR1_CONT=0;
ADC_CSR_CH=3;
ADC_CR1_ADON=1;
ADC_CR1_ADON=1; //dubble 'ADON" for switch on ADC
while(ADC_CSR_EOC==0)
  {asm("nop");}

ADC_CSR_EOC=0;
h=ADC_DRH;
l=ADC_DRL;
s_sensor.adc_1=ADC_compensation((h<<2)|l); 
}

void get_ADC_2(void)
{
extern sensor_struct s_sensor; 
char h=0;
char l=0;
PD_DDR&=~BIT__ADC_chanal_2;
PD_CR1&=~BIT__ADC_chanal_2;
PD_CR2&=~BIT__ADC_chanal_2;
  //ADC_CR2_ALIGN=1;
ADC_CR1_CONT=0;
ADC_CSR_CH=4;
ADC_CR1_ADON=1;
ADC_CR1_ADON=1; //dubble 'ADON" for switch on ADC
while(ADC_CSR_EOC==0)
  {asm("nop");}

ADC_CSR_EOC=0;
h=ADC_DRH;
l=ADC_DRL;
s_sensor.adc_2=ADC_compensation((h<<2)|l);  
}


void get_P(void)
{
extern sensor_struct s_sensor; 
s_sensor.P=(s_sensor.adc_2*s_sensor.adc_1)/100;
}

int ADC_compensation(int value_ADC) //compensation diode voltage drop 
{
 if (value_ADC!=0) 
 {
 value_ADC+=20;
 value_ADC*=17;
 value_ADC/=25;
 }
 return (value_ADC);
}

char get_lm75a(char adr)
{
  char t=0xFF;
  start_iic_1 (adr, READ);
  t=receive_byte_1(1);
  receive_byte_1(0);
  stop_iic_1 ();
  return(t); 
}

void init_aht10(void)
{ 
  sys_del_mes(100);//20m
  start_iic_2 (ADDRESS_AHT10, WRITE);
  send_byte_2(0xe1);//init
  stop_iic_2();
  sys_del_mes(100);//20m
 
  start_iic_2 (ADDRESS_AHT10, WRITE);
  send_byte_2(0xba);//soft reset 20m
  stop_iic_2();
  sys_del_mes(100);//20m
}
void get_aht10_part1(void )
{
  start_iic_2 (ADDRESS_AHT10, WRITE);
  send_byte_2(0xac);// measure 75m
  stop_iic_2();
} 

void get_aht10_part2( unsigned char* data_n )
{
  start_iic_2 (ADDRESS_AHT10, READ);
  receive_byte_2 (1);
  for (char i=0;i<4;i++)
   *(data_n+i)=receive_byte_2 (1);
  *(data_n+4)=receive_byte_2 (0);
  stop_iic_2();
}
   

void get_aht10_part3(void)
{  
extern sensor_struct s_sensor;
long int t=0xFFFF;
long int h=0xFFFF;  



h=s_sensor.aht10_data1;
h<<=8;
h|=s_sensor.aht10_data2;
h<<=8;
h|=s_sensor.aht10_data3;
h>>=4;
h*=100;
h>>=20;

t=(s_sensor.aht10_data3&0x0f);
t<<=8;
t|=s_sensor.aht10_data4;
t<<=8;
t|=s_sensor.aht10_data5;
t*=25;
t>>=17;
t-=50;
 

s_sensor.aht10_temp=t;
s_sensor.aht10_humid=h;
  
}


void write_eeprom(char* addr_eeprom,char data)///save to eeprom
{ asm("sim");
  	if (!((FLASH_IAPSR) & (MASK_FLASH_IAPSR_DUL)))
    {  // unlock EEPROM
       FLASH_DUKR = 0xAE;
       FLASH_DUKR = 0x56;
    }
  asm("rim");
       while (!((FLASH_IAPSR) & (MASK_FLASH_IAPSR_DUL))) {;}
       *addr_eeprom=data;
       FLASH_IAPSR &= ~(MASK_FLASH_IAPSR_DUL); // lock EEPROM
}

