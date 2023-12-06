#include<LPC21xx.h>
void _delay_ms(int j)
{
int x,l;
for(l=0;l<j;l++)
{
for(x=0; x<6000; x++); /* loop to generate 1 milisecond delay with Cclk = 60MHz */
}
}
int main()
{
int i=0;
PINSEL0=0x00000000; //select pins for blinking led
IODIR0=0x00ff0000; //select I/O pins as output
while(1)
{
for(i=0;i<100;i++)
{
IOSET0 = 0x00EC0000;
_delay_ms(4);
IOCLR0 = 0x00EC0000;
IOSET0 = 0x00DC0000;
_delay_ms(4);
IOCLR0=0x00DC0000;
IOSET0 = 0x00BC0000;
_delay_ms(4);
IOCLR0=0x00BC0000;
IOSET0 = 0x007C0000;
_delay_ms(4);
IOCLR0=0x007C0000;
}
for(i=0;i<100;i++)
{
IOSET0 = 0x007C0000;
_delay_ms(4);
IOCLR0=0x007C0000;
IOSET0 = 0x00BC0000;
_delay_ms(4);
IOCLR0=0x00BC0000;
IOSET0 = 0x00DC0000;
_delay_ms(4);
IOCLR0=0x00DC0000;
IOSET0 = 0x00EC0000;
_delay_ms(4);
IOCLR0=0x00EC0000;
}
}

}
//Connections: Port 2 to Relay/stepper motor
