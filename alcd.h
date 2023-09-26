#ifndef _MEGA32A_INCLUDED_
#include <mega32a.h>
#endif

#ifndef _DELAY_INCLUDED_
#include <delay.h>
#endif

#ifndef _LCDX_INCLUDED_
#include <alcd.h>
#endif

#pragma used+

#ifndef _NEW_ALCD_INCLUDED
#define _NEW_ALCD_INCLUDED
#endif

flash unsigned char char_0[8]={31,31,31,31,31,31,31,31};
flash unsigned char char_1[8]={14,27,17,17,25,29,31,31};
flash unsigned char char_2[8]={14,27,17,25,29,31,31,31};
flash unsigned char char_3[8]={14,31,31,31,31,31,31,31};



void lcd_clearxy(unsigned int X,unsigned int Y)
{
lcd_gotoxy(X,Y);
lcd_puts(" ");
}
void lcd_clearrow(int ROW)
{
 lcd_gotoxy(0,ROW);
 lcd_puts("                ");
}
void lcd_clearrowxy(int X1,int X2,int ROW)
{
 while(X1!=X2)
 {
 lcd_gotoxy(X1,ROW);
 lcd_puts(" ");
 X1++;
 }
 lcd_puts(" ");
}
void lcd_clear_anim()
{
 int _COUNTER;
  
 lcd_gotoxy(0,0);
 for(_COUNTER=0;_COUNTER<=32;_COUNTER++) 
 {
 lcd_puts(" ");  
 delay_ms(100);
 }
}
void cursor_blinking()
{
 _lcd_write_data(0x0F);
}
void lcd_black()
{
_lcd_write_data(0x08);
}
void lcd_cursor_in_home()
{
 lcd_gotoxy(0,0);
}
void lcd_shift_right()
{
 _lcd_write_data(0x1C);
}
void lcd_shift_left()
{
 _lcd_write_data(0x18);
}

void define_char(unsigned char flash *pc,unsigned char char_code)
{
unsigned char i,a;
a=(char_code<<3) | 0x40;
for (i=0; i<8; i++) lcd_write_byte(a++,*pc++);
}
void battery(int STATE)
{
 if(STATE==1){define_char(char_0,0);lcd_gotoxy(15,0);lcd_putchar(0);}
 if(STATE==2){define_char(char_1,1);lcd_gotoxy(15,0);lcd_putchar(1);}
 if(STATE==3){define_char(char_2,2);lcd_gotoxy(15,0);lcd_putchar(2);}
 if(STATE==4){define_char(char_3,3);lcd_gotoxy(15,0);lcd_putchar(3);}
}
void backlight_on()
{
 DDRC.3=1;
 PORTC.3=1;
}
void backlight_off()
{
 DDRC.3=1;
 PORTC.3=0;
}

#pragma used-

