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

#ifndef DEFINES_H
#define DEFINES_H

#define TRUE 1
#define FALSE 0
#define SCREEN_X 640
#define SCREEN_Y 480

//#define PC_SIM

#define TICK_CNT_THRESHOLD_DEFAULT 160
#define FRAME_PERIOD_MS 200
#define NUM_STRINGS 4
#define BULBS_PER_STRING 18 
#define BULB_RADIUS 5
#define SEPARATION_ANGLE ( 15.0 * M_PI / 180.0 )
#define ANGLE ( M_PI / 2 - ( ( NUM_STRINGS - 1 ) * SEPARATION_ANGLE / 2 ) )
#define STRING_LENGTH ( SCREEN_Y - BULB_RADIUS )
#define BULB_SPACING ( STRING_LENGTH / BULBS_PER_STRING )
#define BULB_POOL_SIZE ( NUM_STRINGS * BULBS_PER_STRING + 1 )

#endif
