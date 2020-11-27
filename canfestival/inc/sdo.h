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

/** @defgroup comobj Communication Objects
 *  @ingroup userapi
 */
 
/** @defgroup sdo Service Data Object (SDO)
 *  SDOs provide the access to entries in the CANopen Object Dictionary.
 * 	An SDO is made up of at least two CAN messages with different identifiers.
 * 	SDO s are always confirmed point-to-point communications services. 
 *  @ingroup comobj
 */
 
#ifndef __sdo_h__
#define __sdo_h__

struct struct_s_transfer;

#include "timer_canfestival.h"

/* Block mode : Data consumer receive step 
 * - set to RXSTEP_STARTED when client receive initiate upload response 
 * - set to RXSTEP_END when last segment of a block received 
 */
typedef enum {RXSTEP_INIT, RXSTEP_STARTED, RXSTEP_END } rxStep_t;

typedef void (*SDOCallback_t)(CO_Data* d, uint8_t nodeId);

/* The Transfer structure
Used to store the different segments of
 - a SDO received before writing in the dictionary
 - the reading of the dictionary to put on a SDO to transmit
WARNING : after a change in this structure check the macro s_transfer_Initializer in data.h
*/

struct struct_s_transfer {
  uint8_t           CliServNbr; /**< The index of the SDO client / server in our OD minus 0x1280 / 0x1200 */

  uint8_t           whoami;     /**< Takes the values SDO_CLIENT or SDO_SERVER */
  uint8_t           state;      /**< state of the transmission : Takes the values SDO_... */
  uint8_t           toggle;	
  uint32_t          abortCode;  /**< Sent or received */
  /**< index and subindex of the dictionary where to store */
  /**< (for a received SDO) or to read (for a transmit SDO) */
  uint16_t          index;
  uint8_t           subIndex;
  uint32_t          count;      /**< Number of data received or to be sent. */
  uint32_t          offset;     /**< stack pointer of data[]
                              * Used only to tranfer part of a line to or from a SDO.
                              * offset is always pointing on the next free cell of data[].
                              * WARNING s_transfer.data is subject to ENDIANISATION
                              * (with respect to CANOPEN_BIG_ENDIAN)
                              */
  uint8_t           data [SDO_MAX_LENGTH_TRANSFERT];
#ifdef SDO_DYNAMIC_BUFFER_ALLOCATION
  uint8_t           *dynamicData;
  uint32_t          dynamicDataSize;
#endif //SDO_DYNAMIC_BUFFER_ALLOCATION
                                    
  uint8_t           peerCRCsupport;    /**< True if peer supports CRC */
  uint8_t           blksize;           /**< Number of segments per block with 0 < blksize < 128 */
  uint8_t           ackseq;            /**< sequence number of last segment that was received successfully */
  uint32_t          objsize;           /**< Size in bytes of the object provided by data producer */
  uint8_t           lastblockoffset;   /**< Value of offset before last block */
  uint8_t           seqno;             /**< Last sequence number received OK or transmitted */   
  uint8_t           endfield;          /**< nbr of bytes in last segment of last block that do not contain data */
  rxStep_t       rxstep;            /**< data consumer receive step - set to true when last segment of a block received */
  uint8_t           tmpData[8];        /**< temporary segment storage */

  uint8_t           dataType;   /**< Defined in objdictdef.h Value is visible_string
                              * if it is a string, any other value if it is not a string,
                              * like 0. In fact, it is used only if client.
                              */
  TIMER_HANDLE   timer;      /**< Time counter to implement a timeout in milliseconds.
                              * It is automatically incremented whenever
                              * the line state is in SDO_DOWNLOAD_IN_PROGRESS or
                              * SDO_UPLOAD_IN_PROGRESS, and reseted to 0
                              * when the response SDO have been received.
                              */
  SDOCallback_t Callback;   /**< The user callback func to be called at SDO transaction end */
};
typedef struct struct_s_transfer s_transfer;


#include "data.h"

