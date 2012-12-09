/*!	Christmas Light Control Header
**	By Robert Quattlebaum <darco@deepdarc.com>
**	Released November 27th, 2010
**
**	For more information,
**	see <http://www.deepdarc.com/2010/11/27/hacking-christmas-lights/>.
**
**	Originally intended for the ATTiny13, but should
**	be easily portable to other microcontrollers.
*/

#ifndef __RGB_BULB_H__
#define __RGB_BULB_H__	1

#include <stdint.h>

#define RGB_BULB_LIGHT_COUNT		(36)
#define RGB_BULB_CHANNEL_MAX		(0xF)
#define RGB_BULB_DEFAULT_INTENSITY	(0xCC)
#define RGB_BULB_HUE_MAX		((RGB_BULB_CHANNEL_MAX+1)*6-1)

#define RGB_BULB_COLOR(r,g,b)	((r)+((g)<<4)+((b)<<8))

#define RGB_BULB_COLOR_WHITE	RGB_BULB_COLOR(RGB_BULB_CHANNEL_MAX,RGB_BULB_CHANNEL_MAX,RGB_BULB_CHANNEL_MAX)
#define RGB_BULB_COLOR_BLACK	RGB_BULB_COLOR(0,0,0)
#define RGB_BULB_COLOR_RED	RGB_BULB_COLOR(RGB_BULB_CHANNEL_MAX,0,0)
#define RGB_BULB_COLOR_GREEN	RGB_BULB_COLOR(0,RGB_BULB_CHANNEL_MAX,0)
#define RGB_BULB_COLOR_BLUE	RGB_BULB_COLOR(0,0,RGB_BULB_CHANNEL_MAX)
#define RGB_BULB_COLOR_CYAN	RGB_BULB_COLOR(0,RGB_BULB_CHANNEL_MAX,RGB_BULB_CHANNEL_MAX)
#define RGB_BULB_COLOR_MAGENTA	RGB_BULB_COLOR(RGB_BULB_CHANNEL_MAX,0,RGB_BULB_CHANNEL_MAX)
#define RGB_BULB_COLOR_YELLOW	RGB_BULB_COLOR(RGB_BULB_CHANNEL_MAX,RGB_BULB_CHANNEL_MAX,0)

typedef uint16_t rgb_bulb_color_type;

void rgb_bulb_set_color(uint8_t phys_string_num, uint8_t led,uint8_t intensity,rgb_bulb_color_type color);
rgb_bulb_color_type rgb_bulb_color(uint8_t r,uint8_t g,uint8_t b);
rgb_bulb_color_type rgb_bulb_color_hue(uint8_t h);

static inline void
rgb_bulb_fill_color( uint8_t phys_string_num, uint8_t begin,uint8_t count,uint8_t intensity,rgb_bulb_color_type color)
{
	while(count--)
		rgb_bulb_set_color(phys_string_num,begin++,intensity,color);
}


#endif // #ifndef __RGB_BULB_H__
