#include "iostm8s103f3.h"
#include "wh1604a.h"

void del_WH(unsigned int del)
{
  for(int j=0;j<=del;j++){}
}

void init1604(void)
{
  PC_ODR&=~(MASK_db);//b00001111
  PC_DDR|=(MASK_db); //b11110000
  PC_CR1|=(MASK_db); //b11110000
  PC_CR2|=(MASK_db); //b11110000
  PC_ODR&=~BIT_rs;
  PA_ODR&=~BIT_en;
  PC_DDR|=BIT_rs; 
  PA_DDR|=BIT_en;
  PC_CR1|=BIT_rs; 
  PA_CR1|=BIT_en;   
  PC_CR2&=~BIT_rs; 
  PA_CR2&=~BIT_en;
  del_WH(30000);
  write_wh_byte(0,8);
  write_wh(0,0x30); //arg: char rs, char db7_db4
  del_WH(50);
  write_wh_byte(0,32|N_F_code<<2);
  write_wh_byte(0,32|N_F_code<<2);
  write_wh_byte(0,8|(D_C_B_code));
  clear();
  write_wh_byte(0,4|ID_SH_code);
  clear();
  set_cursor(1,1);
}

void clear(void)
{
  write_wh_byte(0,1);
  del_WH(2000);
}

void set_cursor(char x,char y)
{
  x--;
  switch(y)
{
case 1: {write_wh_byte(0,0x80|0x0|x);break;} 
case 2: {write_wh_byte(0,0x80|0x40|x);break;}
case 3: {write_wh_byte(0,0x80|0x10|x);break;}
case 4: {write_wh_byte(0,0x80|0x50|x);break;}
}
}

void write_wh(char rs,char db7_db4)// rs=0-instruction; rs=1-data (write half of byte)
{
  PC_ODR&=~MASK_db;
  PC_ODR&=~BIT_rs;
  PC_ODR|=rs<<PIN_rs;
  PA_ODR|=BIT_en;
  PC_ODR|=(db7_db4&0xF0);
  asm("nop");
  PA_ODR&=~BIT_en;
}

void write_wh_byte(char rs,char db7_db0)
{
  write_wh(rs,db7_db0);
  write_wh(rs,db7_db0<<4);
  del_WH(20);
}

void print_char(char ch,char x, char y) 
{
  set_cursor(x,y);
  write_wh_byte(1,ch);
}

void print_string(char* pointer, char x, char y) 
{
set_cursor(x,y);
while (*pointer!=0)
{
write_wh_byte(1,*pointer);
pointer++;
}
}

void print_string_cyr(char* pointer_1,char* pointer_2,char x_1,char x_2) // if LED don't provide CYR letters
{
char count=0;//no CGRAM
while (*pointer_1!=0)
{
count=print_symb(*pointer_1,x_1,1,count);
pointer_1++;
x_1++;
}

while (*pointer_2!=0)
{
count=print_symb(*pointer_2,x_2,2,count);
pointer_2++;
x_2++;
}

}


char print_nomber(int n, char x,char y ) //n=0..999
{ 
 
 set_cursor(x,y);
 if (n>99) 
 { 
   write_wh_byte(1,0x30|(n/100)); n%=100;
   write_wh_byte(1,0x30|(n/10)); n%=10;
   write_wh_byte(1,0x30|n);
   x+=3;
 }
 else
 {
 if (n>9) {write_wh_byte(1,0x30|(n/10)); n%=10;x++;}
 write_wh_byte(1,0x30|n);x++;
 }
 return(x);
 }

