/*   RGB Xmas Tree	
*    Scott Stickeler (sstickeler@gmail.com)
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef BULB_H
#define BULB_H

#include <stdint.h>

typedef enum 
{
  OFF, ON, BLINK, TWINKLE, FADE_ON, FADE_OFF, FADE_ON_OFF
} bulb_mode_e_type;

typedef enum
{
  FADE_UP,
  FADE_DOWN
} bulb_fade_direction_e_type;

typedef struct
{
  bulb_mode_e_type mode;
  uint8_t intensity;
  uint8_t fade_rate;

  /* private */
  bulb_fade_direction_e_type current_fade_direction;
  uint8_t current_intensity;
} bulb_behavior_type;

void BulbInit( void );
void BulbSetColor( uint8_t string_num, uint8_t bulb_id, rgb_bulb_color_type color, uint8_t intensity );

#endif
