/*!	Christmas Light Control
**	By Robert Quattlebaum <darco@deepdarc.com>
**	Released November 27th, 2010
**
**	For more information,
**	see <http://www.deepdarc.com/2010/11/27/hacking-christmas-lights/>.
**
**	Originally intended for the ATTiny13, but should
**	be easily portable to other microcontrollers.
*/

#include "defines.h"
#include "rgb_bulb.h"

#ifndef PC_SIM
#include <avr/io.h>
#include <util/delay.h>

#define sbi(x,y)	x|=(1<<y)
#define cbi(x,y)	x&=~(1<<y)

#define RGB_BULB_PIN_STRING0	7
#define RGB_BULB_PIN_STRING1	6
#define RGB_BULB_PORT	PORTD
#define RGB_BULB_DDR	DDRD

static void rgb_bulb_begin(uint8_t pin) 
{
  sbi(RGB_BULB_DDR,pin);
  sbi(RGB_BULB_PORT,pin);
  _delay_us(10);
  cbi(RGB_BULB_PORT,pin);
}

static void rgb_bulb_one(uint8_t pin) 
{
  cbi(RGB_BULB_PORT,pin);
  _delay_us(20);
  sbi(RGB_BULB_PORT,pin);
  _delay_us(8);
  cbi(RGB_BULB_PORT,pin);
}

static void rgb_bulb_zero(uint8_t pin) 
{
  cbi(RGB_BULB_PORT,pin);
  _delay_us(10);
  sbi(RGB_BULB_PORT,pin);
  _delay_us(20);
  cbi(RGB_BULB_PORT,pin);
}

static void rgb_bulb_end(uint8_t pin) 
{
  cbi(RGB_BULB_PORT,pin);
  _delay_us(30);
}

void rgb_bulb_set_color(uint8_t phys_string_num, uint8_t led,uint8_t intensity,rgb_bulb_color_type color) 
{
  uint8_t i;
  uint8_t pin;

  if( phys_string_num > 1 )
    return;

  pin = phys_string_num ? RGB_BULB_PIN_STRING1 : RGB_BULB_PIN_STRING0;

  rgb_bulb_begin( pin );

  for(i=6;i;i--,(led<<=1))
    if(led&(1<<5))
      rgb_bulb_one( pin );
    else
      rgb_bulb_zero( pin );

  for(i=8;i;i--,(intensity<<=1))
    if(intensity&(1<<7))
      rgb_bulb_one( pin );
    else
      rgb_bulb_zero( pin );

  for(i=12;i;i--,(color<<=1))
    if(color&(1<<11))
      rgb_bulb_one( pin );
    else
    rgb_bulb_zero( pin );

  rgb_bulb_end( pin );
}
#endif


rgb_bulb_color_type rgb_bulb_color(uint8_t r,uint8_t g,uint8_t b) 
{
  return RGB_BULB_COLOR(r,g,b);
}

rgb_bulb_color_type rgb_bulb_color_hue(uint8_t h) 
{
  switch(h>>4) 
  {
    case 0:	h-=0; return rgb_bulb_color(h,RGB_BULB_CHANNEL_MAX,0);
    case 1:	h-=16; return rgb_bulb_color(RGB_BULB_CHANNEL_MAX,(RGB_BULB_CHANNEL_MAX-h),0);
    case 2:	h-=32; return rgb_bulb_color(RGB_BULB_CHANNEL_MAX,0,h);
    case 3:	h-=48; return rgb_bulb_color((RGB_BULB_CHANNEL_MAX-h),0,RGB_BULB_CHANNEL_MAX);
    case 4:	h-=64; return rgb_bulb_color(0,h,RGB_BULB_CHANNEL_MAX);
    case 5:	h-=80; return rgb_bulb_color(0,RGB_BULB_CHANNEL_MAX,(RGB_BULB_CHANNEL_MAX-h));
  }

  return rgb_bulb_color(0,0,0);
}

