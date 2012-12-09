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

#ifndef STRING_H
#define STRING_H

#include "rgb_bulb.h"
#include "bulb.h"

#define UP 1
#define DOWN 0
#define TOP 1
#define BOTTOM 0
#define RIGHT 1
#define LEFT 0

typedef enum
{
  FADING,
  FADE_COMPLETE
} fade_status_type;

rgb_bulb_color_type StringPushBulbColor( uint8_t top, uint8_t string_num, rgb_bulb_color_type color );
void StringRotateStrings( uint8_t right );
void StringCopyString( uint8_t string_num_from, uint8_t string_num_to );
void StringPushBulbColorAll( uint8_t top, rgb_bulb_color_type color );
void StringPushBulbBehaviorAll( uint8_t top, bulb_behavior_type *p_behavior );
bulb_behavior_type StringPushBulbBehavior( uint8_t top, uint8_t string_num, bulb_behavior_type *p_behavior );
void StringRotateBulbColor( uint8_t up, uint8_t string_num );
void StringRotateBulbBehavior( uint8_t up, uint8_t string_num );
void StringSetBulbColor( uint8_t string_num, uint8_t bulb_num, rgb_bulb_color_type color );
void StringSetLineColor( uint8_t line_num, rgb_bulb_color_type color );
void StringAllOnColor( rgb_bulb_color_type color );
void StringAllOff( void );
void StringFadeAllOff( uint8_t fade_rate );
fade_status_type StringGetFadeStatus( void );
void DumpStrings( void );
void StringDraw( void );
void StringInit( void );

#endif
