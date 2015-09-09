
 
#include <xc.h>
#include <libpic30.h>
 
#define f2_pin _RD0
#define f1_pin _RD2
#define f2_pin_dir _TRISD0
#define f1_pin_dir _TRISD2
#define f_select _RD1
#define f_select_dir _TRISD1
#define f_select_led _RD3
#define f_select_led_dir _TRISD3
#define t1_flag IFS0bits.T1IF
#define t2_flag IFS0bits.T2IF
#define t3_flag IFS0bits.T3IF
#define t4_flag IFS1bits.T4IF
#define button1 _

_FOSC(CSW_FSCM_OFF & FRC_PLL16);    //   Set up for Internal Fast RC
_FWDT(WDT_OFF);                  	//Turn off the Watch-Dog Timer.  
_FBORPOR(MCLR_EN & PWRT_OFF);   	// Enable MCLR reset pin and turn off the power-up timers. 
_FGS(CODE_PROT_OFF);



//functions  
void setup(void);
void ir_38khz_blink1(void);
void ir_38khz_blink2(void);


//variables
int led1_state,led2_state;
int count;
 
int main(void)
{
    
    setup();
	
    while(1){
		
	
		while(1){
	
			while(t1_flag == 0);
			if(t2_flag) led1_state = ~led1_state; t2_flag = 0;
			if(led1_state) f2_pin = ~f2_pin; //500
			
			if(t3_flag) led2_state = ~led2_state; t3_flag = 0;
			if(led2_state) f1_pin = ~f1_pin; //300
			
			t1_flag = 0;
		}	
		
	}
}

void setup (void) {
	
	//setup timers
	//timer 1 is 38khz carrier frequency
	T1CONbits.TON = 1;      //start timer1
	T1CONbits.TCKPS = 0b01; // 1:8 prescale
	T1CONbits.TCS = 0  ;     //internal clock
	
	//timer 2 is led1 frequency
	T2CONbits.TON = 1;      //start timer2
	T2CONbits.TCKPS = 0b01; // 1:8 prescale
	T2CONbits.TCS = 0 ;    //internal clock
	
	//timer 3 is led2 frequency
	T3CONbits.TON = 1;      //start timer3
	T3CONbits.TCKPS = 0b01; // 1:8 prescale
	T3CONbits.TCS = 0 ;    //internal clock
	
	//timer 4 sets the phase delay
	T4CONbits.TON = 1;      //start timer4
	T4CONbits.TCKPS = 0b01; // 1:8 prescale
	T4CONbits.TCS = 0 ;    //internal clock
	
	
	PR1 = 48; //38khz
	PR2 = 4863;//400hz 
	PR3 = 4095; //475hz
	
	//setup pins
	f1_pin_dir = 0;
	f2_pin_dir = 0;
	f_select_led_dir = 0;
	
	f_select_dir = 1;
	
	//setup interrups int0 and int2
	SRbits.IPL = 6; //cpu priority
	IEC0bits.INT0IE = 1;//external int enable
	IEC1bits.INT2IE = 1;
	
	//flags
	IFS1bits.INT2IF = 0;
	IFS0bits.INT0IF = 0;
	
	IPC0bits.INT0IP = 3;  //priority
	IPC1bits.T2IP = 4;
	
	
	
	
	
	
	
}

void _INT0Interrupt () {
	count++;
}

void _INT1Interrupt () {
	count--;
}

void set_phase_delay(void) {
	
	
	
	TMR2 = 0;
	
	TMR4 = 0;
	t4_flag = 0;
	while(!t4_flag); // set phase delay
	TMR3 = 0;
	
	t2_flag = 0;   //reset flags
	t3_flag = 0;
	
}

