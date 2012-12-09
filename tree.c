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

#include <stdlib.h>
#include <stdint.h>
#include "defines.h"
#include "rgb_bulb.h"
#include "bulb.h"
#include "string.h"

#ifdef PC_SIM
#include "SDL.h"
#include <SDL_draw.h>

SDL_Surface *screen;
#else
#include <util/delay.h>
#endif

uint32_t tick_cnt = 0;
uint32_t tick_cnt_threshold = TICK_CNT_THRESHOLD_DEFAULT;

void Init( void )
{
  /* Init Bulbs */
  BulbInit();

  /* Init Strings */
  StringInit();
}

void SequenceRandomHue( uint8_t init )
{
  if( init )
    tick_cnt_threshold = TICK_CNT_THRESHOLD_DEFAULT;
  
  StringPushBulbColor( TOP, 0, rgb_bulb_color_hue( rand() % RGB_BULB_HUE_MAX ) ); 
  StringPushBulbColor( TOP, 1, rgb_bulb_color_hue( rand() % RGB_BULB_HUE_MAX ) ); 
  StringPushBulbColor( TOP, 2, rgb_bulb_color_hue( rand() % RGB_BULB_HUE_MAX ) ); 
  StringPushBulbColor( TOP, 3, rgb_bulb_color_hue( rand() % RGB_BULB_HUE_MAX ) ); 
}

void SequenceRandomRGB( uint8_t init )
{
  if( init )
    tick_cnt_threshold = TICK_CNT_THRESHOLD_DEFAULT;

  StringPushBulbColor( TOP, 0, 
    rgb_bulb_color( rand() % 16, rand() % 16, rand() % 16 ) ); 
  StringPushBulbColor( TOP, 1, 
    rgb_bulb_color( rand() % 16, rand() % 16, rand() % 16 ) ); 
  StringPushBulbColor( TOP, 2, 
    rgb_bulb_color( rand() % 16, rand() % 16, rand() % 16 ) ); 
  StringPushBulbColor( TOP, 3, 
    rgb_bulb_color( rand() % 16, rand() % 16, rand() % 16 ) ); 
}

void SequenceRainbow( uint8_t init )
{
  static uint8_t i;
  rgb_bulb_color_type hue_color;

  if( init )
  {
    tick_cnt_threshold = TICK_CNT_THRESHOLD_DEFAULT;
    i = 0;
  }

  hue_color = rgb_bulb_color_hue( i++ % RGB_BULB_HUE_MAX );
  StringPushBulbColor( TOP, 0, hue_color ); 
  StringPushBulbColor( TOP, 1, hue_color ); 
  StringPushBulbColor( TOP, 2, hue_color ); 
  StringPushBulbColor( TOP, 3, hue_color ); 
}

void SequenceRainbowSnake( uint8_t init )
{
  static uint8_t i;
  rgb_bulb_color_type hue_color;
  rgb_bulb_color_type color;

  if( init )
  {
    tick_cnt_threshold = TICK_CNT_THRESHOLD_DEFAULT;
    i = 0;
  }

  hue_color = rgb_bulb_color_hue( i++ % RGB_BULB_HUE_MAX );
  color = StringPushBulbColor( TOP, 0, hue_color ); 
  color = StringPushBulbColor( TOP, 1, color ); 
  color = StringPushBulbColor( TOP, 2, color ); 
  StringPushBulbColor( TOP, 3, color ); 
}

void SequenceRainbowRotate( uint8_t init )
{
  static uint8_t i,j;

  if( init )
  {
    tick_cnt_threshold = TICK_CNT_THRESHOLD_DEFAULT;
    i = 0;
    j = 0;
  }

  StringRotateStrings( RIGHT );
  for( i = 0; i < BULBS_PER_STRING; i++ )
  {
    StringPushBulbColor( TOP, 0, rgb_bulb_color_hue( j % RGB_BULB_HUE_MAX ) );
  }

  j+=4;  
}

