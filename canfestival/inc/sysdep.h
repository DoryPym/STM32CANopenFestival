/*
 * @Author: your name
 * @Date: 2020-08-14 09:25:41
 * @LastEditTime: 2020-08-16 19:12:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \STM32F1_CANopen_slave_canfestival\canfestival\inc\sysdep.h
 */
#ifndef __sysdep_h__
#define __sysdep_h__

#include "config.h"

#ifdef CANOPEN_BIG_ENDIAN

/* Warning: the argument must not update pointers, e.g. *p++ */

#define LEndian16(v)  ((((uint16_t)(v) & 0xff00) >> 8) | \
		      (((uint16_t)(v) & 0x00ff) << 8))

#define LEndian32(v)  ((((uint32_t)(v) & 0xff000000) >> 24) |	\
		      (((uint32_t)(v) & 0x00ff0000) >> 8)  |	\
		      (((uint32_t)(v) & 0x0000ff00) << 8)  |	\
		      (((uint32_t)(v) & 0x000000ff) << 24))

#else

#define LEndian16(v)  (v)

#define LEndian32(v)  (v)

#endif

#endif /* __sysdep_h__ */

