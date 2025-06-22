/*
 * File:   ulrasonicsensor.c
 * Author: admin
 *
 * Created on 3 June, 2025, 2:58 PM
 */


// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
#include <xc.h>
#define _XTAL_FREQ 4000000
signed long int time=0;
unsigned int distance=0;
int timercount=1;
void delay(unsigned int a)
{
    while(a--);
}

void enable()
{
    RE1=0;
    delay(10);
    RE1=1;
    delay(10);
}
void data(int r,char c)
{
    RE0=r;
    PORTD=c;
    enable();
}
void string_display(char *c)
{
   
    for(int i=0;c[i]!='\0';i++)
    {
        data(1,c[i]);
    }
}
void timer()
{
    while(timercount!=0){
    if(T0IF==1)
    {
        timercount--;
        TMR0=251;
        T0IF=0;
    }
    }timercount=1;
}
void display(int a)
{
//    a=49;
   
    
    int d1=(a%10);
    int d2=(a/10)%10;
    int d3=(a/100)%10;
    data(0,0x8A);
    data(1,d3+'0');
    data(1,d2+'0');
    data(1,d1+'0');
}
void main(void) {
    PORTC=0X00;
    TRISC=0X01;
    PORTD=TRISD=0X00; 
    PORTE=TRISE=0X00;
    ANSEL=ANSELH=0x00;
    OPTION_REG=0X87;
    T1CON=0X31;
    TMR0=251;
    TMR1=0;
    data(0,0x38);
    data(0,0x0e);
    data(0,0x80);
    string_display("distance=");
    while(1)
    {
       
        RC1=1;
        __delay_us(10); //timer();
        RC1=0;
        //waits for echo to go high
        while(!RC0);  //checks for if got triggered 0 if 1 breaks
        TMR1H = 0;
        TMR1L = 0;
        TMR1ON=1;
        TMR1=0;
        //waits for echo to go low
        while(RC0==1); // gives 1 until pulses are incoming and 0 if trigger ends
           TMR1ON=0;   
        
        
        time = (TMR1H << 8) | TMR1L;
        //distance=(time*2)/58; //(0.017*time)*10;    // speed in cm/sec 34300 cm/s
        distance = (time * 137) / 1000; 
//        distance =49;
        display(distance);
        if(distance<=3)
        {
            RC2=1;
        }
        if(distance>3)
        {
            RC2=0;
        }
        
        delay(1000);
    }
    
}