char print_symb(char s,char x,char y,char n_CGRAM) //if LED don't provide CYR letters
{               //(***x xxxx bits)
  char ch[11][8]={//{6,9,9,6,0,0,0,0},//character#0 - degree 
                              {0x18,0x18,0x00,0x07,0x04,0x04,0x07,0},//character#0 - degree+c
                              {0x1F,0x11,0x11,0x11,0x11,0x11,0x11,0},////character#1 - Pe_cyr
                              {0x11,0x11,0x13,0x15,0x19,0x11,0x11,0},////character#2 - I_cyr
                              {0x0e,0x0a,0x0a,0x0a,0x0a,0x1f,0x11,0},////character#3 - D_cyr
                              {0x11,0x11,0x11,0x1d,0x15,0x15,0x1d,0},////character#4 - Y_cyr
                              {0x11,0x11,0x11,0x1f,0x01,0x01,0x01,0},////character#5 - 4_cyr
                              {0x10,0x10,0x10,0x1f,0x11,0x11,0x1f,0},////character#6 - Be_cyr
                       //       {0x17,0x15,0x15,0x1d,0x15,0x15,0x17,0},////character# - yu_cyr
                              {0x00,0x00,0x1e,0x12,0x12,0x12,0x12,0},////character#7 - pe_small_cyr 
                              {0x00,0x00,0x0e,0x0a,0x0a,0x1f,0x11,0},////character#8 - de_small_cyr
                              {0x1f,0x10,0x10,0x10,0x10,0x10,0x10,0},////character#9 - Ge_cyr
                              {0x18,0x14,0x12,0x11,0x11,0x11,0x11,0},////character#10 - eL_cyr
               
  };

   switch(s)
  {
  case 'd' :{  n_CGRAM=print_by_CGRAM(n_CGRAM,&ch[0][0],x,y);break;}
  case 'G' :{  n_CGRAM=print_by_CGRAM(n_CGRAM,&ch[1][0],x,y);break;}
  case 'B' :{  n_CGRAM=print_by_CGRAM(n_CGRAM,&ch[2][0],x,y);break;}
  case 'L' :{  n_CGRAM=print_by_CGRAM(n_CGRAM,&ch[3][0],x,y);break;}
  case 'S' :{  n_CGRAM=print_by_CGRAM(n_CGRAM,&ch[4][0],x,y);break;}
  case 'X' :{  n_CGRAM=print_by_CGRAM(n_CGRAM,&ch[5][0],x,y);break;}
  case 'M' :{  n_CGRAM=print_by_CGRAM(n_CGRAM,&ch[6][0],x,y);break;}
  case 'g' :{  n_CGRAM=print_by_CGRAM(n_CGRAM,&ch[7][0],x,y);break;}
  case 'l' :{  n_CGRAM=print_by_CGRAM(n_CGRAM,&ch[8][0],x,y);break;}
  case 'U' :{  n_CGRAM=print_by_CGRAM(n_CGRAM,&ch[9][0],x,y);break;}
  case 'K' :{  n_CGRAM=print_by_CGRAM(n_CGRAM,&ch[10][0],x,y);break;}


  ///////////////////////////////////////////////////////////////////////////
  case 'F' :{print_char(0x41,x,y);break;} 
  case 'D' :{print_char(0x42,x,y);break;} 
  case 'T' :{print_char(0x45,x,y);break;}
  case 'R' :{print_char(0x4b,x,y);break;}
  case 'V' :{print_char(0x4d,x,y);break;}
  case 'Y' :{print_char(0x48,x,y);break;}
  case 'J' :{print_char(0x4f,x,y);break;}
  case 'H' :{print_char(0x50,x,y);break;}
 // case 'C' :{print_char(0x43,x,y);break;}
  case 'N' :{print_char(0x54,x,y);break;}
  case 'E' :{print_char(0x79,x,y);break;}
  case '[' :{print_char(0x58,x,y);break;}
  default  :{print_char(s,x,y);   break;}

  }

return n_CGRAM;
}
    

 void set_CGRAM(char n_CGRAM,char* data )
  {
      for (char j=0;j<8;j++) //no.string pix in character
        {  
        write_wh_byte(0,64|(n_CGRAM<<3)|j); //set address CGRAM_segment
        write_wh_byte(1,*(data+j));         //send data[0..7] to CGRAM_segment
        }
  }

char print_by_CGRAM(char n_CGRAM,char* data,char x, char y)
{
set_CGRAM(n_CGRAM,data);
set_cursor(x,y);
write_wh_byte(1,n_CGRAM);
n_CGRAM++;
return (n_CGRAM);
}
               



