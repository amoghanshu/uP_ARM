Code:
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
}
37
38
39 г {|
40
41
42
43
44
45
46
47
48 г
49
50
51
52 г
53
54
55
56
57
58
}
59
#include <1pc214x.h>
#include ‹stdint.h>
#include < stdio.h›>
#include
"lcd.h"
char keys[41[4] = {f'1', '2', '3', '4'}, /* Keypad standard arrays */
(5, 6, 7, 87;
', 'b'},
｛'c'，'d'，'e'，'f'｝｝；
uint32_t rows[4] = {0x00fe0000, 0x00fd0000, 0x00fb0000, 0x00†70000}; uint32_† cols[4] = {0x00100000, 0x00200000, 0x00400000, 0x00800000};
char KEYPAD_CHECK(void){
inta,j；
for（i=0;i<4; i++）｛
I01SET = rows|1];
delay_ms (5);
for（j=0;j<4;j++）｛
if((I01PIN&cols[j]) == 0){
while((I01PIN&cols[j]) == 0); /* Uncomment this if expected op is not observed *,
return(keys[i][jl);
/* If inside col-i row-j is pressed */
｝
｝
}
I01CLR = rows|i];
delay_ms(5);
return 'x';
int main(void)
char c_prt;
LCD_INIT();
LCD_CHAR(0xc0, 0); /* Clear screen */
/* Keypad I/0 setup */
IO1DIR = 0x000F0000; /* C4-C1 = P1.23-P1.20 --> Output, R4-R1 = P1.19-P1.16 --> Input */
while(1){
delay_ms (5);
c_prt = KEYPAD_CHECK();
if(c_prt!= 'x' ){
LCD_CHAR (OxcO, 0);
LCD_CHAR(c_prt, 1);
｝
}
return 0;