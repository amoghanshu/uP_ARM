#include<1pc214x.h>
#include‹stdio.h>
void delay_ms(int ms){
int i, j;
for(j= 0; j‹ms; j†+){
for(i = 0; 1<12000; 1++);
}
｝
int main(){
IO1DIR = 0x00FF0000;// GPIO pins P1.16-P1.23 configured as 0/P|
while(1){
I01SET = 0x00550000;
delay_ms (500);
I01CLR = 0X00550000;
I01SET = 0X00AA0000;
delay_ms (500);|
I01CLR = 0x00AA0000;
}
return 0;
｝
