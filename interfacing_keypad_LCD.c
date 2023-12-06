#include <lpc214x.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>
unsigned int adc_value1,adc_value2,C;
unsigned char buf[16] = {0};
char check_key(void);
unsigned int KeyPort=0x00f00000;
#define sw1 0x00010000
#define sw2 0x00020000
#define sw3 0x00040000
#define sw4 0x00080000
void delay_ms(uint16_t j) /* Function for delay in milliseconds */
{
uint16_t x,i;
for(i=0;i<j;i++)
{
for(x=0; x<6000; x++); /* loop to generate 1 millisecond delay with Cclk = 60MHz */
}
}
void LCD_CMD(char command)
{
IO0PIN = ( (IO0PIN & 0xFF0FFFFF) | ((command & 0xF0)<<16) ); /* Upper nibble
of command */
IO0SET = 0x00020000; /* EN = 1 */
IO0CLR = 0x00010000; /* RS = 0, RW = 0 */
delay_ms(5);
IO0CLR = 0x00020000; /* EN = 0, RS and RW unchanged(i.e. RS = RW = 0) */
delay_ms(5);
IO0PIN = ( (IO0PIN & 0xFF0FFFFF) | ((command & 0x0F)<<20) ); /* Lower nibble
of command */
IO0SET = 0x00020000; /* EN = 1 */
IO0CLR = 0x00010000; /* RS = 0, RW = 0 */
delay_ms(5);
IO0CLR = 0x00020000; /* EN = 0, RS and RW unchanged(i.e. RS = RW = 0) */
delay_ms(5);
}
void LCD_INIT(void)
{
IO0DIR = 0x00FFFFF0; /* P0.20 to P0.23 LCD Data. P0.16,,17 as RS RW and EN */
delay_ms(20);
LCD_CMD(0x02); /* Initialize lcd in 4-bit mode */
LCD_CMD(0x28); /* 2 lines */
LCD_CMD(0x0C); /* Display on cursor off */
LCD_CMD(0x06); /* Auto increment cursor */
LCD_CMD(0x01); /* Display clear */
LCD_CMD(0x80); /* First line first position */
}
void LCD_STRING (char* msg)
{
uint8_t i=0;
while(msg[i]!=0)
{
IO0PIN = ( (IO0PIN & 0xFF0FFFFF) | ((msg[i] & 0xF0)<<16) );
IO0SET = 0x00030000; /* RS = 1, EN = 1 */
IO0CLR = 0x00000020; /* RW = 0 */
delay_ms(2);
IO0CLR = 0x00020000; /* EN = 0, RS and RW unchanged(i.e. RS = 1, RW =

0) */

delay_ms(5);
IO0PIN = ( (IO0PIN & 0xFF0FFFFF) | ((msg[i] & 0x0F)<<20) );
IO0SET = 0x00030000; /* RS = 1, EN = 1 */
IO0CLR = 0x00000020; /* RW = 0 */
delay_ms(2);
IO0CLR = 0x00020000; /* EN = 0, RS and RW unchanged(i.e. RS = 1, RW =

0) */

delay_ms(5);
i++;
}
}
void LCD_CHAR (char msg)
{

IO0PIN = ( (IO0PIN & 0xFF0FFFFF) | ((msg & 0xF0)<<16) );
IO0SET = 0x00030000; /* RS = 1, EN = 1 */
IO0CLR = 0x00000020; /* RW = 0 */
delay_ms(2);
IO0CLR = 0x00020000; /* EN = 0, RS and RW unchanged(i.e. RS = 1, RW =

0) */

delay_ms(5);
IO0PIN = ( (IO0PIN & 0xFF0FFFFF) | ((msg & 0x0F)<<20) );
IO0SET = 0x00030000; /* RS = 1, EN = 1 */
IO0CLR = 0x00000020; /* RW = 0 */
delay_ms(2);
IO0CLR = 0x00020000; /* EN = 0, RS and RW unchanged(i.e. RS = 1, RW =

0) */

delay_ms(5);

}
int main(void)
{
IODIR1=KeyPort;
LCD_INIT();
LCD_STRING("--- UVCE ECE---");
LCD_CMD(0xC0);
while(1)
{
delay_ms(5);
LCD_CHAR(check_key());
}
}
char check_key(void)
{
while(1)
{
IOSET1=0x00EF0000;
delay_ms(5);
if((IOPIN1&sw1)==0)
{
C='1';
while((IOPIN1&sw1)==0);
return(C);
}
if((IOPIN1&sw2)==0)
{
C='5';
while((IOPIN1&sw2)==0);
return(C);
}
if((IOPIN1&sw3)==0)
{
C='9';
while((IOPIN1&sw3)==0);
return(C);
}
if((IOPIN1&sw4)==0)
{
C='C';
while((IOPIN1&sw4)==0);
return(C);
}
IOCLR1=0x00EF0000;
delay_ms(5);
IOSET1=0x00DF0000;
delay_ms(5);
if((IOPIN1&sw1)==0)
{
C='2';
while((IOPIN1&sw1)==0);
return(C);
}
if((IOPIN1&sw2)==0)
{
C='6';
while((IOPIN1&sw2)==0);
return(C);
}
if((IOPIN1&sw3)==0)
{
C='0';
while((IOPIN1&sw3)==0);
return(C);
}
if((IOPIN1&sw4)==0)
{
C='D';
while((IOPIN1&sw4)==0);
return(C);
}
IOCLR1=0x00DF0000;
delay_ms(5);
IOSET1=0x00BF0000;
delay_ms(5);
if((IOPIN1&sw1)==0)
{
C='3';
while((IOPIN1&sw1)==0);
return(C);
}
if((IOPIN1&sw2)==0)
{
C='7';
while((IOPIN1&sw2)==0);
return(C);
}
if((IOPIN1&sw3)==0)
{
C='A';
while((IOPIN1&sw3)==0);
return(C);
}
if((IOPIN1&sw4)==0)
{
C='E';
while((IOPIN1&sw4)==0);
return(C);
}
IOCLR1=0x00BF0000;
delay_ms(5);
IOSET1=0x007F0000;
delay_ms(5);
if((IOPIN1&sw1)==0)
{
C='4';
while((IOPIN1&sw1)==0);
return(C);
}
if((IOPIN1&sw2)==0)
{
C='8';
while((IOPIN1&sw2)==0);
return(C);
}
if((IOPIN1&sw3)==0)
{
C='B';
while((IOPIN1&sw3)==0);
return(C);
}
if((IOPIN1&sw4)==0)
{
C='F';
while((IOPIN1&sw4)==0);
return(C);
}
IOCLR1=0x007F0000;
delay_ms(5);
}
}
//Connections: Port 2 to LCD and Port 4 to Keypad.
