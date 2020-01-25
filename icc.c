//this example is intended for 8 MHz clock frequency
#include "iostm8s103f3.h"
#include "iic.h"

char get_addresses_iic_1(void)
{
  char count_addr=0;
  for (char i=0x01;i<0x7F;i++)
  {
   if(!start_iic_1(i,WRITE))
   {stop_iic_1();
   count_addr++;}

  }
  return count_addr;
}

char get_addresses_iic_2( void )
{
  char count_addr=0;
  for (char i=0x01;i<0x7F;i++)
  {
   if(!start_iic_2(i,WRITE))
   {stop_iic_2();
   count_addr++;}
  }
  return count_addr;
}

void sys_del_iic(char del_us)	
{
char i;
for(i=0;i<del_us;i++)
{asm("nop");}
}

char start_iic_1 (char adr_iic, char read_write_bit)
{
	//configuration pins (sda,clk) 
	PA_CR2&=~BIT_clk_1; //PA_clk_pin  - low speed (interrupt disabled)
	PA_CR1&=~BIT_clk_1; //PA_clk_pin  - open drain /float in inp. direct
	PA_ODR|=BIT_clk_1; //PA_clk_pin  - 1 (CLOCK)
	PA_DDR|=BIT_clk_1; 
	PA_CR2&=~BIT_sda_1; //PA_sda_pin  - low speed (interrupt disabled)
	PA_CR1&=~BIT_sda_1; //PA_sda_pin - open drain /float in inp. direct
	PA_ODR|=BIT_sda_1; //PA_sda_pin  - 1 (SDA)
	PA_DDR|=BIT_sda_1;
                	
	//(SDA) "START!!!!" 
	PA_ODR&=~BIT_sda_1; //set sda=0
	asm("nop");//>0.1us
	return send_byte_1((adr_iic<<1)+read_write_bit);
}	

char start_iic_2 (char adr_iic, char read_write_bit)
{
	//configuration pins (sda,clk) 
	PB_CR2&=~BIT_clk_2; //PA_clk_pin  - low speed (interrupt disabled)
	PB_CR1&=~BIT_clk_2; //PA_clk_pin  - open drain /float in inp. direct
	PB_ODR|=BIT_clk_2; //PA_clk_pin  - 1 (CLOCK)
	PB_DDR|=BIT_clk_2; 
	PB_CR2&=~BIT_sda_2; //PA_sda_pin  - low speed (interrupt disabled)
	PB_CR1&=~BIT_sda_2; //PA_sda_pin - open drain /float in inp. direct
	PB_ODR|=BIT_sda_2; //PA_sda_pin  - 1 (SDA)
	PB_DDR|=BIT_sda_2;
	//(SDA) "START!!!!" 
	PB_ODR&=~BIT_sda_2; //set sda=0
	asm("nop");//>0.1us
	return send_byte_2((adr_iic<<1)+read_write_bit);
}	
	
char send_byte_1 (char data_byte)
{
	//send start byte
	char i;
	for(i=8;i>0;i--)
	 {                                   
	 PA_ODR&=~BIT_clk_1;//clock=0
         PA_ODR&=~BIT_sda_1;//set data=0
	 sys_del_iic(2);//>2us
	 PA_ODR|=((data_byte>>(i-1))&1)<<PIN_sda_1;//set data
	 sys_del_iic(1); //>0.1us
	 PA_ODR|=BIT_clk_1;//clock=1
	 sys_del_iic(1);//>1us
	 }
	// assert acknowledge
	PA_ODR&=~BIT_clk_1;	 //clock=0
	sys_del_iic(2);//>2us
	PA_ODR|=BIT_sda_1;       //set data=1
	sys_del_iic(1);//>0.1us
	PA_ODR|=BIT_clk_1;	//clock=1
	 sys_del_iic(1);//>1us
	if ((PA_IDR&BIT_sda_1)==0) 
	return ACK; 
	else 
	return NOT_ACK;
}

