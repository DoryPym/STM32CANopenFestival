/*
 * @Author: your name
 * @Date: 2020-08-14 09:25:41
 * @LastEditTime: 2020-08-16 18:08:09
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \STM32F1_CANopen_slave_canfestival\canfestival\inc\can.h
 */
/*
This file is part of CanFestival, a library implementing CanOpen Stack. 

Copyright (C): Edouard TISSERANT and Francis DUPIN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __can_h__
#define __can_h__

#include "applicfg.h"

/*
union SHORT_CAN {
  struct { uint8_t b0,b1; } b;
  uint32_t w;
};
*/
/*
typedef struct {
  uint32_t w;
} SHORT_CAN;
*/

/** 
 * @brief The CAN message structure 
 * @ingroup can
 */
typedef struct {
  uint16_t cob_id;	/**< message's ID */
  uint8_t  rtr;		/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
  uint8_t  len;		/**< message's length (0 to 8) */
  uint8_t  data[8]; /**< message's datas */
} Message;

#define Message_Initializer {0,0,0,{0,0,0,0,0,0,0,0}}

typedef uint8_t (*canSend_t)(Message *);

#endif /* __can_h__ */
