#include "iostm8s103f3.h"
#include "sound.h"
#include "timers_and_buttons.h"


void beep( char type_sound, char continuance)
{ 
 int count_note=0;
 char repeat_count_note=0;
  int freq=0;
  int us=0;
  static const char mario_sound[2][260]=\
  { {m_5,m_5,pa,m_5,pa, d_5,m_5,s_5,pa,s_4, pa,d_5,s_4,pa,m_4,    l_4,i_4,ib_4,l_4,s_4, 
   m_5,s_5,l_5,f_5,s_5, pa,m_5,d_5,r_5,i_4, pa,s_5,sb_5,f_5,mb_5, m_5,pa,lb_4,l_4,d_5, 
   pa,l_4,d_5,r_5,     pa,s_5,sb_5,f_5,mb_5, m_5,pa,d_6,d_6,d_6, pa,s_5,sb_5,f_5,mb_5, 
   m_5,pa,lb_4,l_4,d_5, pa,l_4,d_5,r_5,pa, mb_5,pa,r_5,d_5,pa, d_5,d_5,d_5,pa,d_5, 
   r_5,m_5,d_5,l_4,s_4, d_5,d_5,d_5,pa,d_5, r_5,m_5,pa,d_5,d_5, d_5,pa,d_5,r_5,m_5,
   d_5,l_4,s_4,m_5,m_5, pa,m_5,pa,d_5,m_5, s_5,pa,s_4,pa,d_5, s_4,pa,m_4,
   l_4,i_4,ib_4,l_4,s_4,m_5,s_5,l_5,f_5,s_5,pa,m_5,d_5,r_5,i_4,m_5,d_5,s_4,pa,lb_4,l_4,f_5,f_5,l_4,
   i_4,l_5,l_5,l_5,s_5,f_5,m_5,d_5,l_4,s_4,m_5,d_5,s_4,pa,lb_4,l_4,f_5,f_5,l_4,i_4,f_5,f_5,f_5,m_5,r_5,
   d_5,m_4,m_4,d_4,d_5,d_5,d_5,pa,d_5,r_5,m_5,d_5,l_4,s_4,d_5,d_5,d_5,pa,d_5,r_5,m_5,pa,
   d_5,d_5,d_5,pa,d_5,r_5,m_5,d_5,l_4,s_4,m_5,m_5,pa,m_5,pa,d_5,m_5,s_5,pa,s_4,pa,m_5,d_5,s_4,pa,lb_4,
   l_4,f_5,f_5,l_4,i_4,l_5,l_5,l_5,s_5,f_5,m_5,d_5,l_4,s_4,m_5,d_5,s_4,pa,lb_4,l_4,f_5,f_5,l_4, 
   i_4,f_5,f_5,f_5,m_5,r_5,d_5,m_4,m_4,d_4,d_5,s_4,m_4,l_4,i_4,l_4,lb_4,ib_4,l_4,s_4,r_4,m_4  },  

    { 2,2,2,2,2,2,4,4,4,4,4,6,2,4,6,4,4,2,4, 3,3,3,4,2,2,2, 4,2,2,4,4,2,2,2, 4,2,2,2,2,2,2,2,2,2, //0...43 notes
    4,2,2,2,4,2,2,4,2,8,4,2,2,2,4,2,2,2,2,2,2,2,2,2,4,4,2,6,8,8,//44...73 notes
    2,4,2,2,2,4,2,4,2,8,2,4,2,2,2,2,2,10,  //74...91 notes
    2,4,2,2,2,4,2,4,2,8,  2,2,2,2,2,2,4,4,4,4, 4,6,2,4,6,//116
    4,4,2,4,3,3,3,4,2,2,  2,4,2,2,6,2,4,2,4,4, 2,4,2,8, //140
    3,3,3,3,3,3,2,4,2,8,  2,4,2,4,4,2,4,2,8,2, 4,2,3,3,3,//165
    2,4,2,8,2,4,2,2,2,4,  2,4,2,8,2,4,2,2,2,2, 2,10,    //187
    2,4,2,2,2,4,2,4,2,8,  2,2,2,2,2,2,4,4,4,4, 4,2,4,2,4,4, //213
    2,4,2,8,3,3,3,3,3,3,  2,4,2,8,2,4,2,4,4,2, 4,2,8, //236
    2,4,2,3,3,3,2,4,2,8,  8,8,4,3,3,3,3,3,3,2, 2,10}}; //258

  TIM4_IER_UIE=0;
  while(count_note<500)
  {
  if((count_note==30)&&(repeat_count_note ==0)) {count_note=11;repeat_count_note=1;}
  
  if((count_note==74)&&(repeat_count_note==1)) {count_note=30;repeat_count_note=2;}
  if((count_note==132)&&(repeat_count_note==2)) {count_note=113;repeat_count_note=3;}
  if((count_note==170)&&(repeat_count_note==3)) {count_note=132;repeat_count_note=4;}
  if(count_note==259)
  {count_note=499;freq=100;}
else
  if(type_sound==0) {count_note=499;freq=503;}
else  
  switch (mario_sound[0][count_note])
  {
case 1: {freq=261; break;}
case 2: {freq=277; break;}
case 3: {freq=293; break;}
case 4: {freq=311; break;}
case 5: {freq=329; break;} 
case 6: {freq=349; break;}
case 7: {freq=369; break;}
case 8: {freq=391; break;}
case 9: {freq=415; break;}
case 10: {freq=440; break;}
case 11: {freq=466; break;}
case 12: {freq=493; break;}
case 13: {freq=523; break;}
case 14: {freq=554; break;}
case 15: {freq=587; break;} 
case 16: {freq=622; break;}
case 17: {freq=659; break;}
case 18: {freq=698; break;}
case 19: {freq=739; break;}
case 20: {freq=783; break;}
case 21: {freq=830; break;}
case 22: {freq=880; break;}
case 23: {freq=932; break;}
case 24: {freq=987; break;}
case 25: {freq=1046; break;}
case 30: {freq=100; break;} 
  }
  
  us=1000000/freq;
  
        asm("rim");
        TIM2_IER_UIE=1;
	TIM2_IER_CC1IE=1;
        TIM2_PSCR=1; //15 
        TIM2_ARRH=(us>>8); //0xff
        TIM2_ARRL=us; //0xff
        TIM2_CCR1H=0;//capture/compare registr high bits
	TIM2_CCR1L=200;//(unsigned int)(55552)/freq;
	TIM2_CNTRH=0;
	TIM2_CNTRL=0;
        if (freq!=100) TIM2_CR1_CEN=1;

       ////////////////////////////////////////////////
        
        if(type_sound==0) 
        for (unsigned int i=1;i<(256*continuance);i++){}
        else
          for (int i=1;i<4000;i++){for (int j=1;j<mario_sound[1][count_note];j++){}}
       TIM2_CR1_CEN=0;
       PD_ODR|=BIT_chanal_4;//off 
       for (int i=1;i<7000;i++){}
       count_note++;
  } 
 TIM4_IER_UIE=1;
 }

__interrupt void time2(void)
{
 TIM2_SR1_UIF=0; //flag "0"
 PD_ODR&=~BIT_chanal_4;//on 
}

__interrupt void time2cc(void)
{TIM2_SR1_CC1IF=0;
PD_ODR|=BIT_chanal_4;//off 
}
