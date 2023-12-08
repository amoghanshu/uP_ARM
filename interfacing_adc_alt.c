#include<lpc214x.h>
#include<stdio.h>
#include<string.h>
#include "lcd.h"
int main(void){
// Variable decleration must happen before any executable code in C90 (ARM Comp
unsigned int result; char voltage[16];
LCD_INITO);
PINSEL1 = 0x01000000; /* P0.28 function set as AD0.1 */
ADOCR = 0x001200402;
//7:0 = 00000010 -> AD0.1 is activated, 15:8 = 00000100
//19:17 = 000, 21 = 1 -> is like EN for ADC, 26:24 = 001
while(1){
ADOCR = 0x001200402;
//!! Comment to be added
while (!(ADODR1 & 0x80000000) ); //Last bit is set 1 if conversion is dor
result = ADODR1;
result = (result > 6);
result = (result & 0x000003FF); 
sprintf(voltage, 16, "V=%f", ((result/1023.0) * 3.3 ));
LCD_CHAR (0x01, 0); /* Display clear */ 
LCD_CHAR (0x80, 0); /* Cursor at home position */ 
delay_ms (10);
LCD_STRING (voltage);
delay_ms (90);
｝
return 0;
}
