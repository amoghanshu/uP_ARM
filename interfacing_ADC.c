#include <lpc214x.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
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
IO0CLR = 0x00020000; /* EN = 0, RS and RW unchanged(i.e. RS = RW = 0)*/
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
IO0DIR = 0x00FFFFF0; /* P0.12 to P0.15 LCD Data. P0.4,5,6 as RS RW and EN */
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
uint32_t result;
char volt[18];
LCD_INIT();
PINSEL1 = 0x01000000; /* P0.28 as AD0.1 */
AD0CR = 0x00200402; /* ADC operational, 10-bits, 11 clocks for conversion */
LCD_STRING("---UVCE ECE---");
while(1)
{
AD0CR = AD0CR | (1<<24); /* Start Conversion */
while ( !(AD0DR1 & 0x80000000) ); /* Wait till DONE */
result = AD0DR1;
result = (result>>6);
result = (result & 0x000003FF);
LCD_CMD(0xc0);
sprintf(volt, "ADC=%i ", result);
LCD_STRING(volt);
memset(volt, 0, 18);
}
}


//Connection: Port2 to LCD and Port3 to ADC, vary the middle potentiometer to see the digital values.
