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

/** @file
 *  @brief Responsible for accessing the object dictionary.
 *
 *  This file contains functions for accessing the object dictionary and
 *  variables that are contained by the object dictionary.
 *  Accessing the object dictionary contains setting local variables
 *  as PDOs and accessing (read/write) all entries of the object dictionary
 *  @warning Only the basic entries of an object dictionary are included
 *           at the moment.
 */

/** @defgroup od Object Dictionary Management
 *  @brief The Object Dictionary is the heart of each CANopen device containing all communication and application objects.
 *  @ingroup userapi
 */
 
#ifndef __objacces_h__
#define __objacces_h__

#include <applicfg.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef uint32_t (*valueRangeTest_t)(uint8_t typeValue, void *Value);
typedef void (* storeODSubIndex_t)(CO_Data* d, uint16_t wIndex, uint8_t bSubindex);
void _storeODSubIndex (CO_Data* d, uint16_t wIndex, uint8_t bSubindex);

/**
 * @brief Print MSG_WAR (s) if error to the access to the object dictionary occurs.
 * 
 * You must uncomment the lines in the file objaccess.c :\n
 * //#define DEBUG_CAN\n
 * //#define DEBUG_WAR_CONSOLE_ON\n
 * //#define DEBUG_ERR_CONSOLE_ON\n\n
 * Beware that sometimes, we force the sizeDataDict or sizeDataGiven to 0, when we wants to use
 * this function but we do not have the access to the right value. One example is
 * getSDOerror(). So do not take attention to these variables if they are null.
 * @param index
 * @param subIndex
 * @param sizeDataDict Size of the data defined in the dictionary
 * @param sizeDataGiven Size data given by the user.
 * @param code error code to print. (SDO abort code. See file def.h)
 * @return
 */ 
uint8_t accessDictionaryError(uint16_t index, uint8_t subIndex, 
			   uint32_t sizeDataDict, uint32_t sizeDataGiven, uint32_t code);


/* _getODentry() Reads an entry from the object dictionary.\n
 * 
 *    use getODentry() macro to read from object and endianize
 *    use readLocalDict() macro to read from object and not endianize   
 *
 * @code
 * // Example usage:
 * uint8_t  *pbData;
 * uint8_t length;
 * uint32_t returnValue;
 *
 * returnValue = getODentry( (uint16_t)0x100B, (uint8_t)1, 
 * (void * *)&pbData, (uint8_t *)&length );
 * if( returnValue != SUCCESSFUL )
 * {
 *     // error handling
 * }
 * @endcode 
 * @param *d Pointer to a CAN object data structure
 * @param wIndex The index in the object dictionary where you want to read
 *               an entry
 * @param bSubindex The subindex of the Index. e.g. mostly subindex 0 is
 *                  used to tell you how many valid entries you can find
 *                  in this index. Look at the canopen standard for further
 *                  information
 * @param *pDestData Pointer to the pointer which points to the variable where
 *                   the value of this object dictionary entry should be copied
 * @param pExpectedSize This function writes the size of the copied value (in Byte)
 *                      into this variable.
 * @param *pDataType Pointer to the type of the data. See objdictdef.h
 * @param CheckAccess if other than 0, do not read if the data is Write Only
 *                    [Not used today. Put always 0].
 * @param Endianize  When not 0, data is endianized into network byte order
 *                   when 0, data is not endianized and copied in machine native
 *                   endianness 
 * @return 
 * - OD_SUCCESSFUL is returned upon success. 
 * - SDO abort code is returned if error occurs . (See file def.h)
 */
uint32_t _getODentry( CO_Data* d,
                   uint16_t wIndex,
                   uint8_t bSubindex,
                   void * pDestData,
                   uint32_t * pExpectedSize,
                   uint8_t * pDataType,
                   uint8_t checkAccess,
                   uint8_t endianize);

/** 
 * @ingroup od
 * @brief getODentry() to read from object and endianize
 * @param OD Pointer to a CAN object data structure
 * @param wIndex The index in the object dictionary where you want to read
 *                an entry
 * @param bSubindex The subindex of the Index. e.g. mostly subindex 0 is
 *                  used to tell you how many valid entries you can find
 *                  in this index. Look at the canopen standard for further
 *                  information
 * @param *pDestData Pointer to the pointer which points to the variable where
 *                   the value of this object dictionary entry should be copied
 * @param pExpectedSize This function writes the size of the copied value (in Byte)
 *                      into this variable.
 * @param *pDataType Pointer to the type of the data. See objdictdef.h
 * @param checkAccess Flag that indicate if a check rights must be perfomed (0 : no , other than 0 : yes)
 * @param endianize  Set to 1 : endianized into network byte order 
 * @return 
 * - OD_SUCCESSFUL is returned upon success. 
 * - SDO abort code is returned if error occurs . (See file def.h)
 */
#define getODentry( OD, wIndex, bSubindex, pDestData, pExpectedSize, \
		          pDataType,  checkAccess)                         \
       _getODentry( OD, wIndex, bSubindex, pDestData, pExpectedSize, \
		          pDataType,  checkAccess, 1)            

/** 
 * @ingroup od
 * @brief readLocalDict() reads an entry from the object dictionary, but in 
 * contrast to getODentry(), readLocalDict() doesn't endianize entry and reads
 * entry in machine native endianness. 
 * @param OD Pointer to a CAN object data structure
 * @param wIndex The index in the object dictionary where you want to read
 *                an entry
 * @param bSubindex The subindex of the Index. e.g. mostly subindex 0 is
 *                  used to tell you how many valid entries you can find
 *                  in this index. Look at the canopen standard for further
 *                  information
 * @param *pDestData Pointer to the pointer which points to the variable where
 *                   the value of this object dictionary entry should be copied
 * @param pExpectedSize This function writes the size of the copied value (in Byte)
 *                      into this variable.
 * @param *pDataType Pointer to the type of the data. See objdictdef.h
 * @param checkAccess if other than 0, do not read if the data is Write Only
 *                    [Not used today. Put always 0].
 * @param endianize Set to 0, data is not endianized and copied in machine native
 *                  endianness 
 * @return 
 * - OD_SUCCESSFUL is returned upon success. 
 * - SDO abort code is returned if error occurs . (See file def.h)
 */