char send_byte_2 (char data_byte)
{
	//send start byte
	char i;
	for(i=8;i>0;i--)
	 {                                   
	 PB_ODR&=~BIT_clk_2;//clock=0
         PB_ODR&=~BIT_sda_2;//set data=0
	 sys_del_iic(2);//>2us
	 PB_ODR|=((data_byte>>(i-1))&1)<<PIN_sda_2;//set data
	 sys_del_iic(1); //>0.1us
	 PB_ODR|=BIT_clk_2;//clock=1
	 sys_del_iic(1);//>1us
	 }
	// assert acknowledge
	PB_ODR&=~BIT_clk_2;	//clock=0
	sys_del_iic(2);//>2us
	PB_ODR|=BIT_sda_2;      //set data=1
	sys_del_iic(1);//>0.1us
	PB_ODR|=BIT_clk_2;	//clock=1
	 sys_del_iic(1);//>1us
	if ((PB_IDR&BIT_sda_2)==0) 
	return ACK; 
	else 
	return NOT_ACK;
}

unsigned char receive_byte_1(char acknowledge)
{
	//send start byte
	char i;
	char receive_b=0;
	for(i=8;i>0;i--)
	 {
	 PA_ODR&=~BIT_clk_1;	//clock=0
	 sys_del_iic(4);//>5us
	 PA_ODR|=BIT_clk_1;	//clock=1
	 sys_del_iic(4);//>5us
	 PA_DDR&=~BIT_sda_1;
	 receive_b|=(((PA_IDR)&BIT_sda_1)>>PIN_sda_1)<<(i-1);
	 PA_DDR|=BIT_sda_1;
	 }
	// send acknowledge
	PA_ODR&=~BIT_clk_1;	//clock=0
	if(acknowledge) 
          PA_ODR&=~BIT_sda_1;     //set data=0	
	sys_del_iic(2);//>2us
	PA_ODR|=BIT_clk_1;	//clock=1
	sys_del_iic(2);//>1us
	PA_ODR&=~BIT_clk_1;	//clock=0
	PA_ODR|=BIT_sda_1; 	
        
	return receive_b;
}	
	
unsigned char receive_byte_2 (char acknowledge)
{
	//send start byte
	char i;
	char receive_b=0;
	for(i=8;i>0;i--)
	 {
	 PB_ODR&=~BIT_clk_2;	//clock=0
	 sys_del_iic(4);//>5us
	 PB_ODR|=BIT_clk_2;	//clock=1
	 sys_del_iic(4);//>5us
	 PB_DDR&=~BIT_sda_2;
	 receive_b|=(((PB_IDR)&BIT_sda_2)>>PIN_sda_2)<<(i-1);
	 PB_DDR|=BIT_sda_2;
	 }
	// send acknowledge
	PB_ODR&=~BIT_clk_2;	//clock=0
	if(acknowledge) 
          PB_ODR&=~BIT_sda_2;     //set data=0	
	sys_del_iic(2);//>2us
	PB_ODR|=BIT_clk_2;	//clock=1
	sys_del_iic(2);//>1us
	PB_ODR&=~BIT_clk_2;	//clock=0
	PB_ODR|=BIT_sda_2; 
	
	return receive_b;
}	

void stop_iic_1 (void)
{	 
  PA_ODR&=~BIT_clk_1;	          //clock=0
	sys_del_iic(2);//>2us
	PA_ODR&=~BIT_sda_1; 	  //set sda=0
	asm("nop"); //>0.1us
	PA_ODR|=BIT_clk_1;	 //clock=1
	asm("nop");//>0.1us
	PA_ODR|=BIT_sda_1; 	//set sda=1
}

void stop_iic_2 (void)
{	 
  PB_ODR&=~BIT_clk_2;		//clock=0
	sys_del_iic(2);//>2us
	PB_ODR&=~BIT_sda_2; 	//set sda=0
	asm("nop"); //>0.1us
	PB_ODR|=BIT_clk_2;	//clock=1
	asm("nop");//>0.1us
	PB_ODR|=BIT_sda_2; 	//set sda=1
}