/** 
 * @brief Reset of a SDO exchange on timeout.
 * Send a SDO abort.
 * @param *d Pointer on a CAN object data structure
 * @param id
 */
void SDOTimeoutAlarm(CO_Data* d, uint32_t id);

/** 
 * @brief Reset all SDO buffers.
 * @param *d Pointer on a CAN object data structure
 */
void resetSDO (CO_Data* d);


/** 
 * @brief Copy the data received from the SDO line transfert to the object dictionary.
 * @param *d Pointer on a CAN object data structure
 * @param line SDO line
 * @return SDO error code if error. Else, returns 0.
 */
uint32_t SDOlineToObjdict (CO_Data* d, uint8_t line);

/** 
 * @brief Copy the data from the object dictionary to the SDO line for a network transfert.
 * @param *d Pointer on a CAN object data structure
 * @param line SDO line
 * @return SDO error code if error. Else, returns 0.
 */
uint32_t objdictToSDOline (CO_Data* d, uint8_t line);

/** 
 * @brief Copy data from an existant line in the argument "* data"
 * @param d Pointer on a CAN object data structure
 * @param line SDO line
 * @param nbBytes
 * @param *data Pointer on the data
 * @return 0xFF if error. Else, returns 0.
 */
uint8_t lineToSDO (CO_Data* d, uint8_t line, uint32_t nbBytes, uint8_t * data);

/** 
 * @brief Add data to an existant line
 * @param d Pointer on a CAN object data structure
 * @param line SDO line
 * @param nbBytes
 * @param *data Pointer on the data
 * @return 0xFF if error. Else, returns 0.
 */
uint8_t SDOtoLine (CO_Data* d, uint8_t line, uint32_t nbBytes, uint8_t * data);

/** 
 * @brief Called when an internal SDO abort occurs.
 * Release the line * Only if server *
 * If client, the line must be released manually in the core application.
 * The reason of that is to permit the program to read the transfers structure before its reset,
 * because many informations are stored on it : index, subindex, data received or trasmited, ...
 * In all cases, sends a SDO abort.
 * @param *d Pointer on a CAN object data structure
 * @param CliServNbr
 * @param whoami
 * @param index
 * @param subIndex
 * @param abortCode
 * @return 0
 */
uint8_t failedSDO (CO_Data* d, uint8_t CliServNbr, uint8_t whoami, uint16_t index, uint8_t subIndex, uint32_t abortCode);

/** 
 * @brief Reset an unused line.
 * @param *d Pointer on a CAN object data structure
 * @param line SDO line
 */
void resetSDOline (CO_Data* d, uint8_t line);

/** 
 * @brief Initialize some fields of the structure.
 * @param *d Pointer on a CAN object data structure
 * @param line
 * @param CliServNbr
 * @param index
 * @param subIndex
 * @param state
 * @return 0
 */
uint8_t initSDOline (CO_Data* d, uint8_t line, uint8_t CliServNbr, uint16_t index, uint8_t subIndex, uint8_t state);

/** 
 * @brief Search for an unused line in the transfers array
 * to store a new SDO.
 * ie a line which value of the field "state" is "SDO_RESET"
 * An unused line have the field "state" at the value SDO_RESET
 * @param *d Pointer on a CAN object data structure
 * @param whoami Create the line for a SDO_SERVER or SDO_CLIENT.
 * @param *line Pointer on a SDO line 
 * @return 0xFF if all the lines are on use. Else, return 0.
 */
uint8_t getSDOfreeLine (CO_Data* d, uint8_t whoami, uint8_t *line);

/** 
 * @brief Search for the line, in the transfers array, which contains the
 * beginning of the reception of a fragmented SDO
 * @param *d Pointer on a CAN object data structure
 * @param CliServNbr Client or Server object involved
 * @param whoami takes 2 values : look for a line opened as SDO_CLIENT or SDO_SERVER
 * @param *line Pointer on a SDO line 
 * @return 0xFF if error.  Else, return 0
 */
