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
#include <stdlib.h>
#include "string.h"
#include "defines.h"
#include "rgb_bulb.h"
#include "bulb.h"

#ifdef PC_SIM
#include "SDL.h"
#include <SDL_draw.h>
#endif


typedef struct
{
  uint8_t first_color_index;
  uint8_t last_color_index;
  rgb_bulb_color_type *p_color_buff_start;

  uint8_t first_behavior_index;
  uint8_t last_behavior_index;
  bulb_behavior_type *p_behavior_buff_start;
} string_type;

static string_type strings[NUM_STRINGS];
static rgb_bulb_color_type bulb_color[NUM_STRINGS][BULBS_PER_STRING];
static bulb_behavior_type bulb_behavior[NUM_STRINGS][BULBS_PER_STRING];
static fade_status_type fade_status = FADE_OFF;

#ifdef PC_SIM
extern SDL_Surface *screen;
#endif

void StringRotateStrings( uint8_t right )
{
  string_type string;
  uint8_t i;

  if( right )
  {
    string = strings[NUM_STRINGS - 1];

    for( i = NUM_STRINGS - 1; i > 0; i-- )
    {
      strings[i] = strings[i-1];
    }

    strings[0] = string;
  }
  else
  {
    string = strings[0];

    for( i = 0; i < NUM_STRINGS - 1; i++ )
    {
      strings[i] = strings[i+1];
    }

    strings[NUM_STRINGS - 1] = string;
  } 
}

void StringRotateBulbColor( uint8_t up, uint8_t string_num )
{
  string_type *p_string = &strings[string_num];

  if( up )
  {
    if( p_string->first_color_index == BULBS_PER_STRING - 1 )
      p_string->first_color_index = 0;
    else
      p_string->first_color_index++; 
      
    if( p_string->last_color_index == BULBS_PER_STRING - 1 )
      p_string->last_color_index = 0;
    else
      p_string->last_color_index++;
  }
  else
  {
    if( p_string->first_color_index == 0 )
      p_string->first_color_index = BULBS_PER_STRING - 1;
    else
      p_string->first_color_index--;
      
    if( p_string->last_color_index == 0 )
      p_string->last_color_index = BULBS_PER_STRING - 1;
    else
      p_string->last_color_index--;
  }
}

void StringRotateBulbBehavior( uint8_t up, uint8_t string_num )
{
  string_type *p_string = &strings[string_num];

  if( up )
  {
    if( p_string->first_behavior_index == BULBS_PER_STRING - 1 )
      p_string->first_behavior_index = 0;
    else
      p_string->first_behavior_index++; 
      
    if( p_string->last_behavior_index == BULBS_PER_STRING - 1 )
      p_string->last_behavior_index = 0;
    else
      p_string->last_behavior_index++;
  }
  else
  {
    if( p_string->first_behavior_index == 0 )
      p_string->first_behavior_index = BULBS_PER_STRING - 1;
    else
      p_string->first_behavior_index--;
      
    if( p_string->last_behavior_index == 0 )
      p_string->last_behavior_index = BULBS_PER_STRING - 1;
    else
      p_string->last_behavior_index--;
  }

}

rgb_bulb_color_type StringPushBulbColor( uint8_t top, uint8_t string_num, rgb_bulb_color_type color )
{
  string_type *p_string = &strings[string_num];
  rgb_bulb_color_type popped_color;
  rgb_bulb_color_type *p_color_buff = p_string->p_color_buff_start;

  if( top )
  {
    popped_color = p_color_buff[p_string->last_color_index];
    StringRotateBulbColor( DOWN, string_num );
    p_color_buff[p_string->first_color_index] = color;
  }
  else
  {
    popped_color = p_color_buff[p_string->first_color_index];
    StringRotateBulbColor( UP, string_num );
    p_color_buff[p_string->last_color_index] = color;
  }

  return( popped_color );
}

void StringPushBulbColorAll( uint8_t top, rgb_bulb_color_type color )
{
  uint8_t i;

  for( i = 0; i < NUM_STRINGS; i++ )
  {
    StringPushBulbColor( top, i, color );
  }
}
 
bulb_behavior_type StringPushBulbBehavior( uint8_t top, uint8_t string_num, bulb_behavior_type *p_behavior )
{
  string_type *p_string = &strings[string_num];
  bulb_behavior_type popped_behavior;
  bulb_behavior_type *p_behavior_buff = p_string->p_behavior_buff_start;

  p_behavior->current_intensity = p_behavior->intensity;
  p_behavior->current_fade_direction = FADE_DOWN;

  if( top )
  {
    popped_behavior = p_behavior_buff[p_string->last_behavior_index];
    StringRotateBulbBehavior( DOWN, string_num );
    p_behavior_buff[p_string->first_behavior_index] = *p_behavior;
  }
  else
  {
    popped_behavior = p_behavior_buff[p_string->first_behavior_index];
    StringRotateBulbBehavior( UP, string_num );
    p_behavior_buff[p_string->last_behavior_index] = *p_behavior;
  }

  return( popped_behavior );
}

