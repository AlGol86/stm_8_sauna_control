//this example is intended for 8 MHz clock frequency
#include "iostm8s103f3.h"
#define ADDRESS_AHT10 0x38
#define ADDRESS_LM75A_0 0x48 ///define ADDRESS_LM75A_2



#define PIN_sda_1     2  //port A
#define PIN_clk_1     3  //port A
#define PIN_sda_2     4  //port B
#define PIN_clk_2     5  //port B

#define BIT_sda_1    (1<<PIN_sda_1)
#define BIT_clk_1    (1<<PIN_clk_1)
#define BIT_sda_2    (1<<PIN_sda_2)
#define BIT_clk_2    (1<<PIN_clk_2)

#define BIT_read    1
#define READ        1
#define BIT_write   0
#define WRITE       0
#define ACK         0 //acknowledge
#define NOT_ACK     1 //not acknowledge


void sys_del_iic(char del_us);
char start_iic_1 (char adr_iic, char read_write_bit);
char start_iic_2 (char adr_iic, char read_write_bit);
char send_byte_1 (char data_byte);
char send_byte_2 (char data_byte);
unsigned char receive_byte_1(char acknowledge);
unsigned char receive_byte_2(char acknowledge);
void stop_iic_1 (void);
void stop_iic_2 (void);
char get_addresses_iic_1(void);
char get_addresses_iic_2(void);