uint8_t getSDOlineOnUse (CO_Data* d, uint8_t CliServNbr, uint8_t whoami, uint8_t *line);

/** 
 * @brief Search for the line, in the transfers array, which contains the
 * beginning of the reception of a fragmented SDO
 *
 * Because getSDOlineOnUse() does not return any line in state \c SDO_ABORTED_INTERNAL,
 * this funtion is used to return them, too.
 *
 * @param *d Pointer on a CAN object data structure
 * @param CliServNbr Client or Server object involved
 * @param whoami takes 2 values : look for a line opened as SDO_CLIENT or SDO_SERVER
 * @param *line Pointer on a SDO line
 * @return 0xFF if error.  Else, return 0
 */
uint8_t getSDOlineToClose (CO_Data* d, uint8_t CliServNbr, uint8_t whoami, uint8_t *line);

/** 
 * @brief Close a transmission.
 * @param *d Pointer on a CAN object data structure
 * @param CliServNbr Client or Server object involved
 * @param whoami Line opened as SDO_CLIENT or SDO_SERVER
 */
uint8_t closeSDOtransfer (CO_Data* d, uint8_t CliServNbr, uint8_t whoami);

/** 
 * @brief Bytes in the line structure which must be transmited (or received)
 * @param *d Pointer on a CAN object data structure
 * @param line SDO line 
 * @param *nbBytes Pointer on nbBytes
 * @return 0.
 */
uint8_t getSDOlineRestBytes (CO_Data* d, uint8_t line, uint32_t * nbBytes);

/** 
 * @brief Store in the line structure the nb of bytes which must be transmited (or received)
 * @param *d Pointer on a CAN object data structure
 * @param line SDO line 
 * @param nbBytes
 * @return 0 if success, 0xFF if error.
 */
uint8_t setSDOlineRestBytes (CO_Data* d, uint8_t line, uint32_t nbBytes);

/**
 * @brief Transmit a SDO frame on the bus bus_id
 * @param *d Pointer on a CAN object data structure
 * @param whoami Takes 2 values : SDO_CLIENT or SDO_SERVER
 * @param CliServNbr Client or Server object involved
 * @param data Array of the 8 bytes to transmit
 * @return canSend(bus_id,&m) or 0xFF if error.
 */
uint8_t sendSDO (CO_Data* d, uint8_t whoami, uint8_t CliServNbr, uint8_t *pData);

/** 
 * @brief Transmit a SDO error to the client. The reasons may be :
 * Read/Write to a undefined object
 * Read/Write to a undefined subindex
 * Read/write a not valid length object
 * Write a read only object
 * @param *d Pointer on a CAN object data structure
 * @param whoami takes 2 values : SDO_CLIENT or SDO_SERVER
 * @param CliServNbr
 * @param index
 * @param subIndex
 * @param abortCode
 * @return 0
 */
uint8_t sendSDOabort (CO_Data* d, uint8_t whoami, uint8_t CliServNbr, uint16_t index, uint8_t subIndex, uint32_t abortCode);

/** 
 * @brief Treat a SDO frame reception
 * call the function sendSDO
 * @param *d Pointer on a CAN object data structure
 * @param *m Pointer on a CAN message structure 
 * @return code : 
 * 		   - 0xFF if error
 *         - 0x80 if transfert aborted by the server
 *         - 0x0  ok
 */
uint8_t proceedSDO (CO_Data* d, Message *m);

/** 
 * @ingroup sdo
 * @brief Used to send a SDO request frame to write the data at the index and subIndex indicated
 * @param *d Pointer to a CAN object data structure
 * @param nodeId Node Id of the slave
 * @param index At index indicated
 * @param subIndex At subIndex indicated
 * @param count number of bytes to write in the dictionnary.
 * @param dataType (defined in objdictdef.h) : put "visible_string" for strings, 0 for integers or reals or other value.
 * @param *data Pointer to data
 * @return 
 * - 0 is returned upon success.
 * - 0xFE is returned when no sdo client to communicate with node.
 * - 0xFF is returned when error occurs.
 */
