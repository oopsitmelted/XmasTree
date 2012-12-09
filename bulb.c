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

#include <stdint.h>
#include <math.h>
#include "defines.h"
#include "rgb_bulb.h"
#include "bulb.h"

#ifdef PC_SIM
#include "SDL.h"
#include <SDL_draw.h>
#else
#include "rgb_bulb.h"
#endif

#ifdef PC_SIM
typedef struct
{
  uint16_t x;
  uint16_t y;
  uint8_t phys_string_num;
  uint8_t id;
} bulb_xy_type;

static bulb_xy_type bulb_xy[NUM_STRINGS][BULBS_PER_STRING];
extern SDL_Surface *screen;
#endif

void BulbSetColor( uint8_t string_num, uint8_t bulb_id, rgb_bulb_color_type color, uint8_t intensity )
{
#ifndef PC_SIM
  uint8_t phys_string_num;
#endif

  if( string_num >= NUM_STRINGS )
    return;

#ifdef PC_SIM
  uint8_t r = color & 0xF;
  uint8_t g = ( color >> 4 ) & 0xF;
  uint8_t b = ( color >> 8 ) & 0xF;

  r = (uint8_t)( (uint16_t)r * intensity / RGB_BULB_DEFAULT_INTENSITY );
  g = (uint8_t)( (uint16_t)g * intensity / RGB_BULB_DEFAULT_INTENSITY );
  b = (uint8_t)( (uint16_t)b * intensity / RGB_BULB_DEFAULT_INTENSITY );

  bulb_xy_type *p_bulb_xy = &bulb_xy[string_num][bulb_id];
  Draw_FillCircle( screen, p_bulb_xy->x, p_bulb_xy->y + BULB_RADIUS, 
    5, SDL_MapRGB(screen->format, r << 4, g << 4, b << 4) );
#else

  if( string_num == 0 || string_num == 1 )
    phys_string_num = 0;
  else
    phys_string_num = 1;

  if( string_num == 1 || string_num == 2 )
    bulb_id = 17 - bulb_id;
  else
    bulb_id = 18 + bulb_id;

    rgb_bulb_set_color( phys_string_num, bulb_id, intensity, color );
#endif
}

#ifdef PC_SIM
static void BulbXYInit( void )
{
  uint8_t i,j;
  float angle = ANGLE;

  /* Init Bulb XY positions */
  for( i = 0; i < NUM_STRINGS; i++ )
  {
    uint8_t phys_string_num;
    float dx = cos( angle ) * BULB_SPACING;
    float dy = sin( angle ) * BULB_SPACING;

    phys_string_num = i < ( NUM_STRINGS / 2 ) ? 0 : 1;

    for( j = 0; j < BULBS_PER_STRING; j++ )
    {
      bulb_xy_type *p_bulb_xy = &bulb_xy[NUM_STRINGS - 1 - i][j];

      p_bulb_xy->x = (uint16_t)( SCREEN_X / 2 + dx * j );
      p_bulb_xy->y = (uint16_t)( abs( dy * j ) );
      p_bulb_xy->phys_string_num = phys_string_num;

      if( ( i % NUM_STRINGS  == 1 ) || ( i % NUM_STRINGS == 2 ) )
      {
        p_bulb_xy->id = BULBS_PER_STRING + j;
      }
      else
      {
        p_bulb_xy->id = BULBS_PER_STRING - 1 - j;
      }

//      printf("i: %d, j: %d, phys_string: %d, id: %d\n\r", i, j, p_bulb_xy->phys_string_num,
//        p_bulb_xy->id );
    }

    angle += SEPARATION_ANGLE;
  }
}
#endif

void BulbInit( void )
{
#ifdef PC_SIM
  BulbXYInit();
#else
  rgb_bulb_fill_color(0, 0, RGB_BULB_LIGHT_COUNT, 0, RGB_BULB_COLOR_BLACK );
  rgb_bulb_fill_color(1, 0, RGB_BULB_LIGHT_COUNT, 0, RGB_BULB_COLOR_BLACK );
#endif
}
