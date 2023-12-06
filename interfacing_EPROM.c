#include<LPC214x.h>
void I2C_init(void);
void byte_write(unsigned char address, unsigned char location, unsigned char data);
void send_start(void);
void send_stop(void);
unsigned char byte_read(unsigned char address, unsigned char location);
void msdelay(unsigned int time);
void uart0Init(void)
{
U0LCR=0x83; //8bit data, no parity, 1 stop bit
U0DLL=97; // 9600 baud rate @15Mhz Pclk
U0LCR=0x03; // DLAB=0
}
void uart0Putch(unsigned char ch)
{
U0THR=ch; // Transmitter holding register
while(!(U0LSR & 0x20)); // wait still THR=0
}
void UART0_Txstring(unsigned char *Str)
{
int i=0;
while(Str[i]!='\0')
{
uart0Putch(Str[i]);
i++;
}
}
int main()
{
int i;
unsigned char read_data;
unsigned char write_data[10]="UVCE"; // {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
0x48, 0x49, 0x4A};
PINSEL0=0x00000055;
PINSEL1=0x00000000;
PINSEL2=0x00000000;
uart0Init();
I2C_init();
UART0_Txstring("Writing from I2C");
for(i=0;i<10;i++)
{
byte_write(0xA0,i,write_data[i]);
uart0Putch(write_data[i]);
msdelay(100);
}
uart0Putch(0x0d);
UART0_Txstring("reading from I2C : ");
for(i=0;i<6;i++)
{
read_data=byte_read(0xA0,i);
uart0Putch(read_data);
}
}
void I2C_init(void)
{
I2C0CONCLR=0xFF;
I2C0CONSET=0x40; //enable I2C
I2C0SCLH=75; //0x4B
I2C0SCLL=75; //0x4B
}
void byte_write(unsigned char address, unsigned char location, unsigned char data)
{
I2C0CONCLR=0xFF;
I2C0CONSET=0x40;
send_start(); //send start condition
while(!(I2C0CONSET&0x08)); //check SI flag
I2C0CONCLR=0x28; //clear SI flag and start
I2C0DAT=address&0xFE; //selecting address in
while(!(I2C0CONSET&0x08)); //check SI flag
I2C0CONCLR=0x28; //clear SI flag and start
I2C0DAT=location; //sending memory location
while(!(I2C0CONSET&0x08)); //check SI flag
I2C0CONCLR=0x28; //clear SI flag and start
I2C0DAT=data;
while(!(I2C0CONSET&0x08)); //check SI flag
I2C0CONCLR=0x28; //clear SI flag and start flag
send_stop(); //send stop bit
}
void send_start()
{
I2C0CONSET=0x20;
}
void send_stop()
{
I2C0CONSET=0x10;
}
unsigned char byte_read(unsigned char address,unsigned char location)
{
unsigned char data;
I2C0CONCLR=0xFF;
I2C0CONSET=0x40;
send_start();
while(!(I2C0CONSET&0x08)); //check SI flag
I2C0CONCLR=0x28; //clear SI flag and start
I2C0DAT=address&0xFE; //selecting address in
while(!(I2C0CONSET&0x08)); //check SI flag
I2C0CONCLR=0x28; //clear SI flag and start
I2C0DAT=location; //sending memory location
while(!(I2C0CONSET&0x08)); //check SI flag
I2C0CONCLR=0x28; //clear SI flag and start
send_start(); //repeated start
while(!(I2C0CONSET&0x08)); //check SI flag
I2C0CONCLR=0x28;
I2C0DAT=address|0x01;
while(!(I2C0CONSET&0x08)); //check SI flag
I2C0CONCLR=0x28;
I2C0CONCLR=0x04; //NACK
while(!(I2C0CONSET&0x08)); //check SI flag
I2C0CONCLR=0x28;
data=I2C0DAT;
send_stop();
return data;
}
void msdelay(unsigned int time)
{
int i,j;
for(i=0;i<time;i++)
{
for(j=0;j<1008;j++)
{
}
}
}