uint8_t writeNetworkDict (CO_Data* d, uint8_t nodeId, uint16_t index,
		       uint8_t subIndex, uint32_t count, uint8_t dataType, void *data, uint8_t useBlockMode);

/** 
 * @ingroup sdo
 * @brief Used to send a SDO request frame to write in a distant node dictionnary.
 * @details The function Callback which must be defined in the user code is called at the
 * end of the exchange. (on succes or abort).
 * @param *d Pointer to a CAN object data structure
 * @param nodeId Node Id of the slave
 * @param index At index indicated
 * @param subIndex At subIndex indicated
 * @param count number of bytes to write in the dictionnary.
 * @param dataType (defined in objdictdef.h) : put "visible_string" for strings, 0 for integers or reals or other value.
 * @param *data Pointer to data
 * @param Callback Callback function
 * @return 
 * - 0 is returned upon success.
 * - 0xFE is returned when no sdo client to communicate with node.
 * - 0xFF is returned when error occurs.
 */
uint8_t writeNetworkDictCallBack (CO_Data* d, uint8_t nodeId, uint16_t index,
		       uint8_t subIndex, uint32_t count, uint8_t dataType, void *data, SDOCallback_t Callback, uint8_t useBlockMode);

/**
 * @ingroup sdo 
 * @brief Used to send a SDO request frame to write in a distant node dictionnary.
 * @details The function Callback which must be defined in the user code is called at the
 * end of the exchange. (on succes or abort). First free SDO client parameter is
 * automatically initialized for specific node if not already defined.
 * @param *d Pointer to a CAN object data structure
 * @param nodeId Node Id of the slave
 * @param index At index indicated
 * @param subIndex At subIndex indicated
 * @param count number of bytes to write in the dictionnary.
 * @param dataType (defined in objdictdef.h) : put "visible_string" for strings, 0 for integers or reals or other value.
 * @param *data Pointer to data
 * @param Callback Callback function
 * @param endianize When not 0, data is endianized into network byte order
 *                  when 0, data is not endianized and copied in machine native
 *                  endianness
 * @return 
 * - 0 is returned upon success.
 * - 0xFF is returned when error occurs.
 */
uint8_t writeNetworkDictCallBackAI (CO_Data* d, uint8_t nodeId, uint16_t index,
		       uint8_t subIndex, uint32_t count, uint8_t dataType, void *data, SDOCallback_t Callback, uint8_t endianize, uint8_t useBlockMode);

/**
 * @ingroup sdo 
 * @brief Used to send a SDO request frame to read.
 * @param *d Pointer to a CAN object data structure
 * @param nodeId Node Id of the slave
 * @param index At index indicated
 * @param subIndex At subIndex indicated
 * @param dataType (defined in objdictdef.h) : put "visible_string" for strings, 0 for integers or reals or other value.
 * @return 
 * - 0 is returned upon success.
 * - 0xFE is returned when no sdo client to communicate with node.
 * - 0xFF is returned when error occurs.
 */
uint8_t readNetworkDict (CO_Data* d, uint8_t nodeId, uint16_t index, uint8_t subIndex, uint8_t dataType, uint8_t useBlockMode);

/** 
 * @ingroup sdo
 * @brief Used to send a SDO request frame to read in a distant node dictionnary.
 * @details The function Callback which must be defined in the user code is called at the
 * end of the exchange. (on succes or abort).
 * @param *d Pointer on a CAN object data structure
 * @param nodeId Node Id of the slave
 * @param index At index indicated
 * @param subIndex At subIndex indicated
 * @param dataType (defined in objdictdef.h) : put "visible_string" for strings, 0 for integers or reals or other value.
 * @param Callback Callback function
 * @return 
 * - 0 is returned upon success.
 * - 0xFE is returned when no sdo client to communicate with node.
 * - 0xFF is returned when error occurs.
 */
uint8_t readNetworkDictCallback (CO_Data* d, uint8_t nodeId, uint16_t index, uint8_t subIndex, uint8_t dataType, SDOCallback_t Callback, uint8_t useBlockMode);

