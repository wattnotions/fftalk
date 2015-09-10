
 
#include <p30f4011.h>
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
#define button1 _RE8
#define button2 _RD1
#define debug_led _RC14

_FOSC(CSW_FSCM_OFF & FRC_PLL16); // Fosc=16x7.5MHz, Fcy=30MHz
_FWDT(WDT_OFF);                  // Watchdog timer off
_FBORPOR(MCLR_DIS);              // Disable reset pin

//functions  
void setup(void);
void ir_38khz_blink1(void);
void ir_38khz_blink2(void);
void __attribute__((__interrupt__, __auto_psv__)) _INT0Interrupt(void);
void __attribute__((__interrupt__, __auto_psv__)) _INT2Interrupt (void);
void set_phase_delay(void);


//variables
int led1_state,led2_state;
int count;
 
int main(void)
{
    
    setup();

    while(1){
		
	
		while(1){
	
			
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
	_TRISC14 = 0; //debug led pin output
	f_select_dir = 1;
	_TRISD1 = 1; //int0 pin input
	_TRISE8 = 1; //int2 pin input
	
	
	//setup interrups int0 and int2
	IEC0bits.INT0IE = 1;//external int enable
	IEC1bits.INT2IE = 1;
	
	//flags
	IFS1bits.INT2IF = 0;
	IFS0bits.INT0IF = 0;
	
	IPC0bits.INT0IP = 4;  //priority
	IPC1bits.T2IP = 5;
	
	//setup timer interrupts
	_T1IP = 1; 
	_T1IF = 0;            
    _T1IE = 1;
	T1CONbits.TON = 1;      //start timer1
	
	_T2IP = 2; 
	_T2IF = 0;            
    _T2IE = 1;
	T2CONbits.TON = 1;      //start timer2
	
	_T3IP = 3; 
	_T3IF = 0;            
    _T3IE = 1;
	T3CONbits.TON = 1;      //start timer2
	
	
	
	
	
	
	
}

void __attribute__((__interrupt__, __auto_psv__)) _INT0Interrupt(void) {
	IFS0bits.INT0IF = 0;
	count++;
	set_phase_delay();
	debug_led = 1;
	
}

void __attribute__((__interrupt__, __auto_psv__)) _INT2Interrupt (void) {
	IFS1bits.INT2IF = 0;
	count--;
	set_phase_delay();
	debug_led = 0;
}

void set_phase_delay(void) {
	
	
	
	TMR2 = 0;
	TMR3 = 0;
	
	TMR4 = 0;
	t4_flag = 0;
	//while(!t4_flag); // set phase delay
	TMR3 = 0;
	
	t2_flag = 0;   //reset flags
	t3_flag = 0;
	
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{
	_T2IF = 0;
    debug_led = ~ debug_led;
	f1_pin = ~f1_pin;
}

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void)
{
   _T3IF = 0;
   f2_pin = ~f2_pin;
}