void SequenceFallingBlueWhite( uint8_t init )
{
  static uint8_t i;
  static uint8_t build_up;
  static uint8_t draw_line;
  static uint8_t current_color;

  if( init )
  {
    tick_cnt_threshold = 200;
    build_up = TRUE;
    draw_line = BULBS_PER_STRING - 1;
    current_color = 0;
  }

  if( i != 0 )
    StringSetLineColor( i - 1, RGB_BULB_COLOR_BLACK );

  StringSetLineColor( i, current_color ? RGB_BULB_COLOR_WHITE : RGB_BULB_COLOR_BLUE );

  i++;
  if( i == draw_line + 1 )
  {
    i = 0; 

    if( draw_line > 0 )
      draw_line--;
    else
      draw_line = BULBS_PER_STRING - 1;

    current_color ^=1;
  }
}

void SequenceGreenWithWhite( uint8_t init )
{
  uint8_t i;
  static uint8_t j;
  static struct
  {
    uint8_t string_num;
    uint8_t bulb_num;
  } white_bulbs[25];

  if( init )
  {
    tick_cnt_threshold = TICK_CNT_THRESHOLD_DEFAULT;
    StringAllOnColor( RGB_BULB_COLOR_GREEN );
   
    for( i = 0; i < 25; i++ )
    {
      white_bulbs[i].string_num = rand() % 4;
      white_bulbs[i].bulb_num = rand() % BULBS_PER_STRING;
      StringSetBulbColor( white_bulbs[i].string_num, white_bulbs[i].bulb_num, RGB_BULB_COLOR_WHITE );
    }

    j = 0;
  }
  else
  if( tick_cnt % 4 == 0 )
  {
    StringSetBulbColor( white_bulbs[j].string_num, white_bulbs[j].bulb_num, RGB_BULB_COLOR_GREEN );
    white_bulbs[j].string_num = rand() % 4;
    white_bulbs[j].bulb_num = rand() % BULBS_PER_STRING;
    j++;
    if( j == 25 )
      j = 0;

    for( i = 0; i < 25; i++ )
    {
      StringSetBulbColor( white_bulbs[i].string_num, white_bulbs[i].bulb_num, RGB_BULB_COLOR_WHITE );
    }
  }
}

void SequenceRandomRedGreen( uint8_t init )
{
  static uint8_t i,j;

  if( init )
  {
    tick_cnt_threshold = TICK_CNT_THRESHOLD_DEFAULT;
    i = 0;
    j = 0;
  }

  if( i++ % 4 == 0 )
  {
    for( j = 0; j < BULBS_PER_STRING; j++ )
    {
      StringPushBulbColor( TOP, 0, 
        ( rand() % 2 )  == 0 ? RGB_BULB_COLOR_GREEN : RGB_BULB_COLOR_RED ); 
      StringPushBulbColor( TOP, 1, 
        ( rand() % 2 )  == 0 ? RGB_BULB_COLOR_GREEN : RGB_BULB_COLOR_RED ); 
      StringPushBulbColor( TOP, 2, 
        ( rand() % 2 )  == 0 ? RGB_BULB_COLOR_GREEN : RGB_BULB_COLOR_RED ); 
      StringPushBulbColor( TOP, 3, 
        ( rand() % 2 )  == 0 ? RGB_BULB_COLOR_GREEN : RGB_BULB_COLOR_RED ); 
    }
  }
}

void SequenceRedGreenStripes( uint8_t init )
{
  static uint8_t i,j;

  if( init )
  {
    tick_cnt_threshold = TICK_CNT_THRESHOLD_DEFAULT;
    i = 0;
    j = 0;
  }

  // red / green stripes
  if( j == 0 )
  {
    StringPushBulbColorAll( TOP, RGB_BULB_COLOR_RED );

    if( i++ >= 5 )
    {
      j = 1;
      i = 0;
    }
  }
  else
  {
    StringPushBulbColorAll( TOP, RGB_BULB_COLOR_GREEN );
     
    if( i++ >= 5 )
    {
      j = 0;
      i = 0;
    }
  }
}

void SequenceWhiteTwinkle( uint8_t init )
{
  bulb_behavior_type behavior;
  static uint8_t i,j;

  if( init )
  {
    tick_cnt_threshold = TICK_CNT_THRESHOLD_DEFAULT;
    i = 0;
    j = 0;
  }

  if( i == 0 )
  {
    behavior.mode = TWINKLE;
    behavior.intensity = RGB_BULB_DEFAULT_INTENSITY;

    for( j = 0; j < BULBS_PER_STRING; j++ )
    {
      StringPushBulbBehaviorAll( TOP, &behavior );
      StringPushBulbColorAll( TOP, RGB_BULB_COLOR_WHITE );
    }
  }
}

