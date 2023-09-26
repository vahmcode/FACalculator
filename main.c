/*
Chip type               : ATmega32A
Program type            : Application
AVR Core Clock frequency: 16.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*/

#include <mega32a.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
// Alphanumeric LCD functions
#include <alcd.h>
#include "new_alcd.h"
// I2C Bus functions
#include <i2c.h>
// DS1307 Real Time Clock functions
#include <ds1307.h>
#define buzzer    PORTB.3
#define g2        PINB.4
#define g3        PINB.5
#define g4        PINB.6
#define g5        PINB.7
#define g1        PIND.5
#define hps       PINB.0
#define normal    PINB.1
#define start     PIND.4
#define analog    PIND.0
// Declare your global variables here
char enable;
char alarm;
char s;
char k;
char run2;
char buzzerf;
char buzzeri;
char z; 
char r;
char run;
char run_bar;
char run_init;
char run_init_bar;
char run1;
char i;
char sec_point;
char sec_point_bar;
char update_time;
char update_time_bar;
float rpmf;
float vc=0;
float vc_real;
float vc_bar;
float v0=0;
float u;
char buf[20];
unsigned long int rpm;
float time;
char stat1;
char stat2;
char stat3;
char stat4;
char stat5;
char a;
float d1;
float d2;
float d3;
float d4;
float d5;
float d1h;
float d2h;
float d3h;
float d4h;
float d5h;      
float open_time;
float open_time_bar;
eeprom float total_time;
float fuel;
float fuel_bar;
eeprom float total_fuel;
float discharge_time ;
char hour;
char minute;
char sec;
char update;
char sec_bar;
char minute_bar;
char hour_bar;
char enable_fuel_calc;
char initialize;
char initialize_bar;
float ta=(15.4*0.8)*0.195;
float ta_bar=150;
char run_bar;
float v1,v2,v3,v4,v5,v6,v7,v8,v9,v10;
float fuel_index=13.333*0.81;
float min_fuel;
float max_fuel;
float step_bar;
int amp_bar;
char bar;
char temp; 
int low_1=2500;
int up_1=4000;
int low_2=3000;
int up_2=5000;  
int low_3=3000;
int up_3=4500;  
int low_4=3500;
int up_4=4000;  
int low_5=3500;
int up_5=3750;                                              
// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (0<<ADLAR))

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=(1<<ADSC);
// Wait for the AD conversion to complete
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);              
return ADCW;
}
//*****************Show Data ****************
// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void)
{
// Place your code here
while(PIND.2==0);
r++;
if(r==1)
{
lcd_clear();
lcd_gotoxy(8,0) ;
lcd_puts("Test_1");
lcd_gotoxy(4,1) ;
lcd_puts("rpm:2500-4000");
lcd_gotoxy(0,2);
sprintf(buf,"TN:%5.2fs",d1);
lcd_puts(buf);
lcd_gotoxy(11,2);
sprintf(buf,"TH:%5.2fs",d1h);
lcd_puts(buf);    
}
if(r==2)
{
lcd_clear();
lcd_gotoxy(8,0) ;
lcd_puts("Test_2");
lcd_gotoxy(4,1) ;
lcd_puts("rpm:3000-5000");
lcd_gotoxy(0,2);
sprintf(buf,"TN:%5.2fs",d2);
lcd_puts(buf);
lcd_gotoxy(11,2);
sprintf(buf,"TH:%5.2fs",d2h);
lcd_puts(buf);  
}
if(r==3)
{
lcd_clear();
lcd_gotoxy(8,0) ;
lcd_puts("Test_3");
lcd_gotoxy(4,1) ;
lcd_puts("rpm:3000-4500");
lcd_gotoxy(0,2);
sprintf(buf,"TN:%5.2fs",d3);
lcd_puts(buf);
lcd_gotoxy(11,2);
sprintf(buf,"TH:%5.2fs",d3h);
lcd_puts(buf);    
}
if(r==4)
{
lcd_clear();
lcd_gotoxy(8,0) ;
lcd_puts("Test_4");
lcd_gotoxy(4,1) ;
lcd_puts("rpm:3500-4000");
lcd_gotoxy(0,2);
sprintf(buf,"TN:%5.2fs",d4);
lcd_puts(buf);
lcd_gotoxy(11,2);
sprintf(buf,"TH:%5.2fs",d4h);
lcd_puts(buf);  
}
if(r==5)
{
lcd_clear();
lcd_gotoxy(8,0) ;
lcd_puts("Test_5");
lcd_gotoxy(4,1) ;
lcd_puts("rpm:3500-3750");
lcd_gotoxy(0,2);
sprintf(buf,"TN:%5.2fs",d5);
lcd_puts(buf);
lcd_gotoxy(11,2);
sprintf(buf,"TH:%5.2fs",d5h);
lcd_puts(buf);   
}
if(r==6)
{
lcd_clear();
enable=1;
time=0;  
stat1=1;
stat2=1;
stat3=1;
stat4=1;
stat5=1;
a=1;
u=0;
// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: On
// INT2 Mode: Falling Edge
lcd_clear();
GICR=(0<<INT1) | (0<<INT0) | (1<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (0<<INTF0) | (1<<INTF2);
}}
// External Interrupt 2 service routine
//***************Pulse Management***************
// in once turn on timer in twice turn off tiner//  
interrupt [EXT_INT2] void ext_int2_isr(void)
{
i++;
if(i==1)
{
u=0;
//************Start Timing**********************
// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 2000.000 kHz
// Mode: Normal top=0xFF
// OC0 output: Disconnected
// Timer Period: 0.1 ms
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (1<<CS01) | (0<<CS00);
TCNT0=0x38;
OCR0=0x00;
}
if(i==2)
{
i=0;
//******************Stop Timing*****************
TCCR0=0;
//******************RPM calculation*************** 
rpmf=60*(10000/u);
rpm=ceil(rpmf); }}                               

// Timer 0 overflow interrupt service routine
//*************CALCULATE PERIODE TIME**************
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
// Reinitialize Timer 0 value
TCNT0=0x38;
u=u+1;
}

