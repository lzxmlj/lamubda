//modbus 程序，仅供参考 #include<reg52.h> #include"delay.h" #include"IO.h" #include"LED.h" #include"uart.h" #include"modbus.h"
unsigned int celiang[5]={0}; unsigned char t = 0; unsigned char flag;
void main() 
{ 
	uart_init(); 
	while(1) 
	{ 
	if(flag) 
		{
		frame_handle(t);
		} 
	} 
}
//#include"delay.h"
void delay_ms(unsigned int x)
{
	unsigned int a,b; 
	for(a = 0;a < 112;a++) 
		for(b = 0;b < x;b++); 
}
void delay_50ms(unsigned int x) 
{ 
	while(x--)
		{ delay_ms(50); }
}
void delay_50us(unsigned int x)
{
   unsigned int a,b;
   for(a = 0;a < 6;a++)
      for(b = 0;b < x;b++);
}
#include"uart.h" #include"modbus.h"

void uart_send(unsigned char senddata)
{ 
   SBUF=senddata;
   while(TI == 0);
   TI = 0;
}
void uart_sendstring(unsigned char *s) 
{
   while(*s!='\0') {
      uart_send(*s); s++;
   }
}
void string_ASCII(unsigned char *s) 
{
   unsigned char high,low;
   unsigned char i = 0;
   while(*s!='\0')
   {
      high = *s/16;
      low = *s%16;
      if(high > 64)
      {
         high = high + 55;
      }
      else
      {
         high = high + 48;
      }
      if(low > 64)
      {
         low = low + 55;
      }
      else
      {
         low = low + 48;
      }
      Tx[i++] = high;
      Tx[i++] = low;
      s++;
   }
   Tx[i] = '\0';
}
void uart_receive(void) interrupt 4 { static unsigned char i = 0;
ES = 0;
if(RI) {
RI = 0; Rx[i] = SBUF; if(Rx[i] == ':') {
Rx[0] = ':'; i = 0;
} if(Rx[i] == 0x0a && Rx[i-1] == 0x0d) { frame(i); flag = 1; i = 0; } i++; if(i >= MAX) { i = 0; }
}
ES = 1;
}
#include"modbus.h" #include"uart.h" #include"IO.h"
#define Addr 1
code unsigned char ASCII[17]="0123456789ABCDEF"; //unsigned char a[]={0x3A,0x30,0x31,0x30,0x31,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x31,0x46,0x44,0x0D,0x0A ,'\0'};
void frame(unsigned char i) { flag=1; t = i; }
void frame_handle(unsigned char i)
{ 
	unsigned char addr,func,j; 
	flag=0; 
	if(Rx[0] == ':') 
	{ 
		for(j = 0;j < 5;j++)
		{
	   		Tx[j] = Rx[j];
		}
		chartoint(Rx);
		if(LRC(i))
		{
		   addr = Rx[1]*16+Rx[2];
			if(addr == Addr)
			{
			   func = Rx[3]*16+Rx[4];
				switch(func)
				{ case 1:
					func_1();
					break;
				  case 3:
				  	func_3();
					break; 
				  case 5:
				  	func_5(i);
				    break;
              case 6:
               func_6();
               break;
              case 15:
               func_15();
               break; 
              case 16:
               func_16();
               break; 
              default:
               Tx[3]+=8;
              break;
           } 
         }
         else
         {
            Tx[3]+=8;
            Tx[5]='7';
            Tx[6]='e';
            Tx[7]=0x0d;
            Tx[8]=0x0a;
            Tx[9]='\0';
            uart_sendstring(Tx);
         }
      }
      else
      {
         Tx[3]+=8;
         Tx[5]='7';
         Tx[6]='e';
         Tx[7]=0x0d;
         Tx[8]=0x0a;
         Tx[9]='\0';
         uart_sendstring(Tx);
      }
   }
}
void chartoint(unsigned char temp[])
{
   unsigned char i = 1;
   while(temp[i] != 0x0d) {
      if(temp[i] > 64)
      {
         temp[i] = temp[i]-55;
      }
      else
      {
         temp[i] = temp[i]-48;
      }
      i++;
   }
}
void inttochar(unsigned char temp[]) { unsigned char i = 1; while(temp[i] != 0x0d||temp[i+1] != 0x0a) { temp[i] = ASCII[temp[i]]; i++; } }
void func_1() { unsigned char temp;
switch(Rx[5]*256*16+Rx[6]*256+Rx[7]*16+Rx[8]) { case 0: switch(Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12]) { case 1:temp = P3&0x10;break; case 2:temp = P3&0x30;break; case 3:temp = P3&0x70;break; case 4:temp = P3&0xF0;break; default:Tx[3] += 8;temp=0;break; } break;
case 1:
switch(Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12]) { case 1:temp = P3&0x20;break; case 2:temp = P3&0x60;break; case 3:temp = P3&0xe0;break; default:Tx[3] += 8;temp=0;break; } break;
case 2:
switch(Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12]) { case 1:temp = P3&0x40;break; case 2:temp = P3&0xc0;break; default:Tx[3] += 8;temp=0;break; } break;
case 3:
switch(Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12]) { case 1:temp = P3&0x80;break; default:Tx[3] += 8;temp=0;break; } break; default:Tx[3] += 8;temp=0;break;
} if(Tx[3]=='8'||Tx[3]=='9') { Tx[5]='7'; Tx[6]='e'; Tx[7]=0x0d; Tx[8]=0x0a; Tx[9]='\0'; uart_sendstring(Tx); } else { temp = temp/16; temp = ASCII[temp]; Tx[5] = '0'; Tx[6] = '1'; Tx[7] = '0'; Tx[8] = temp; Tx[11] = 0x0d;
Tx[12] = 0x0a; LRC_cal(11); Tx[13] = '\0';
}
uart_sendstring(Tx);
}
void func_3() { unsigned char j,k; unsigned char temp;
k=Rx[5]*256*16+Rx[6]*256+Rx[7]*16+Rx[8]; j=Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12];
EEPROM(); //readreg(j,k);
temp=2*j; Tx[5]='0'; Tx[6]=ASCII[temp]; for(temp=0;temp<j;temp++) { Tx[temp*4+7] = ASCII[celiang[temp+k]/256/16]; Tx[temp*4+8] = ASCII[celiang[temp+k]/256%16]; Tx[temp*4+9] = ASCII[celiang[temp+k]%256/16]; Tx[temp*4+10] = ASCII[celiang[temp+k]%256%16]; } Tx[j*4+9] = 0x0d; Tx[j*4+10] = 0x0a; LRC_cal(j*4+9); Tx[j*4+11] = '\0'; uart_sendstring(Tx);
}
void func_5(unsigned char no) { unsigned temp; Rx[no+1]='\0'; switch(Rx[5]*256*16+Rx[6]*256+Rx[7]*16+Rx[8]) { case 0: if(Rx[9]==15&&Rx[10]==15&&Rx[11]==0&&Rx[12]==0) {
inttochar(Rx); uart_sendstring(Rx); LED1_ON;
} if(Rx[9]==0&&Rx[10]==0&&Rx[11]==0&&Rx[12]==0) { inttochar(Rx); uart_sendstring(Rx); LED1_OFF; } break; case 1: if(Rx[9]==15&&Rx[10]==15&&Rx[11]==0&&Rx[12]==0) { inttochar(Rx); uart_sendstring(Rx); LED2_ON; } if(Rx[9]==0&&Rx[10]==0&&Rx[11]==0&&Rx[12]==0) { inttochar(Rx); uart_sendstring(Rx); LED2_OFF; } break; case 2: if(Rx[9]==15&&Rx[10]==15&&Rx[11]==0&&Rx[12]==0) { inttochar(Rx); uart_sendstring(Rx); LED3_ON; } if(Rx[9]==0&&Rx[10]==0&&Rx[11]==0&&Rx[12]==0) { inttochar(Rx); uart_sendstring(Rx); LED3_OFF; } break; case 3: if(Rx[9]==15&&Rx[10]==15&&Rx[11]==0&&Rx[12]==0) { inttochar(Rx); uart_sendstring(Rx); LED4_ON; } if(Rx[9]==0&&Rx[10]==0&&Rx[11]==0&&Rx[12]==0)
{
inttochar(Rx); uart_sendstring(Rx); LED4_OFF;
} break; default:Tx[3] += 8; Tx[5]='7'; Tx[6]='e'; Tx[7]=0x0d; Tx[8]=0x0a; Tx[9]='\0'; uart_sendstring(Tx);break;
}
}
/*功 能：功能 6 /*函数名：func_6 /*参 数：无 /*返回值：无 */ void func_6() { unsigned int j; celiang[Rx[5]*256*16+Rx[6]*256+Rx[7]*16+Rx[8]] = Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12]; for(j = 0;j < 9;j++) { Tx[j] = Rx[j]; } Tx[9] = celiang[Rx[5]*256*16+Rx[6]*256+Rx[7]*16+Rx[8]]/256/16; Tx[10] =celiang[Rx[5]*256*16+Rx[6]*256+Rx[7]*16+Rx[8]]/256%16; Tx[11] = celiang[Rx[5]*256*16+Rx[6]*256+Rx[7]*16+Rx[8]]%256/16; Tx[12] = celiang[Rx[5]*256*16+Rx[6]*256+Rx[7]*16+Rx[8]]%256%16; Tx[15] = 0x0d; Tx[16] = 0x0a; inttochar(Tx); LRC_cal(15); Tx[17] ='\0'; uart_sendstring(Tx); }
void func_15() {
unsigned temp,j; temp = Rx[15]*16+Rx[16]; switch(Rx[5]*256*16+Rx[6]*256+Rx[7]*16+Rx[8]) { case 0:
switch(Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12]) { case 1:P2 = ~((temp<<4&0x10));break; case 2:P2 = ~((temp<<4&0x30));break; case 3:P2 = ~((temp<<4&0x70));break; case 4:P2 = ~((temp<<4&0xf0));break; default:Tx[3] += 8;temp=0;break; } break;
case 1:
switch(Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12]) { case 1:P2 = ~((temp<<4)&0x20);break; case 2:P2 = ~((temp<<4)&0x60);break; case 3:P2 = ~((temp<<4)&0xe0);break; default:Tx[3] += 8;temp=0;break; } break;
case 2:
switch(Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12]) { case 1:P2 = ~((temp<<4)&0x40);break; case 2:P2 = ~((temp<<4)&0xc0);break; default:Tx[3] += 8;temp=0;break; } break;
case 3:
switch(Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12]) { case 1:P2 = ~((temp<<4)&0x80);break; default:Tx[3] += 8;temp=0;break; } break; default:Tx[3] += 8; Tx[5]='7'; Tx[6]='e'; Tx[7]=0x0d; Tx[8]=0x0a;
Tx[9]='\0'; uart_sendstring(Tx);break;
} for(j = 0;j < 13;j++) { Tx[j] = Rx[j]; } Tx[15] = 0x0d; Tx[16] = 0x0a; inttochar(Tx); LRC_cal(15); Tx[17] = '\0'; uart_sendstring(Tx);
}
void func_16()
	{ EEPROM();
switch(Rx[5]*256*16+Rx[6]*256+Rx[7]*16+Rx[8]) { case 0: switch(Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12]) { case 1: celiang[0] = Rx[15]*256*16+Rx[16]*256+Rx[17]*16+Rx[18];break; case 2: celiang[0] = Rx[15]*256*16+Rx[16]*256+Rx[17]*16+Rx[18]; celiang[1] = Rx[19]*256*16+Rx[20]*256+Rx[21]*16+Rx[22];break; case 3: celiang[0] = Rx[15]*256*16+Rx[16]*256+Rx[17]*16+Rx[18]; celiang[1] = Rx[19]*256*16+Rx[20]*256+Rx[21]*16+Rx[22]; celiang[2] = Rx[23]*256*16+Rx[24]*256+Rx[25]*16+Rx[26];break; case 4: celiang[0] = Rx[15]*256*16+Rx[16]*256+Rx[17]*16+Rx[18]; celiang[1] = Rx[19]*256*16+Rx[20]*256+Rx[21]*16+Rx[22]; celiang[2] = Rx[23]*256*16+Rx[24]*256+Rx[25]*16+Rx[26]; celiang[3] = Rx[27]*256*16+Rx[28]*256+Rx[29]*16+Rx[30];break; } break; case 1: switch(Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12]) { case 1: celiang[1] = Rx[15]*256*16+Rx[16]*256+Rx[17]*16+Rx[18];break;
case 2: celiang[1] = Rx[15]*256*16+Rx[16]*256+Rx[17]*16+Rx[18]; celiang[2] = Rx[19]*256*16+Rx[20]*256+Rx[21]*16+Rx[22];break; case 3: celiang[1] = Rx[15]*256*16+Rx[16]*256+Rx[17]*16+Rx[18]; celiang[2] = Rx[19]*256*16+Rx[20]*256+Rx[21]*16+Rx[22]; celiang[3] = Rx[23]*256*16+Rx[24]*256+Rx[25]*16+Rx[26];break; } break; case 2: switch(Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12]) { case 1: celiang[2] = Rx[15]*256*16+Rx[16]*256+Rx[17]*16+Rx[18];break; case 2: celiang[2] = Rx[15]*256*16+Rx[16]*256+Rx[17]*16+Rx[18]; celiang[3] = Rx[19]*256*16+Rx[20]*256+Rx[21]*16+Rx[22];break; } break; case 3: switch(Rx[9]*256*16+Rx[10]*256+Rx[11]*16+Rx[12]) { case 1: celiang[3] = Rx[15]*256*16+Rx[16]*256+Rx[17]*16+Rx[18];break; } break; default:Tx[3] += 8; Tx[5]='7'; Tx[6]='e'; Tx[7]=0x0d; Tx[8]=0x0a; Tx[9]='\0'; uart_sendstring(Tx);break; } if(Tx[3]!='8'&&Tx[3]!='9') { Tx[5] = ASCII[Rx[5]]; Tx[6] = ASCII[Rx[6]]; Tx[7] = ASCII[Rx[7]]; Tx[8] = ASCII[Rx[8]]; Tx[9] = ASCII[Rx[9]]; Tx[10] = ASCII[Rx[10]]; Tx[11] = ASCII[Rx[11]]; Tx[12] = ASCII[Rx[12]];
Tx[15] = 0x0d; Tx[16] = 0x0a; LRC_cal(15); Tx[17] = '\0'; uart_sendstring(Tx);
}
}
/*功 能：计算 LRC /*函数名：LRC_cal /*参 数：停止位在 Tx 中的位置 /*返回值：无 */ void LRC_cal(unsigned char i) { unsigned char sum=0,j; chartoint(Tx); for(j = 1;j < i-2;j+=2) { sum +=Tx[j]*16+Tx[j+1]; } inttochar(Tx); sum=sum%256; sum=255-sum+1; Tx[i-2] = ASCII[sum/16]; Tx[i-1] = ASCII[sum%16]; }
unsigned char LRC(unsigned char i) { unsigned char sum = 0,temp[2],j; for(j = 1;j < i-3;j+=2) { sum +=Rx[j]*16+Rx[j+1]; } sum=sum%256; sum=255-sum+1; temp[0]=sum/16; temp[1]=sum%16; if(temp[0] == Rx[i-3]&&temp[1] == Rx[i-2]) { return 1; } return 0;
}