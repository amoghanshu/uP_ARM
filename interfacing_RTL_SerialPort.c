//Main Code
#include <lpc214x.h>
#include <stdint.h>
#include <stdio.h>
#include "UART0.h"
uint8_t alarm, flag, i;
__irq void RTC_ISR(void)
{
if (ILR & 0x01)
{
flag = 1;
ILR = ILR | 0x01;
}
if (ILR & 0x02)
{
alarm = 1;
ILR = ILR | 0x02;
}
VICVectAddr = 0;
}
typedef struct
{
uint8_t seconds;
uint8_t minutes;
uint8_t hours;
uint8_t day_of_month;
uint8_t day_of_week;
uint16_t day_of_year;
uint8_t month;
uint16_t year;
}RTC_Time;
void RTC_Set_Time( RTC_Time Set_Time)
{
SEC = Set_Time.seconds;
MIN = Set_Time.minutes;
HOUR = Set_Time.hours;
DOM = Set_Time.day_of_month;
DOW = Set_Time.day_of_week;
DOY = Set_Time.day_of_year;
MONTH = Set_Time.month;
YEAR = Set_Time.year;
}
void RTC_Set_Alarm_Time( RTC_Time Alarm_Time)
{
ALSEC = Alarm_Time.seconds;
ALMIN = Alarm_Time.minutes;
ALHOUR = Alarm_Time.hours;
ALDOM = Alarm_Time.day_of_month;
ALDOW = Alarm_Time.day_of_week;
ALDOY = Alarm_Time.day_of_year;
ALMON = Alarm_Time.month;
ALYEAR = Alarm_Time.year;
}
RTC_Time RTC_Get_Time(void)
{
RTC_Time time;
time.seconds = SEC;
time.minutes = MIN;
time.hours = HOUR;
time.day_of_month = DOM;
time.day_of_week = DOW;
time.day_of_year = DOY;
time.month = MONTH;
time.year = YEAR;
return time;
}
int main(void)
{
/* Setting Time + Alarm */
RTC_Time set_time, alarm_time, current_time;
char timestr[30], datestr[30];
alarm = 0;
flag = 0;
IO0DIR = 0x00000010; /* P0.4 as output pin for LED */
UART0_init();
ILR = 0x0; /* No RTC interrupts */
CCR = 0x02; /* Reset Clock Tick Counter */
CCR = 0x00;
CIIR = 0x00; /* No interrupts */
AMR = 0x00; /* Alarm registers not masked */
PREINT = 0x01C8; /* For 15MHz Fpclk, PREINT value */
PREFRAC = 0x61C0; /* For 15MHz Fpclk, PREFRAC value */
VICVectAddr0 = (unsigned) RTC_ISR;
VICVectCntl0 = 0x0000002D;
VICIntEnable = 0x00002000;
VICIntSelect = 0x00000000;
set_time.seconds = 00;
set_time.minutes = 25;
set_time.hours = 11;
set_time.day_of_month = 6;
set_time.day_of_week = 5;
set_time.day_of_year = 279;
set_time.month = 10;
set_time.year = 2017;
RTC_Set_Time(set_time);
CIIR = 0x01; /* Seconds value increment interrupt */
alarm_time.seconds = 15;
alarm_time.minutes = 25;
alarm_time.hours = 11;
alarm_time.day_of_month = 6;
alarm_time.day_of_week = 5;
alarm_time.day_of_year = 279;
alarm_time.month = 10;
alarm_time.year = 2017;
RTC_Set_Alarm_Time(alarm_time);
CCR = 0x01; /* Clock Enable */
ILR = 0x03; /* RTC interrupts enabled */
IO0CLR = 0x00000010;
/* Code lines below are common for "Setting time + Alarm" as well as "Only RTC
Read" */
while(1)
{
if(alarm == 1)
{
current_time = RTC_Get_Time();
sprintf(timestr,"Alarm!!!: %d:%d:%d
\r\n",current_time.hours,current_time.minutes,current_time.seconds);
UART0_SendString(timestr);
for(i=0;i<10;i++)
  {
  IO0SET = 0x00000010;
delay_ms(300);
IO0CLR = 0x00000010;
delay_ms(300);
}
alarm = 0;
}
if (flag == 1)
{
current_time = RTC_Get_Time();
sprintf(timestr,"Time: %d:%d:%d ",current_ time.hours, current_time.

minutes,current_time.seconds);

sprintf(datestr,"Date: %d/%d/%d \r\n", current_time. day_of_month,

current_time.month, current_time.year);

UART0_SendString(timestr);
UART0_SendString(datestr);
flag = 0;
}
}
}




//Library Function: UART0.c
#include "UART0.h"
char receive_string[50];
void delay_ms(uint16_t j) /* Function for delay in milliseconds */
{
uint16_t x,i;
for(i=0;i<j;i++)
{
for(x=0; x<6000; x++); /* loop to generate 1 millisecond delay with Cclk = 60MHz */
}
}
void UART0_init(void)
{
PINSEL0 = PINSEL0 | 0x00000005; /* Enable UART0 Rx0 and Tx0 pins of UART0
*/
U0LCR = 0x83; /* DLAB = 1, 1 stop bit, 8-bit character length */
U0DLM = 0x00; /* For baud rate of 9600 with Pclk = 15MHz */
U0DLL = 0x61; /* We get these values of U0DLL and U0DLM from formula */
U0LCR = 0x03; /* DLAB = 0 */
delay_ms(5);
}
void UART0_TxChar(char ch) /* A function to send a byte on UART0 */
{
U0THR = ch;
while( (U0LSR & 0x40) == 0 ); /* Wait till THRE bit becomes 1 which tells that
transmission is completed */
}
void UART0_SendString(char* str) /* A function to send string on UART0 */
{
uint8_t i = 0;
while( str[i] != '\0' )
{
UART0_TxChar(str[i]);
i++;
}
}
unsigned char UART0_RxChar(void) /* A function to receive a byte on UART0 */
{
while( (U0LSR & 0x01) == 0); /* Wait till RDR bit becomes 1 which tells that
receiver contains valid data */
return U0RBR;

}
void UART0_RxString(void) /* A function to receive string on UART0 */
{
uint8_t i = 0;
do{
while( (U0LSR & 0x01) == 0);

receive_string[i] = U0RBR;
i++;
}while( (U0RBR != 0x0D) );
}

// Header function: UART0.h
#ifndef UART_H
#define UART_H
#include <lpc214x.h>
#include <stdint.h>
void delay_ms(uint16_t j);
void UART0_init(void);
void UART0_TxChar(char ch);
void UART0_SendString(char* str);
unsigned char UART0_RxChar(void);
void UART0_RxString(void);
#endif


/* 
--> In programming part code should end with new line.
--> All programing files should be in the same folder.
*/