// Timer1 overflow interrupt service routine
//*********************TIMER***********************
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{

TCNT1H=0x3CB0 >> 8;
TCNT1L=0x3CB0 & 0xff;
// Place your code here
//Time of holding Start key
//**************calculate Time*************
sec_point++;
sec_point_bar++;
if(sec_point==5)
{sec++;
sec_point=0;}
if(sec_point_bar==5)
{sec_bar++;
sec_point_bar=0;}
//**********************Create Time1**************
if(sec==60)
{
 minute++;
 sec=0;
}
if(minute==60)
{
 hour++;
 minute=0;
}
////***********Create Time 2*************
//if(sec_bar==60)
//{
// minute_bar++;
// sec_bar=0;
//}
//if(minute_bar==60)
//{
// hour_bar++;
// minute_bar=0;
//}
//*************HOLDING START KEY****************
if(start==0)
s++;
if(buzzerf==1)
k++;
//*************HOLDING READ KEY******************
if(PIND.2==0)
z++;
if(buzzeri==1)
alarm++;
//****************RPM ALGORITHM*********************
if((s>=10)&&(s<20))
{                           
lcd_clear();
lcd_gotoxy(8,1);
lcd_puts("start");
buzzer=1;
buzzerf=1;
if(start==1)
{
//*******Reset rpm_varieable*********                               
s=0;
time=0;  
stat1=1;
stat2=1;
stat3=1;
stat4=1;
stat5=1;
a=1;
u=0;
//******Enable rpm algorithm**********
enable=1;
enable_fuel_calc=0;
run1=0;
// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: On
// INT2 Mode: Falling Edge
lcd_clear();
GICR=(0<<INT1) | (0<<INT0) | (1<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (0<<INTF0) | (1<<INTF2);  
 }
 }
 if((s>=20)&&(s<50))
 {
  lcd_clear();
  lcd_gotoxy(0,0);
  lcd_puts("fuel_calc_algorithm");

 if(start==1)
 { 
//*******Enable Fuel Calc Algorithm***********
enable_fuel_calc=1;
enable=0;
//*********Reset fuel_varieables************
s=0;
open_time=0;
hour=0;
minute=0;
sec=0;
run2=0;
run=0;
//*********Off Timer*********
TCCR0=0;
TCCR2=0;
//***********Off Intrrupt*********
// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: On
// INT2 Mode: Falling Edge
lcd_clear();
GICR=(0<<INT1) | (0<<INT0) | (1<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (0<<INTF0) | (1<<INTF2);  
//*****************Set Time************
//rtc_set_time(0,0,0);

 }
 }
 //******clear fuel data*****
 if(s>=50)
 {
  lcd_clear();
  lcd_gotoxy(0,0);
  lcd_puts("Clear Fuel Data");
 if(start==1)
 {
 total_fuel=0;
 total_time=0;
 //*******Enable Fuel Calc Algorithm***********
enable_fuel_calc=1;
enable=0;
//*********Reset fuel_varieables************
s=0;
open_time=0;
hour=0;
minute=0;
sec=0;
fuel=0;
run2=0;
run=0;
//*********Off Timer*********
TCCR0=0;
TCCR2=0;
//***********Off Intrrupt*********
// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: On
// INT2 Mode: Falling Edge
lcd_clear();
GICR=(0<<INT1) | (0<<INT0) | (1<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (0<<INTF0) | (1<<INTF2);
 }}
if(k==15)
{
 buzzer=0;
 k=0;
 buzzerf=0;
}
if((z>=10))
{
                              
// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Falling Edge
// INT1: Off
// INT2: Off
GICR=(0<<INT1) | (1<<INT0) | (0<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (1<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (1<<INTF0) | (0<<INTF2);
r=0;
z=0;
TCCR0=0;
TCCR2=0;
enable=0;
lcd_clear();
lcd_gotoxy(6,1);
lcd_puts("read mode");

}
if(alarm==4)
{
buzzer=0;
alarm=0;
buzzeri=0;
}

}
// Timer2 overflow interrupt service routine
//***************CALCULATE ACCELERATE****************
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
TCNT2=0x64;
// Place your code here
 time=time+1; 
                            
}

void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRA=(0<<DDA7) | (0<<DDA6) | (1<<DDA5) | (1<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTA=(1<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (1<<PORTA3) | (1<<PORTA2) | (1<<PORTA1) | (1<<PORTA0);

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=Out Bit2=In Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (1<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=0 Bit2=T Bit1=T Bit0=T 
PORTB=(1<<PORTB7) | (1<<PORTB6) | (1<<PORTB5) | (1<<PORTB4) | (0<<PORTB3) | (1<<PORTB2) | (1<<PORTB1) | (1<<PORTB0);

// Port C initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTC=(1<<PORTC7) | (1<<PORTC6) | (1<<PORTC5) | (1<<PORTC4) | (1<<PORTC3) | (1<<PORTC2) | (1<<PORTC1) | (1<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=P Bit3=T Bit2=P Bit1=T Bit0=T 
PORTD=(1<<PORTD7) | (1<<PORTD6) | (1<<PORTD5) | (1<<PORTD4) | (1<<PORTD3) | (1<<PORTD2) | (1<<PORTD1) | (1<<PORTD0);
// ADC initialization
// ADC Clock frequency: 125.000 kHz
// ADC Voltage Reference: AVCC pin
// ADC Auto Trigger Source: ADC Stopped
ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);
// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 250.000 kHz
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 0.2 s
// Timer1 Overflow Interrupt: On
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
TCNT1H=0x3C;
TCNT1L=0xB0;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;
// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (1<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);
//******Off Timer0,2*******
TCCR0=0;
TCCR2=0;
//******Off Interrupts*********
// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
// INT2 Mode: Falling Edge
GICR=(0<<INT1) | (0<<INT0) | (1<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (0<<INTF0) | (1<<INTF2);  
// Bit-Banged I2C Bus initialization
// I2C Port: PORTC
// I2C SDA bit: 1
// I2C SCL bit: 0
// Bit Rate: 100 kHz
// Note: I2C settings are specified in the
// Project|Configure|C Compiler|Libraries|I2C menu.
//i2c_init();

// DS1307 Real Time Clock initialization
// Square wave output on pin SQW/OUT: On
// Square wave frequency: 1Hz
//rtc_init(0,1,0);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTD Bit 6
// RD - PORTD Bit 7
// EN - PORTC Bit 2
// D4 - PORTC Bit 4
// D5 - PORTC Bit 5
// D6 - PORTC Bit 6
// D7 - PORTC Bit 7
// Characters/line: 20
lcd_init(20);
// Global enable interrupts
#asm("sei")
lcd_clear();
lcd_gotoxy(2,0);
lcd_puts("High Power Spark");
lcd_gotoxy(6,1);
lcd_puts("WELLCOME");
lcd_gotoxy(1,2);
lcd_puts("SHETAB ALMAS SANAT");
lcd_gotoxy(2,3);
lcd_puts("www.shetabafza.ir");
a=1;
while (1)
      {   
//      if(a==1)
//           {
//           lcd_clear();
//           a=0;}    
       if((enable==1)&&(enable_fuel_calc==0))
           //***********Rpm Calculation Loop******************
           { 
            lcd_gotoxy(6,1);
            sprintf(buf,"rpm:%04d  ",rpm);
            lcd_puts(buf);
            if(g1==0)
            {
             lcd_gotoxy(0,0);
             lcd_puts("2500   Test_1   4000");
            }            
            if(g2==0)
            {
             lcd_gotoxy(0,0);
             lcd_puts("3000   Test_2   5000");
            }
            if(g3==0)
            {
             lcd_gotoxy(0,0);
             lcd_puts("3000   Test_3   4500");
            }
            if(g4==0)
            {
             lcd_gotoxy(0,0);
             lcd_puts("3500   Test_4   4000");
            }
            if(g5==0)
            {
             lcd_gotoxy(0,0);
             lcd_puts("3500   Test_5   3750");
            }
            if((g1==1)&(g2==1)&(g3==1)&(g4==1)&(g5==1))
            {
            lcd_gotoxy(6,0);
            lcd_puts("Test_   ");
            }  
            if(normal==0)
            {
             lcd_gotoxy(6,2);
             lcd_puts(" Normal");
            }  
            if(hps==0)
            {
             lcd_gotoxy(6,2);
             lcd_puts("  HPS   ");
            }                                           
                            
               if((rpm>low_1)&(rpm<low_1+80)&(g1==0))
              {
              //ON Timer
              TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (1<<CS20);
              TCNT2=0x64;
              time=0;   
              stat1=0;
              if(d1==0)
              {
              buzzer=1;
              buzzeri=1;
              }
              if(d1h==0)
              {buzzer=1;
              buzzeri=1;              
               }
               }

           if((rpm>up_1)&(rpm<up_1+80)&(g1==0)&(stat1==0))
           {
           //Off Timer
           TCCR2=0;
           stat1=1;           
           if((normal==0)&(d1==0))
           { 
           //Save Data
           d1=time/100;           
           sprintf(buf,"time:%5.2fs",d1);
           lcd_gotoxy(6,3);
           lcd_puts(buf);
           buzzer=1;
           buzzeri=1;
           }
           if((hps==0)&(d1h==0))
           {
           d1h=time/100;
           sprintf(buf,"time:%5.2fs",d1h);
           lcd_gotoxy(6,3);
           lcd_puts(buf);
           buzzer=1;
           buzzeri=1;           
           }
           }
           if((rpm>low_2)&(rpm<low_2+80)&(g2==0))
            {
              TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (1<<CS20);
              TCNT2=0x64;
              time=0;            
              stat2=0;
              if(d2==0)
              {
              buzzer=1;
              buzzeri=1;
              } 
              if(d2h==0)
              {
              buzzer=1;
              buzzeri=1;
              }                            
           }           
           if((rpm>up_2)&(rpm<up_2+80)&(g2==0)&(stat2==0))
           {
            stat2=1;
            TCCR2=0;
           if((normal==0)&(d2==0))
           { 
           d2=time/100;           
           sprintf(buf,"time:%5.2fs",d2);
           lcd_gotoxy(6,3);
           lcd_puts(buf);
           buzzer=1;
           buzzeri=1;
           }
           if((hps==0)&(d2h==0))
           {
           d2h=time/100;
           sprintf(buf,"time:%5.2fs",d2h);
           lcd_gotoxy(6,3);
           lcd_puts(buf);
           buzzer=1;
           buzzeri=1;           
           }            
          } 
           
           if((rpm>low_3)&(rpm<low_3+80)&(g3==0))
           {
           TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (1<<CS20);
           TCNT2=0x64;
           time=0;            
           stat3=0;
           if(d3==0)
           {
            buzzer=1;
            buzzeri=1;
            } 
           if(d3h==0)
           {
            buzzer=1;
            buzzeri=1;
            }                        
           }
           if((rpm>up_3)&(rpm<up_3+80)&(g3==0)&(stat3==0))
           {
           stat3=1;
           TCCR2=0;
           if((normal==0)&(d3==0))
           { 
           d3=time/100;           
           sprintf(buf,"time:%5.2fs",d3);
           lcd_gotoxy(6,3);
           lcd_puts(buf);
           buzzer=1;
           buzzeri=1;
           }
           if((hps==0)&(d3h==0))
           {
           d3h=time/100;
           sprintf(buf,"time:%5.2fs",d3h);
           lcd_gotoxy(6,3);
           lcd_puts(buf);
           buzzer=1;
           buzzeri=1;           
           }                              
           } 
           if((g4==0)&(rpm>low_4)&(rpm<low_4+80))
           {
           TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (1<<CS20);
           TCNT2=0x64;
           time=0;            
           stat4=0;
           if(d4==0)
           {
            buzzer=1;
            buzzeri=1;
            } 
           if(d4h==0)
           {
            buzzer=1;
            buzzeri=1;
            }              
           }
           if((g4==0)&(rpm>up_4)&(rpm<up_4+80)&(stat4==0))
           {
           stat4=1;
           TCCR2=0;
           if((normal==0)&(d4==0))
           { 
           d4=time/100;           
           sprintf(buf,"time:%5.2fs",d4);
           lcd_gotoxy(6,3);
           lcd_puts(buf);
           buzzer=1;
           buzzeri=1;
           }
           if((hps==0)&(d4h==0))
           {
           d4h=time/100;
           sprintf(buf,"time:%5.2fs",d4h);
           lcd_gotoxy(6,3);
           lcd_puts(buf);
           buzzer=1;
           buzzeri=1;           
           }           
            }
           if((g5==0)&(rpm>low_5)&(rpm<low_5+80))
           {
           stat5=0; 
           //On Timer
           TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (1<<CS22) | (1<<CS21) | (1<<CS20);
           TCNT2=0x64;
           time=0; 
           if(d5==0)
           {
           buzzer=1;
           buzzeri=1;            
           } 
           if(d5h==0)
           {
           buzzer=1;
           buzzeri=1;            
           }           
           
           }
           if((g5==0)&(rpm>up_5)&(rpm<up_5+80)&(stat5==0))
           {
           stat5=1;
           //Off Timer
           TCCR2=0;
           if((normal==0)&(d5==0))
           { 
           //Save Data
           d5=time/100;           
           sprintf(buf,"time:%5.2fs",d5);
           lcd_gotoxy(6,3);
           lcd_puts(buf);
           buzzer=1;
           buzzeri=1;
           }
           if((hps==0)&(d5h==0))
           {
           d5h=time/100;
           sprintf(buf,"time:%5.2fs",d5h);
           lcd_gotoxy(6,3);
           lcd_puts(buf);
           buzzer=1;
           buzzeri=1;           
           }           
           }            
           }
           if(enable_fuel_calc==1)
           {
            //************Fuel_Calculation_Loop****************
            //get Time
            //rtc_get_time(&hour,&minute,&sec);
            //*****CALCULATE_FUEL IN 10 SEC******
            enable=0;
            TCCR0=0;
            TCCR2=0;
            run2++;
            if(run2==0)
            lcd_clear();
                    //External Interrupt(s) initialization
                  // INT0: Off     
                 //INT1: Off
               //INT2: On
              //INT2 Mode: Falling Edge
            GICR=(0<<INT1) | (0<<INT0) | (1<<INT2);
            MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
            MCUCSR=(0<<ISC2);
            GIFR=(0<<INTF1) | (0<<INTF0) | (1<<INTF2);
            
             //******Initialize voltage of cap***********
             if((sec==0)&&(minute==0)&&(hour==0)&&(initialize==0))
             {
             PORTA.5=1;
             }
             //****************Initialize Time**********
             if((sec==3)&&(minute==0)&&(hour==0))
             {
              run_init++;
              if(run_init==1)
              {
              PORTA.5=0;
              initialize=1;
              sec=0;
              sec_point=0;
              minute=0;
              hour=0;
              v0=read_adc(6);
              v0=v0*5/1023;
             }}
             //**********Update Time & Voltage*********
             update_time=2;
             if(((sec==0)&&(minute!=0)&&(initialize==1))||((sec!=0)&&(sec%update_time==0)&&(initialize==1)))
             {
              run++;
              if(run==1){
              //***********Sampling Data***************
//              v1=read_adc(6);
//              delay_ms(1);
//              v2=read_adc(6);
//              delay_ms(1);
//              v3=read_adc(6);
//              delay_ms(1);
//              v4=read_adc(6);
//              delay_ms(1);
//              v5=read_adc(6);
//              delay_ms(1);
//              v6=read_adc(6);
//              delay_ms(1);
//              v7=read_adc(6);
//              delay_ms(1);
//              v8=read_adc(6);
//              delay_ms(1);
//              v9=read_adc(6);
//              delay_ms(1);
//              v10=read_adc(6);
//              delay_ms(1);                           
//              vc=(v1+v2+v3+v4+v5+v6+v7+v8+v9+v10)/10;
              update++;
              vc=read_adc(6);
              vc=vc*5/1023;
              //total_time=ta*log(5/(5-vc));
              open_time=ta*log((v0-5)/(vc-5))+discharge_time; 
              total_time=total_time+open_time;
              fuel_bar=open_time*fuel_index;
             v0=vc;
             discharge_time=0;
              if(update==5)
              {
              total_fuel=total_time*fuel_index;
              update=0;  
             }
             }
             }
             if((sec!=0)&&(initialize==1)&&(sec%update_time!=0))
             run=0;
           //**********Discharge Cap***********
           vc_real=read_adc(6);
           vc_real=vc_real*5/1023;
           if(vc_real>3)
           {
           discharge_time=ta*log((v0-5)/(vc_real-5)); 
           PORTA.5=1;
           delay_ms(10);
           PORTA.5=0;
           v0=read_adc(6);
           v0=v0*5/1023;
           }
//           //*************** Bar_graph_Calculation****************
//            //******Initialize voltage of cap_2***********
//             if((sec_bar==0)&&(minute_bar==0)&&(hour_bar==0)&&(initialize_bar==0))
//             {
//             //discharge_cap
//             PORTA.4=1;
//             }
//             //****************Initialize Time_bar**********
//             if((sec_bar==10)&&(minute_bar==0)&&(hour_bar==0))
//             {
//              run_init_bar++;
//              if(run_init_bar==1)
//              {
//              PORTA.4=0;
//              initialize_bar=1;
//              sec_bar=0;
//              sec_point_bar=0;
//              minute_bar=0;
//              hour_bar=0;
//             }}
//           
//             //**********Update Time & Voltage_BAR*********
//             update_time_bar=2;
//             if(((sec_bar==0)&&(minute_bar!=0)&&(initialize_bar==1))||((sec_bar!=0)&&(sec_bar%update_time==0)&&(initialize_bar==1)))
//             {
//              run_bar++;
//              if(run_bar==1){
//              vc_bar=read_adc(7);                           
//              vc_bar=vc_bar*5/1023;
//              open_time_bar=ta_bar*log(5/(5-vc_bar));
//              fuel_bar=open_time_bar*fuel_index;  
//             }}
//             if((sec_bar!=0)&&(initialize_bar==1)&&(sec_bar%update_time_bar!=0))
//             run_bar=0;  
//             //**************Discaharge cap*****************
//             if(vc_bar>3)
//             {
//             temp=sec_bar;
//             PORTA.4=1;
//             }
//             if(sec_bar==temp+1)
//             PORTA.4=0;
           //**************Show Resault************
           lcd_gotoxy(0,0);
           sprintf(buf,"%d:%02d:%02d",hour,minute,sec);
           lcd_puts(buf);
           sprintf(buf,"%6.3fcc",fuel_bar);
           lcd_gotoxy(9,0);
           lcd_puts(buf);
           sprintf(buf,"%6.3fcc",total_fuel);
           lcd_gotoxy(9,1);
           lcd_puts(buf);
           lcd_gotoxy(0,1) ;
           lcd_puts("FUEL:");   
           sprintf(buf,"%6.3fV",vc);
           lcd_gotoxy(9,2);
           lcd_puts(buf); 
           lcd_gotoxy(0,2);
           lcd_puts("VOLTAGE:");
           //****************Show Bar Graph********************
           min_fuel=1;
           max_fuel=12;
           step_bar=(max_fuel-min_fuel)/19;
           amp_bar=ceil(fuel_bar/step_bar);
           if(amp_bar>=19)
           amp_bar=19;
           for(bar=0;bar<=amp_bar;bar++)
           {
            lcd_gotoxy(bar,3);
            //define_char(char_0,0);
            lcd_putchar('#');
           }
           //****************Reset Algorithm***********
//           if((hour==1)&&(sec==5)&&(minute==0))
//           {
//           buzzer=0;
//           PORTA.5=0;
//           fuel=0;
//           total_fuel=0;
//           vc=0;
//           v0=0;
//           //rtc_set_time(0,0,0);
//           }
           }
           }
           }