void StringPushBulbBehaviorAll( uint8_t top, bulb_behavior_type *p_behavior )
{
  uint8_t i;

  for( i = 0; i < NUM_STRINGS; i++ )
  {
    StringPushBulbBehavior( top, i, p_behavior );
  }
}

void StringSetBulbColor( uint8_t string_num, uint8_t bulb_num, rgb_bulb_color_type color )
{
  uint8_t bulb_index;

  if( string_num >= NUM_STRINGS || bulb_num >= BULBS_PER_STRING )
    return;

  bulb_index = strings[string_num].first_color_index + bulb_num;
  if( bulb_index > BULBS_PER_STRING - 1 )
  {
    bulb_index -= BULBS_PER_STRING;
  }

  strings[string_num].p_color_buff_start[ bulb_index ] = color;
}

void StringSetLineColor( uint8_t line_num, rgb_bulb_color_type color )
{
  uint8_t i;

  if( line_num >= BULBS_PER_STRING )
    return;

  for( i = 0; i < NUM_STRINGS; i++ )
  {
    StringSetBulbColor( i, line_num, color );
  }
}

void StringAllOnColor( rgb_bulb_color_type color )
{
  uint8_t i;
  bulb_behavior_type behavior;

  for( i = 0; i < BULBS_PER_STRING; i++ )
  {
    behavior.mode = ON;
    behavior.intensity = RGB_BULB_DEFAULT_INTENSITY;
    StringPushBulbBehaviorAll( TOP, &behavior );
    StringPushBulbColorAll( TOP, color );
  }
}

void StringAllOff( void )
{
  uint8_t i;
  bulb_behavior_type behavior;

  for( i = 0; i < BULBS_PER_STRING; i++ )
  {
    behavior.mode = OFF;
    StringPushBulbBehaviorAll( TOP, &behavior );
    StringPushBulbColorAll( TOP, RGB_BULB_COLOR_BLACK );
  } 
}

void StringFadeAllOff( uint8_t fade_rate )
{
  uint8_t i,j;
  uint8_t behavior_index;
  bulb_behavior_type *p_behavior_buff;

  fade_status = FADING;

  for( i = 0; i < NUM_STRINGS; i++ )
  {
    behavior_index = strings[i].first_behavior_index;
    p_behavior_buff = strings[i].p_behavior_buff_start;
    
    for( j = 0; j < BULBS_PER_STRING; j++ )
    {
      p_behavior_buff[behavior_index].mode = FADE_OFF;
      p_behavior_buff[behavior_index].fade_rate = fade_rate;

      if( behavior_index == BULBS_PER_STRING - 1 )
       behavior_index = 0;
      else
       behavior_index++;
    }
  } 
}

fade_status_type StringGetFadeStatus( void )
{
  return fade_status;
}

#ifdef PC_SIM
void DumpStrings( void )
{
  uint8_t i,j;
  bulb_behavior_type *p_behavior_buff;
  rgb_bulb_color_type *p_color_buff;
  string_type *p_string;

  for( i = 0; i < NUM_STRINGS; i++ )
  {
    p_string = &strings[i];
    p_behavior_buff = p_string->p_behavior_buff_start;
    p_color_buff = p_string->p_color_buff_start;

    printf("String: %d, FirstBehaviorIndex: %d, LastBehaviorIndex: %d ",
      i, p_string->first_behavior_index, p_string->last_behavior_index );

    printf("FirstColorIndex: %d, LastColorIndex: %d\n\r\n\r",
      p_string->first_color_index, p_string->last_color_index );

    for( j = 0; j < BULBS_PER_STRING; j++ )
    {
//      printf("Index: %d, Mode: %d, Duration: %d, Cnt: %d\n\r", 
//        j, p_behavior_buff[j].mode, p_behavior_buff[j].duration, 
//        p_behavior_buff[j].cnt );
    }
    printf("\n\r");

    for( j = 0; j < BULBS_PER_STRING; j++ )
    {
      printf("Index: %d, Red: %d, Green: %d, Blue: %d\n\r", 
        j, p_color_buff[j] & 0xF, ( p_color_buff[j] >> 4 ) & 0xF , ( p_color_buff[j] >> 8 ) & 0xF  );
    }
    printf("\n\r");
  }
}
#endif

