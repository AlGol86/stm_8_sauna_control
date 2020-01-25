#define PIN_ADC_chanal_1     2  //port D
#define PIN_ADC_chanal_2     3  //port D

#define BIT__ADC_chanal_1    (1<<PIN_ADC_chanal_1 )
#define BIT__ADC_chanal_2    (1<<PIN_ADC_chanal_2 )

#define eeprom_alarm ((char*)0x4100)

typedef struct
{
 unsigned char lm75a_sauna  ;
 unsigned char lm75a_transf ;
 unsigned char lm75a_oven   ;
 unsigned char lm75a_smoke  ;
 unsigned char aht10_temp   ;
 unsigned char aht10_humid  ; 
 unsigned int adc_1         ;
 unsigned int adc_2         ;
 unsigned long int P             ;
 unsigned int  timer_min    ;
 unsigned char aht10_data1  ;
 unsigned char aht10_data2  ;
 unsigned char aht10_data3  ;
 unsigned char aht10_data4  ;
 unsigned char aht10_data5  ;
} sensor_struct;

void sys_del_mes(unsigned int del);
void get_alarm_data(void);
void reset_alarms(void);
void get_ADC_1(void);
void get_ADC_2(void);
void get_P(void);
int ADC_compensation(int value_ADC); //compensation diode voltage drop 
char get_lm75a(char adr);
void init_aht10(void);
void get_aht10_part1(void );
void get_aht10_part2( unsigned char* data_n );
void get_aht10_part3(void );
void write_eeprom(char* addr_eeprom,char data);///save to eeprom
