#include "iostm8s103f3.h"
#include "timers_and_buttons.h"

void init_ULN2803A(void)
{
    extern settings_struct s_settings;
    extern out_struct s_out;  
    //configuration pins  
	PD_CR2&=~BIT_chanal_1; //- low speed (interrupt disabled)
        PD_CR2&=~BIT_chanal_2; //- low speed (interrupt disabled)
        PD_CR2&=~BIT_chanal_3; //- low speed (interrupt disabled)
        PD_CR2&=~BIT_chanal_4; //- low speed (interrupt disabled)
	PD_CR1|=BIT_chanal_1; //  - push-pull
        PD_CR1|=BIT_chanal_2; //  - push-pull
        PD_CR1|=BIT_chanal_3; //  - push-pull
        PD_CR1|=BIT_chanal_4; //  - push-pull        
        PD_ODR|=BIT_chanal_1; //  - "1"
        PD_ODR|=BIT_chanal_2; //  - "1"
        PD_ODR|=BIT_chanal_3; //  - "1"
        PD_ODR|=BIT_chanal_4; //  - "1" 
        PD_DDR|=BIT_chanal_1;
        PD_DDR|=BIT_chanal_2;
        PD_DDR|=BIT_chanal_3;
	PD_DDR|=BIT_chanal_4;
        
 s_out.alarm=0;
 s_out.out_1=0;
 s_out.out_2=0; 
        
}
