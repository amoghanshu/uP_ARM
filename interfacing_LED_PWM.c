/*
* PIN CONNECTION :
* LED -> 0.0
* BUTTON1 -> 0.2
* BUTTON2 -> 0.3
* BUTTON3 -> 0.4
* BUTTON4 -> 0.5
*/
#include <lpc214x.h>
#define PLOCK 0x00000400
#define PWMPRESCALE 60 //60 PCLK cycles to increment TC by 1 i.e 1 Micro-second
void initPWM(void);
void initClocks(void);
void setupPLL0(void);
void feedSeq(void);
void connectPLL0(void);
void delay_ms(int j) /* Function for delay in milliseconds */
{
int x,i;
for(i=0;i<j;i++)
{
for(x=0; x<6000; x++); /* loop to generate 1 millisecond delay with Cclk = 60MHz */
}
}
int DUTY=0;
int main(void)
{
initClocks(); //Initialize CPU and Peripheral Clocks @ 60Mhz
initPWM(); //Initialize PWM //Also by default all pins are configured as Inputs after
MCU Reset.
IO0DIR = 0x01;
while(1)
{

for(DUTY=0;DUTY<10000;DUTY++)
{
PWMMR1 = DUTY; //25% Bright
PWMLER = (1<<1);
delay_ms(1);
}

}
}
void initPWM(void)
{
/*Assuming that PLL0 has been setup with CCLK = 60Mhz and PCLK also = 60Mhz.*/
/*This is a per the Setup & Init Sequence given in the tutorial*/
PINSEL0 = 0x00000002; //SELECT PIN Select 0 FOR PWM1(P0.0)
PWMPCR = 0x0; //Select Single Edge PWM - by default its single Edged so this line
can be removed
PWMPR = PWMPRESCALE-1; // 1 micro-second resolution
PWMMR0 = 10000; // 10ms period duration
PWMMR1 = 2500; // 2.5ms - pulse duration i.e width (Brigtness level)
PWMMCR = (1<<1); // Reset PWMTC on PWMMR0 match
PWMLER = (1<<1) | (1<<0); // update MR0 and MR1
PWMPCR = (1<<9); // enable PWM output
PWMTCR = (1<<1) ; //Reset PWM TC & PR
//Now , the final moment - enable everything
PWMTCR = (1<<0) | (1<<3); // enable counters and PWM Mode
//PWM Generation goes active now - LED must be 25% Bright after boot!!
//Now you can get the PWM output at Pin P0.0!
}
void initClocks(void)
{
setupPLL0();
feedSeq(); //sequence for locking PLL to desired freq.
connectPLL0();
feedSeq(); //sequence for connecting the PLL as system clock
//SysClock is now ticking @ 60Mhz!
VPBDIV = 0x01; // PCLK is same as CCLK i.e 60Mhz
}
//---------PLL Related Functions :---------------
void setupPLL0(void)
{
//Note : Assuming 12Mhz Xtal is connected to LPC2148.
PLL0CON = 0x01; // PPLE=1 & PPLC=0 so it will be enabled
// but not connected after FEED sequence
PLL0CFG = 0x24; // set the multipler to 5 (i.e actually 4)
// i.e 12x5 = 60 Mhz (M - 1 = 4)!!!
// Set P=2 since we want FCCO in range!!!
// So , Assign PSEL =01 in PLL0CFG as per the table.
}
void feedSeq(void)
{
PLL0FEED = 0xAA;
PLL0FEED = 0x55;
}
void connectPLL0(void)
{
// check whether PLL has locked on to the desired freq by reading the lock bit in the
PPL0STAT register
while( !( PLL0STAT & PLOCK )); // now enable(again) and connect
PLL0CON = 0x03;
}

// Connection: Port 0 to LED