void StringDraw( void )
{
  uint8_t i,j;
  uint8_t color_index;
  uint8_t behavior_index;
  rgb_bulb_color_type *p_color_buff;
  bulb_behavior_type *p_behavior_buff;
  string_type *p_string;
  fade_status_type fade_status_local = FADE_COMPLETE;

  for( i = 0; i < NUM_STRINGS; i++ )
  {
    p_string = &strings[i];
    p_color_buff = p_string->p_color_buff_start;
    p_behavior_buff = p_string->p_behavior_buff_start;
    color_index = p_string->first_color_index;
    behavior_index = p_string->first_behavior_index;

    for( j = 0; j < BULBS_PER_STRING; j++ )
    {
      bulb_behavior_type *p_behavior = &p_behavior_buff[behavior_index];
      rgb_bulb_color_type color = p_color_buff[ color_index ];

      switch( p_behavior->mode )
      {
        case ON:
        {
          break;
        }

        case OFF:
        {
          color = RGB_BULB_COLOR_BLACK;
          break;
        }

        case BLINK:
        {
          if( rand() % 2 )
            color = RGB_BULB_COLOR_BLACK;
            
          break;
        }

        case TWINKLE:
        {
          if( p_behavior->current_intensity != RGB_BULB_DEFAULT_INTENSITY )
            p_behavior->current_intensity = RGB_BULB_DEFAULT_INTENSITY;
          else
            p_behavior->current_intensity = ( ( rand() % 100 ) > 90 ) ? 0 : RGB_BULB_DEFAULT_INTENSITY;
          break;
        }

        case FADE_OFF:
        {
          int16_t temp;

          if( p_behavior->current_intensity > 0 )
          {
            temp = p_behavior->current_intensity - p_behavior->fade_rate;

            if( temp < 0 )
              temp = 0;

            p_behavior->current_intensity = (uint8_t)temp;
          }

          break;
        }
 
        case FADE_ON_OFF:
        {
          int16_t temp;

          if( p_behavior->current_fade_direction == FADE_DOWN )
          {
            temp = p_behavior->current_intensity - p_behavior->fade_rate;

            if( temp < 0 )
            {
              p_behavior->current_intensity = 0;
              p_behavior->current_fade_direction = FADE_UP;
            }
            else
              p_behavior->current_intensity = (uint8_t)temp; 
          }
          else
          {
            temp = p_behavior->current_intensity + p_behavior->fade_rate;

            if( temp > p_behavior->intensity )
            {
              p_behavior->current_intensity = p_behavior->intensity;
              p_behavior->current_fade_direction = FADE_DOWN;
            }
            else
              p_behavior->current_intensity = (uint8_t)temp; 

          }
          break;
        }

        default:
          break;
      }

      if( p_behavior->current_intensity > 0 )
        fade_status_local = FADING;

      BulbSetColor( i, j, color, p_behavior->current_intensity );

      if( color_index == BULBS_PER_STRING - 1 )
      {
        color_index = 0;
      }
      else
        color_index++;

      if( behavior_index == BULBS_PER_STRING - 1 )
      {
        behavior_index = 0;
      }
      else
        behavior_index++;
    }
  }

  fade_status = fade_status_local;
#ifdef PC_SIM
  SDL_UpdateRect(screen, 0, 0, 0, 0);
#endif
}

void StringInit( void )
{
  uint8_t i,j;

  /* Initialize string array */
  for( i = 0; i < NUM_STRINGS; i++ )
  {
    string_type *p_string = &strings[i];

    p_string->first_color_index = 0;
    p_string->last_color_index = BULBS_PER_STRING - 1;
    p_string->p_color_buff_start = &bulb_color[i][0];

    p_string->first_behavior_index = 0;
    p_string->last_behavior_index = BULBS_PER_STRING - 1;
    p_string->p_behavior_buff_start = &bulb_behavior[i][0];

    for( j = 0; j < BULBS_PER_STRING; j++ )
    {
      bulb_behavior_type z_bulb_behavior;

      StringPushBulbColor( TOP, i, 
        rgb_bulb_color( rand() % 16, rand() % 16, rand() % 16 ) );

      z_bulb_behavior.mode = ON;
      z_bulb_behavior.intensity = RGB_BULB_DEFAULT_INTENSITY;
      z_bulb_behavior.fade_rate = 0;
      z_bulb_behavior.current_fade_direction = FADE_DOWN;
      z_bulb_behavior.current_intensity = z_bulb_behavior.intensity;

      StringPushBulbBehavior( TOP, i, &z_bulb_behavior );
    }
  }
}