#define readLocalDict( OD, wIndex, bSubindex, pDestData, pExpectedSize, \
		          pDataType,  checkAccess)                         \
       _getODentry( OD, wIndex, bSubindex, pDestData, pExpectedSize, \
		          pDataType,  checkAccess, 0)

/* By this function you can write an entry into the object dictionary
 * @param *d Pointer to a CAN object data structure
 * @param wIndex The index in the object dictionary where you want to write
 *               an entry
 * @param bSubindex The subindex of the Index. e.g. mostly subindex 0 is
 *                  used to tell you how many valid entries you can find
 *                  in this index. Look at the canopen standard for further
 *                  information
 * @param *pSourceData Pointer to the variable that holds the value that should
 *                     be copied into the object dictionary
 * @param *pExpectedSize The size of the value (in Byte).
 * @param checkAccess Flag that indicate if a check rights must be perfomed (0 : no , other than 0 : yes)
 * @param endianize When not 0, data is endianized into network byte order
 *                  when 0, data is not endianized and copied in machine native
 *                  endianness   
 * @return 
 * - OD_SUCCESSFUL is returned upon success. 
 * - SDO abort code is returned if error occurs . (See file def.h)
 */
uint32_t _setODentry( CO_Data* d,
                   uint16_t wIndex,
                   uint8_t bSubindex,
                   void * pSourceData,
                   uint32_t * pExpectedSize,
                   uint8_t checkAccess,
                   uint8_t endianize);

/**
 * @ingroup od
 * @brief setODentry converts SourceData from network byte order to machine native 
 * format, and writes that to OD.
 * @code
 * // Example usage:
 * uint8_t B;
 * B = 0xFF; // set transmission type
 *
 * retcode = setODentry( (uint16_t)0x1800, (uint8_t)2, &B, sizeof(uint8_t), 1 );
 * @endcode
 * @param d Pointer to a CAN object data structure
 * @param wIndex The index in the object dictionary where you want to write
 *               an entry
 * @param bSubindex The subindex of the Index. e.g. mostly subindex 0 is
 *                  used to tell you how many valid entries you can find
 *                  in this index. Look at the canopen standard for further
 *                  information
 * @param *pSourceData Pointer to the variable that holds the value that should
 *                     be copied into the object dictionary
 * @param *pExpectedSize The size of the value (in Byte).
 * @param checkAccess Flag that indicate if a check rights must be perfomed (0 : no , other than 0 : yes)
 * @param endianize Set to 1 : endianized into network byte order
 * @return 
 * - OD_SUCCESSFUL is returned upon success. 
 * - SDO abort code is returned if error occurs . (See file def.h)
 */
#define setODentry( d, wIndex, bSubindex, pSourceData, pExpectedSize, \
                  checkAccess) \
       _setODentry( d, wIndex, bSubindex, pSourceData, pExpectedSize, \
                  checkAccess, 1)

/** @fn uint32_t writeLocalDict(d, wIndex, bSubindex, pSourceData, pExpectedSize, checkAccess)
 * @ingroup od
 * @hideinitializer
 * @brief Writes machine native SourceData to OD.
 * @param d Pointer to a CAN object data structure
 * @param wIndex The index in the object dictionary where you want to write
 *               an entry
 * @param bSubindex The subindex of the Index. e.g. mostly subindex 0 is
 *                  used to tell you how many valid entries you can find
 *                  in this index. Look at the canopen standard for further
 *                  information
 * @param *pSourceData Pointer to the variable that holds the value that should
 *                     be copied into the object dictionary
 * @param *pExpectedSize The size of the value (in Byte).
 * @param checkAccess Flag that indicate if a check rights must be perfomed (0 : no , other than 0 : yes)
 * @param endianize Data is not endianized and copied in machine native endianness 
 * @return 
 * - OD_SUCCESSFUL is returned upon success. 
 * - SDO abort code is returned if error occurs . (See file def.h)
 * \n\n
 * @code
 * // Example usage:
 * uint8_t B;
 * B = 0xFF; // set transmission type
 *
 * retcode = writeLocalDict( (uint16_t)0x1800, (uint8_t)2, &B, sizeof(uint8_t), 1 );
 * @endcode
 */
#define writeLocalDict( d, wIndex, bSubindex, pSourceData, pExpectedSize, checkAccess) \
       _setODentry( d, wIndex, bSubindex, pSourceData, pExpectedSize, checkAccess, 0)



/**
 * @brief Scan the index of object dictionary. Used only by setODentry and getODentry.
 * @param *d Pointer to a CAN object data structure
 * @param wIndex
 * @param *errorCode :  OD_SUCCESSFUL if index foundor SDO abort code. (See file def.h)
 * @param **Callback
 * @return NULL if index not found. Else : return the table part of the object dictionary.
 */
 const indextable * scanIndexOD (CO_Data* d, uint16_t wIndex, uint32_t *errorCode, ODCallback_t **Callback);

uint32_t RegisterSetODentryCallBack(CO_Data* d, uint16_t wIndex, uint8_t bSubindex, ODCallback_t Callback);

#ifdef __cplusplus
}
#endif

#endif /* __objacces_h__ */