void SequenceTrails( uint8_t init )
{
  static uint8_t i, i2;
  static uint8_t j, j2;
  bulb_behavior_type behavior;

  if( init )
  {
    tick_cnt_threshold = TICK_CNT_THRESHOLD_DEFAULT;
    i = 0;
    j = 0;
    j2 = 5;
    i2 = i;
  }

  if( j > RGB_BULB_DEFAULT_INTENSITY / 6 )
  {
    i++; // step
    i %= RGB_BULB_LIGHT_COUNT;
    j = 0;
    j2 = 5;
  }
  
  if( j2 < 0 )
  {
    j2 = 5;
    i2 = i;
    j++;
  }

  i2++;
  i2 %= 50;
 
  behavior.mode = ON;
  behavior.intensity = j + ( RGB_BULB_DEFAULT_INTENSITY / 6 ) * j2;
  StringPushBulbColorAll( TOP, RGB_BULB_COLOR_WHITE );
  StringPushBulbBehaviorAll( TOP, &behavior );

  j2--;
}

int main(int argc, char *argv[])
{
  int done;
  uint8_t sequence_num = 0;

#ifdef PC_SIM
  int width, height;
  Uint8  video_bpp;
  Uint32 videoflags;
  SDL_Event event;
  Uint32 then, now, frames;

  if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
    fprintf(stderr, "SDL_Init problem: %s", SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);

  videoflags = SDL_SWSURFACE | SDL_ANYFORMAT;
  width = SCREEN_X;
  height = SCREEN_Y;
  video_bpp = 0;

  /*Video mode activation*/
  screen = SDL_SetVideoMode(width, height, video_bpp, videoflags);
  if (!screen) {
    fprintf(stderr, "I can not activate video mode: %dx%d: %s\n",
            width, height, SDL_GetError());
    exit(2);
  }
#endif

{/*BEGIN*/
  Init();

#ifdef PC_SIM
  //DumpStrings();

  frames = 0;
  then = SDL_GetTicks();
#endif

  done = 0;

  while( !done ) 
  {
    uint8_t init = !tick_cnt;

    if( !( tick_cnt == 0 && StringGetFadeStatus() == FADING ) )
    {
      if( tick_cnt == 0 )
        StringAllOnColor( RGB_BULB_COLOR_BLACK );

      switch( sequence_num )
      {
        case 0:
          SequenceRandomHue( init );
          break;

        case 1:
          SequenceGreenWithWhite( init );
          break;

        case 2:
          SequenceRainbow( init );
          break;

        case 3:
          SequenceTrails( init );
          break;

        case 4:
          SequenceRedGreenStripes( init );
          break;

        case 5:
          SequenceWhiteTwinkle( init );
          break;

        case 6:
          SequenceRandomRedGreen( init );
          break;
  
        case 7:
          SequenceRainbowRotate( init );
          break;

        case 8:
          SequenceFallingBlueWhite( init );
          break;
 
        case 9:
          SequenceRainbowSnake( init );
          break;
      }

      tick_cnt++;
 
      if( tick_cnt == tick_cnt_threshold )
      {
        tick_cnt = 0;
        sequence_num++;
        sequence_num %= 10;
        StringFadeAllOff( 20 );
      }
    }

    StringDraw();

#ifdef PC_SIM
    SDL_Delay(FRAME_PERIOD_MS);
    ++frames;
    while ( SDL_PollEvent(&event) ) {
      switch (event.type) {
        case SDL_KEYDOWN:
        /*break;*/
        case SDL_QUIT:
          done = 1;
        break;
        default:
        break;
      }
    }/*while*/
#else
  _delay_ms(FRAME_PERIOD_MS);
#endif
  }/*while(!done)*/

}/*END*/

#ifdef PC_SIM
  now = SDL_GetTicks();
  if ( now > then ) {
    printf("%2.2f frames per second\n",
          ((double)frames*1000)/(now-then));
  }

  fprintf(stderr, "[END]\n");
  return 0;
#else
  while(1);
#endif

}/*main*/