/** 
 * @ingroup sdo
 * @brief Used to send a SDO request frame to read in a distant node dictionnary.
 * @details The function Callback which must be defined in the user code is called at the
 * end of the exchange. (on succes or abort). First free SDO client parameter is
 * automatically initialized for specific node if not already defined.
 * @param *d Pointer on a CAN object data structure
 * @param nodeId Node Id of the slave
 * @param index At index indicated
 * @param subIndex At subIndex indicated
 * @param dataType (defined in objdictdef.h) : put "visible_string" for strings, 0 for integers or reals or other value.
 * @param Callback Callback function
 * @return 
 * - 0 is returned upon success.
 * - 0xFF is returned when error occurs.
 */
uint8_t readNetworkDictCallbackAI (CO_Data* d, uint8_t nodeId, uint16_t index, uint8_t subIndex, uint8_t dataType, SDOCallback_t Callback, uint8_t useBlockMode);

/** 
 * @ingroup sdo
 * @brief Use this function after calling readNetworkDict to get the result.
 * 
 * @param *d Pointer to a CAN object data structure
 * @param nodeId Node Id of the slave
 * @param *data Pointer to the buffer to get the data
 * @param *size Pointer to the size : MUST contain the size of the buffer before calling
 *                                    The function set it to the actual number of written bytes
 * @param *abortCode Pointer to the abortcode. (0 = not available. Else : SDO abort code. (received if return SDO_ABORTED_RCV)
 * 
 * 
 * @return
 *           - SDO_FINISHED             // datas are available
 *           - SDO_ABORTED_RCV          // Transfert failed (abort SDO received)
 *           - SDO_ABORTED_INTERNAL     // Transfert failed (internal abort)
 *           - SDO_UPLOAD_IN_PROGRESS   // Datas are not yet available
 *           - SDO_DOWNLOAD_IN_PROGRESS // Download is in progress
 *           - SDO_PROVIDED_BUFFER_TOO_SMALL //The value *size is not enough to store the received data
 * \n\n
 * example :
 * @code
 * uint32_t data;
 * uint8_t size;
 * readNetworkDict(0, 0x05, 0x1016, 1, 0) // get the data index 1016 subindex 1 of node 5
 * while (getReadResultNetworkDict (0, 0x05, &data, &size) == SDO_UPLOAD_IN_PROGRESS);
 * @endcode
*/
uint8_t getReadResultNetworkDict (CO_Data* d, uint8_t nodeId, void* data, uint32_t *size, uint32_t * abortCode);

/**
 * @ingroup sdo
 * @brief Use this function after calling writeNetworkDict function to get the result of the write.
 * @details It is mandatory to call this function because it is releasing the line used for the transfer.
 * @param *d Pointer to a CAN object data structure
 * @param nodeId Node Id of the slave
 * @param *abortCode Pointer to the abortcode
 * - 0 = not available. 
 * - SDO abort code (received if return SDO_ABORTED_RCV)
 * 
 * @return : 
 *           - SDO_FINISHED             // datas are available
 *           - SDO_ABORTED_RCV          // Transfert failed (abort SDO received)
 *           - SDO_ABORTED_INTERNAL     // Transfert failed (Internal abort)
 *           - SDO_DOWNLOAD_IN_PROGRESS // Datas are not yet available
 *           - SDO_UPLOAD_IN_PROGRESS   // Upload in progress
 * \n\n
 * example :
 * @code
 * uint32_t data = 0x50;
 * uint8_t size;
 * uint32_t abortCode;
 * writeNetworkDict(0, 0x05, 0x1016, 1, size, &data) // write the data index 1016 subindex 1 of node 5
 * while (getWriteResultNetworkDict (0, 0x05, &abortCode) == SDO_DOWNLOAD_IN_PROGRESS);
 * @endcode
*/
uint8_t getWriteResultNetworkDict (CO_Data* d, uint8_t nodeId, uint32_t * abortCode);

#endif
