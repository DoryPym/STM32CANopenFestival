
/*
  This file is part of CanFestival, a library implementing CanOpen
  Stack.

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
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
  USA
*/
#include "pdo.h"
#include "objacces.h"
#include "canfestival.h"
#include "sysdep.h"


//extern uint32_t MY_NOID;//zxb add this

/*!
** @file   pdo.c
** @author Edouard TISSERANT and Francis DUPIN
** @date   Tue Jun  5 09:32:32 2007
**
** @brief
**
**
*/

/*!
**
**
** @param d
** @param TPDO_com TPDO communication parameters OD entry
** @param TPDO_map TPDO mapping parameters OD entry
**
** @return
**/

uint8_t buildPDO (CO_Data * d, uint8_t numPdo, Message * pdo)
{
  const indextable *TPDO_com = d->objdict + d->firstIndex->PDO_TRS + numPdo;
  const indextable *TPDO_map = d->objdict + d->firstIndex->PDO_TRS_MAP + numPdo;

  uint8_t prp_j = 0x00;
  uint32_t offset = 0x00000000;
  const uint8_t *pMappingCount = (uint8_t *) TPDO_map->pSubindex[0].pObject;

  pdo->cob_id = (uint16_t) LEndian16(*(uint32_t*)TPDO_com->pSubindex[1].pObject & 0x7FF);
  pdo->rtr = NOT_A_REQUEST;

  MSG_WAR (0x3009, "  PDO CobId is : ",
           *(uint32_t *) TPDO_com->pSubindex[1].pObject);
  MSG_WAR (0x300D, "  Number of objects mapped : ", *pMappingCount);

  do
    {
      uint8_t dataType;            /* Unused */
      uint8_t tmp[] = { 0, 0, 0, 0, 0, 0, 0, 0 };  /* temporary space to hold bits */

      /* pointer fo the var which holds the mapping parameter of an mapping entry  */
      uint32_t *pMappingParameter =
        (uint32_t *) TPDO_map->pSubindex[prp_j + 1].pObject;
      uint16_t index = (uint16_t) ((*pMappingParameter) >> 16);
      uint32_t Size = (uint32_t) (*pMappingParameter & (uint32_t) 0x000000FF);     /* Size in bits */

      /* get variable only if Size != 0 and Size is lower than remaining bits in the PDO */
      if (Size && ((offset + Size) <= 64))
        {
          uint32_t ByteSize = 1 + ((Size - 1) >> 3);        /*1->8 => 1 ; 9->16 => 2, ... */
          uint8_t subIndex =
            (uint8_t) (((*pMappingParameter) >> (uint8_t) 8) & (uint32_t) 0x000000FF);

          MSG_WAR (0x300F, "  got mapping parameter : ", *pMappingParameter);
          MSG_WAR (0x3050, "    at index : ", TPDO_map->index);
          MSG_WAR (0x3051, "    sub-index : ", prp_j + 1);

          if (getODentry (d, index, subIndex, tmp, &ByteSize, &dataType, 0) !=
              OD_SUCCESSFUL)
            {
              MSG_ERR (0x1013,
                       " Couldn't find mapped variable at index-subindex-size : ",
                       (uint16_t) (*pMappingParameter));
              return 0xFF;
            }
          /* copy bit per bit in little endian */
          CopyBits ((uint8_t) Size, ((uint8_t *) tmp), 0, 0,
                    (uint8_t *) & pdo->data[offset >> 3], (uint8_t)(offset % 8), 0);

          offset += Size;
        }
      prp_j++;
    }
  while (prp_j < *pMappingCount);

  pdo->len = (uint8_t)(1 + ((offset - 1) >> 3));

  MSG_WAR (0x3015, "  End scan mapped variable", 0);

  return 0;
}

/*!
**
**
** @param d
** @param cobId
**
** @return
**/
uint8_t
sendPDOrequest (CO_Data * d, uint16_t RPDOIndex)
{
  uint32_t *pwCobId;
  uint16_t offset = d->firstIndex->PDO_RCV;
  uint16_t lastIndex = d->lastIndex->PDO_RCV;

  if (!d->CurrentCommunicationState.csPDO)
    {
      return 0;
    }

  /* Sending the request only if the cobid have been found on the PDO
     receive */
  /* part dictionary */

  MSG_WAR (0x3930, "sendPDOrequest RPDO Index : ", RPDOIndex);

  if (offset && RPDOIndex >= 0x1400)
    {
      offset += RPDOIndex - 0x1400;
      if (offset <= lastIndex)
        {
          /* get the CobId */
          pwCobId = d->objdict[offset].pSubindex[1].pObject;

          MSG_WAR (0x3930, "sendPDOrequest cobId is : ", *pwCobId);
          {
            Message pdo;
            pdo.cob_id = (uint16_t)LEndian16(*pwCobId);
            pdo.rtr = REQUEST;
            pdo.len = 0;
            return canSend (d->canHandle, &pdo);
          }
        }
    }
  MSG_ERR (0x1931, "sendPDOrequest : RPDO Index not found : ", RPDOIndex);
  return 0xFF;
}


/*!
**
**
** @param d
** @param m
**
** @return
**/
uint8_t
proceedPDO (CO_Data * d, Message * m)
{
	
  uint8_t numPdo;
  uint8_t numMap;                  /* Number of the mapped varable */
  uint8_t *pMappingCount = NULL;   /* count of mapped objects... */
  /* pointer to the var which is mapped to a pdo... */
  /*  void *     pMappedAppObject = NULL;   */
  /* pointer fo the var which holds the mapping parameter of an
     mapping entry */
  uint32_t *pMappingParameter = NULL;
  uint8_t *pTransmissionType = NULL;       /* pointer to the transmission
                                           type */
  uint32_t *pwCobId = NULL;
  uint8_t Size;
  uint8_t offset;
  uint8_t status;
  uint32_t objDict;
  uint16_t offsetObjdict;
  uint16_t lastIndex;
  TIMEVAL EventTimerDuration = 0;

  status = state2;
	MSG_WAR(0x000,"proceedPDO proceedPDO proceedPDO is",0xaaa);

  MSG_WAR (0x3935, "proceedPDO, cobID : ", (LEndian16(m->cob_id) & 0x7ff));
  offset = 0x00;
  numPdo = 0;
  numMap = 0;
  MSG_WAR (0x3935, "(*m).rtr : ", (*m).rtr);
  if ((*m).rtr == NOT_A_REQUEST)
    {                           /* The PDO received is not a
                                   request. */

      offsetObjdict = d->firstIndex->PDO_RCV;
      lastIndex = d->lastIndex->PDO_RCV;
	  MSG_WAR(0x3935,"offsetObjdict",offsetObjdict);
	  MSG_WAR(0x3935,"lastIndex",lastIndex);

      /* study of all the PDO stored in the dictionary */
      if (offsetObjdict)
        while (offsetObjdict <= lastIndex)
          {

            switch (status)
              {

              case state2:
                /* get CobId of the dictionary correspondant to the received
                   PDO */
                pwCobId =
                  d->objdict[offsetObjdict].pSubindex[1].pObject;
				
				MSG_WAR(0x3935,"pwCobId pwCobId pwCobId is",*pwCobId);
				MSG_WAR(0x3935,"LEndian16(m->cob_id) is",LEndian16(m->cob_id));
                /* check the CobId coherance */
                /*pwCobId is the cobId read in the dictionary at the state 3
                 */
                if (*pwCobId == LEndian16(m->cob_id))
                  {
                    /* The cobId is recognized */
                    status = state4;
                    EventTimerDuration = *(uint16_t *)d->objdict[offsetObjdict].pSubindex[5].pObject;
                    MSG_WAR (0x3936, "cobId found at index ",
                             0x1400 + numPdo);
                    break;
                  }
                else
                  {
                    /* cobId received does not match with those write in the
                       dictionnary */
                    MSG_WAR(0x3935,"1111111111111111:",0x11111);
                    numPdo++;
                    offsetObjdict++;
                    status = state2;
                    break;
                  }

              case state4:     /* Get Mapped Objects Number */
                /* The cobId of the message received has been found in the
                   dictionnary. */
                offsetObjdict = d->firstIndex->PDO_RCV_MAP;
                lastIndex = d->lastIndex->PDO_RCV_MAP;
                pMappingCount =
                  (uint8_t *) (d->objdict + offsetObjdict +
                            numPdo)->pSubindex[0].pObject;
                numMap = 0;
                while (numMap < *pMappingCount)
                  {
                    uint8_t tmp[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
                    uint32_t ByteSize;
                    pMappingParameter =
                      (uint32_t *) (d->objdict + offsetObjdict +
                                 numPdo)->pSubindex[numMap + 1].pObject;
					MSG_WAR(0x3935,"*pMappingParameter is",*pMappingParameter);
                    if (pMappingParameter == NULL)
                      {
                        MSG_ERR (0x1937, "Couldn't get mapping parameter : ",
                                 numMap + 1);
                        return 0xFF;
                      }
                    /* Get the addresse of the mapped variable. */
                    /* detail of *pMappingParameter : */
                    /* The 16 hight bits contains the index, the medium 8 bits
                       contains the subindex, */
                    /* and the lower 8 bits contains the size of the mapped
                       variable. */

                    Size = (uint8_t) (*pMappingParameter & (uint32_t) 0x000000FF);
					MSG_WAR(0x3935,"Size is",Size);

                    /* set variable only if Size != 0 and 
                     * Size is lower than remaining bits in the PDO */
                    if (Size && ((offset + Size) <= (m->len << 3)))
                      {
                        /* copy bit per bit in little endian */
                        CopyBits (Size, (uint8_t *) & m->data[offset >> 3],
                                  offset % 8, 0, ((uint8_t *) tmp), 0, 0);
                        /*1->8 => 1 ; 9->16 =>2, ... */
                        ByteSize = (uint32_t)(1 + ((Size - 1) >> 3));
                        MSG_WAR(0x3935,"*pMappingParameter is",*pMappingParameter);
                        objDict =
                          setODentry (d, (uint16_t) ((*pMappingParameter) >> 16),
                                      (uint8_t) (((*pMappingParameter) >> 8) &
                                              0xFF), tmp, &ByteSize, 0);

                        if (objDict != OD_SUCCESSFUL)
                          {
							MSG_ERR (0xaaaaaa,"objDict var : ",objDict);  
                            MSG_ERR (0x1938,
                                     "error accessing to the mapped var : ",
                                     numMap + 1);
                            MSG_WAR (0x2939, "         Mapped at index : ",
                                     (*pMappingParameter) >> 16);
                            MSG_WAR (0x2940, "                subindex : ",
                                     ((*pMappingParameter) >> 8) & 0xFF);
                            return 0xFF;
                          }

                        MSG_WAR (0x3942,
                                 "Variable updated by PDO cobid : ",
                                 LEndian16(m->cob_id));
                        MSG_WAR (0x3943, "         Mapped at index : ",
                                 (*pMappingParameter) >> 16);
                        MSG_WAR (0x3944, "                subindex : ",
                                 ((*pMappingParameter) >> 8) & 0xFF);
                        offset += Size;
                      }
                    numMap++;
                  }             /* end loop while on mapped variables */
                if (EventTimerDuration && d->RxPDO_EventTimers)
                {
                    DelAlarm (d->RxPDO_EventTimers[numPdo]);
                    d->RxPDO_EventTimers[numPdo] = SetAlarm (d, numPdo, d->RxPDO_EventTimers_Handler,
                    MS_TO_TIMEVAL (EventTimerDuration), 0);
                }
                return 0;

              }                 /* end switch status */
          }                     /* end while */
    }                           /* end if Donnees */
  else if ((*m).rtr == REQUEST)
    {
      MSG_WAR (0x3946, "Receive a PDO request cobId : ", LEndian16(m->cob_id));
      status = state1;
      offsetObjdict = d->firstIndex->PDO_TRS;
      lastIndex = d->lastIndex->PDO_TRS;
      if (offsetObjdict)
        while (offsetObjdict <= lastIndex)
          {
            /* study of all PDO stored in the objects dictionary */
             MSG_WAR (0x3946, "offsetObjdict", offsetObjdict);
            switch (status)
              {

              case state1:     /* check the CobId */
                /* get CobId of the dictionary which match to the received PDO
                 */
                pwCobId =
                   (d->objdict +
                             offsetObjdict)->pSubindex[1].pObject;
				MSG_WAR(0x3935,"pwCobId pwCobId pwCobId is",*pwCobId);
				MSG_WAR(0x3935,"LEndian16(m->cob_id) is",LEndian16(m->cob_id));
                if (*pwCobId == LEndian16(m->cob_id))
                  {
                    status = state4;
                    break;
                  }
                else
                  {
                    numPdo++;
                    offsetObjdict++;
                  }
                status = state1;
                break;


              case state4:     /* check transmission type */
                pTransmissionType =
                  (uint8_t *) d->objdict[offsetObjdict].pSubindex[2].pObject;
                /* If PDO is to be sampled and send on RTR, do it */
                if ((*pTransmissionType == TRANS_RTR))
                  {
                    status = state5;
                    break;
                  }
                /* RTR_SYNC means data prepared at SYNC, transmitted on RTR */
                else if ((*pTransmissionType == TRANS_RTR_SYNC))
                  {
                    if (d->PDO_status[numPdo].
                        transmit_type_parameter & PDO_RTR_SYNC_READY)
                      {
                        /*Data ready, just send */
                        canSend (d->canHandle,
                                 &d->PDO_status[numPdo].last_message);
                        return 0;
                      }
                    else
                      {
                        /* if SYNC did never occur, transmit current data */
                        /* DS301 do not tell what to do in such a case... */
                        MSG_ERR (0x1947,
                                 "Not ready RTR_SYNC TPDO send current data : ",
                                 LEndian16(m->cob_id));
                        status = state5;
                      }
                    break;
                  }
                else if ((*pTransmissionType == TRANS_EVENT_PROFILE) ||
                         (*pTransmissionType == TRANS_EVENT_SPECIFIC))
                  {
                    /* Zap all timers and inhibit flag */
                    d->PDO_status[numPdo].event_timer =
                      DelAlarm (d->PDO_status[numPdo].event_timer);
                    d->PDO_status[numPdo].inhibit_timer =
                      DelAlarm (d->PDO_status[numPdo].inhibit_timer);
                    d->PDO_status[numPdo].transmit_type_parameter &=
                      ~PDO_INHIBITED;
                    /* Call  PDOEventTimerAlarm for this TPDO, 
                     * this will trigger emission et reset timers */
                    PDOEventTimerAlarm (d, numPdo);
                    return 0;
                  }
                else
                  {
                    /* The requested PDO is not to send on request. So, does
                       nothing. */
                    MSG_WAR (0x2947, "PDO is not to send on request : ",
                             LEndian16(m->cob_id));
                    return 0xFF;
                  }

              case state5:     /* build and send requested PDO */
                {
                  Message pdo;
                  if (buildPDO (d, numPdo, &pdo))
                    {
                      MSG_ERR (0x1948, " C Couldn't build TPDO number : ", numPdo);
                      return 0xFF;
                    }
                  canSend (d->canHandle, &pdo);
                  return 0;
                }
              }                 /* end switch status */
          }                     /* end while */
    }                           /* end if Requete */

  return 0;
}

/*!
**
**
** @param NbBits
** @param SrcByteIndex
** @param SrcBitIndex
** @param SrcBigEndian
** @param DestByteIndex
** @param DestBitIndex
** @param DestBigEndian
**/
void
CopyBits (uint8_t NbBits, uint8_t * SrcByteIndex, uint8_t SrcBitIndex,
          uint8_t SrcBigEndian, uint8_t * DestByteIndex, uint8_t DestBitIndex,
          uint8_t DestBigEndian)
{
  /* This loop copy as many bits that it can each time, crossing */
  /* successively bytes */
  // boundaries from LSB to MSB.
  while (NbBits > 0)
    {
      /* Bit missalignement between src and dest */
      INTEGER8 Vect = DestBitIndex - SrcBitIndex;

      /* We can now get src and align it to dest */
      uint8_t Aligned =
        Vect > 0 ? *SrcByteIndex << Vect : *SrcByteIndex >> -Vect;

      /* Compute the nb of bit we will be able to copy */
      uint8_t BoudaryLimit = (Vect > 0 ? 8 - DestBitIndex : 8 - SrcBitIndex);
      uint8_t BitsToCopy = BoudaryLimit > NbBits ? NbBits : BoudaryLimit;

      /* Create a mask that will serve in: */
      uint8_t Mask =
        ((0xff << (DestBitIndex + BitsToCopy)) |
         (0xff >> (8 - DestBitIndex)));

      /* - Filtering src */
      uint8_t Filtered = Aligned & ~Mask;

      /* - and erase bits where we write, preserve where we don't */
      *DestByteIndex &= Mask;

      /* Then write. */
      *DestByteIndex |= Filtered;

      /*Compute next time cursors for src */
      if ((SrcBitIndex += BitsToCopy) > 7)      /* cross boundary ? */
        {
          SrcBitIndex = 0;      /* First bit */
          SrcByteIndex += (SrcBigEndian ? -1 : 1);      /* Next byte */
        }


      /*Compute next time cursors for dest */
      if ((DestBitIndex += BitsToCopy) > 7)
        {
          DestBitIndex = 0;     /* First bit */
          DestByteIndex += (DestBigEndian ? -1 : 1);    /* Next byte */
        }

      /*And decrement counter. */
      NbBits -= BitsToCopy;
    }

}

static void sendPdo(CO_Data * d, uint32_t pdoNum, Message * pdo)
{
  /*store_as_last_message */
  d->PDO_status[pdoNum].last_message = *pdo;
  MSG_WAR (0x396D, "sendPDO cobId :", LEndian16(pdo->cob_id));
  MSG_WAR (0x396E, "     Nb octets  : ", pdo->len);

  canSend (d->canHandle, pdo);
}


/*!
**
**
** @param d
**
** @return
**/

uint8_t
sendPDOevent (CO_Data * d)
{
  /* Calls _sendPDOevent specifying it is not a sync event */
  return _sendPDOevent (d, 0);
}

uint8_t
sendOnePDOevent (CO_Data * d, uint8_t pdoNum)
{
  uint16_t offsetObjdict;
  Message pdo;
  if (!d->CurrentCommunicationState.csPDO ||
      (d->PDO_status[pdoNum].transmit_type_parameter & PDO_INHIBITED))
    {
      return 0;
    }

  offsetObjdict = (uint16_t) (d->firstIndex->PDO_TRS + pdoNum);

  MSG_WAR (0x3968, "  PDO is on EVENT. Trans type : ",
           *((uint8_t *) d->objdict[offsetObjdict].pSubindex[2].pObject));
  
  memset(&pdo, 0, sizeof(pdo));
  if (buildPDO (d, pdoNum, &pdo))
    {
      MSG_ERR (0x3907, " Couldn't build TPDO number : ",
               pdoNum);
      return 0;
    }

  /*Compare new and old PDO */
  if (d->PDO_status[pdoNum].last_message.cob_id == pdo.cob_id
      && d->PDO_status[pdoNum].last_message.len == pdo.len
      && memcmp(d->PDO_status[pdoNum].last_message.data,
					pdo.data, 8) == 0
    )
    {
      /* No changes -> go to next pdo */
      return 0;
    }
  else
    {

      TIMEVAL EventTimerDuration;
      TIMEVAL InhibitTimerDuration;

      MSG_WAR (0x306A, "Changes TPDO number : ", pdoNum);
      /* Changes detected -> transmit message */
      EventTimerDuration =
        *(uint16_t *) d->objdict[offsetObjdict].pSubindex[5].
        pObject;
      InhibitTimerDuration =
        *(uint16_t *) d->objdict[offsetObjdict].pSubindex[3].
        pObject;

      /* Start both event_timer and inhibit_timer */
      if (EventTimerDuration)
        {
          DelAlarm (d->PDO_status[pdoNum].event_timer);
          d->PDO_status[pdoNum].event_timer =
            SetAlarm (d, pdoNum, &PDOEventTimerAlarm,
                      MS_TO_TIMEVAL (EventTimerDuration), 0);
        }

      if (InhibitTimerDuration)
        {
          DelAlarm (d->PDO_status[pdoNum].inhibit_timer);
          d->PDO_status[pdoNum].inhibit_timer =
            SetAlarm (d, pdoNum, &PDOInhibitTimerAlarm,
                      US_TO_TIMEVAL (InhibitTimerDuration *
                                     100), 0);
          /* and inhibit TPDO */
          d->PDO_status[pdoNum].transmit_type_parameter |=
            PDO_INHIBITED;
        }

      sendPdo(d, pdoNum, &pdo);
    }
    return 1;
}

void
PDOEventTimerAlarm (CO_Data * d, uint32_t pdoNum)
{
  /* This is needed to avoid deletion of re-attribuated timer */
  d->PDO_status[pdoNum].event_timer = TIMER_NONE;
  /* force emission of PDO by artificially changing last emitted */
  d->PDO_status[pdoNum].last_message.cob_id = 0;
  sendOnePDOevent (d, (uint8_t) pdoNum);
}

void
PDOInhibitTimerAlarm (CO_Data * d, uint32_t pdoNum)
{
  /* This is needed to avoid deletion of re-attribuated timer */
  d->PDO_status[pdoNum].inhibit_timer = TIMER_NONE;
  /* Remove inhibit flag */
  d->PDO_status[pdoNum].transmit_type_parameter &= ~PDO_INHIBITED;
  sendOnePDOevent (d, (uint8_t) pdoNum);
}

void
_RxPDO_EventTimers_Handler(CO_Data *d, uint32_t pdoNum)
{
}

/*!
**
**
** @param d
** @param isSyncEvent
**
** @return
**/

uint8_t
_sendPDOevent (CO_Data * d, uint8_t isSyncEvent)
{
  uint8_t pdoNum = 0x00;           /* number of the actual processed pdo-nr. */
  uint8_t *pTransmissionType = NULL;
  uint8_t status = state3;
  uint16_t offsetObjdict = d->firstIndex->PDO_TRS;
  uint16_t offsetObjdictMap = d->firstIndex->PDO_TRS_MAP;
  uint16_t lastIndex = d->lastIndex->PDO_TRS;
  
	
	MSG_WAR (0x3960, "d->CurrentCommunicationState.csPDO is ",d->CurrentCommunicationState.csPDO);
  if (!d->CurrentCommunicationState.csPDO)
    {
      return 0;
    }

   MSG_WAR (0x3960, "offsetObjdict is ",offsetObjdict);
  /* study all PDO stored in the objects dictionary */
  if (offsetObjdict)
    {
      Message pdo;/* = Message_Initializer;*/
      memset(&pdo, 0, sizeof(pdo));
      while (offsetObjdict <= lastIndex)
        {
          switch (status)
            {
            case state3:
              if ( /* bSubCount always 5 with objdictedit -> check disabled */
                   /*d->objdict[offsetObjdict].bSubCount < 5 ||*/
                   /* check if TPDO is not valid */
                   *(uint32_t *) d->objdict[offsetObjdict].pSubindex[1].
                   pObject & 0x80000000)
                {
                  MSG_WAR (0x3960, "Not a valid PDO ", 0x1800 + pdoNum);
                  /*Go next TPDO */
                  status = state11;
                  break;
                }
              /* get the PDO transmission type */
              pTransmissionType =
                (uint8_t *) d->objdict[offsetObjdict].pSubindex[2].pObject;
              MSG_WAR (0x3962, "Reading PDO at index : ", 0x1800 + pdoNum);
			  printf ("%s(%d) pTransmissionType is %d \r\n", __FILE__, __LINE__,*pTransmissionType);

              /* check if transmission type is SYNCRONOUS */
              /* message transmited every n SYNC with n=TransmissionType */
              if (isSyncEvent &&
                  (*pTransmissionType >= TRANS_SYNC_MIN) &&
                  (*pTransmissionType <= TRANS_SYNC_MAX) &&
                  (++d->PDO_status[pdoNum].transmit_type_parameter ==
                   *pTransmissionType))
                {
                  /*Reset count of SYNC */
                  d->PDO_status[pdoNum].transmit_type_parameter = 0;
                  MSG_WAR (0x3964, "  PDO is on SYNCHRO. Trans type : ",
                           *pTransmissionType);
                  //memset(&pdo, 0, sizeof(pdo));
                  //{
                   // Message msg_init = Message_Initializer;
                  //  pdo = msg_init;
                 // }
                  if (buildPDO (d, pdoNum, &pdo))
                    {
                      MSG_ERR (0x1906, " Couldn't build TPDO number : ",
                               pdoNum);
                      status = state11;
                      break;
                    }
                  status = state5;
                  /* If transmission RTR, with data sampled on SYNC */
                }
              else if (isSyncEvent && (*pTransmissionType == TRANS_RTR_SYNC))
                {
                  if (buildPDO
                      (d, pdoNum, &d->PDO_status[pdoNum].last_message))
                    {
                      MSG_ERR (0x1966, " Couldn't build TPDO number : ",
                               pdoNum);
                      d->PDO_status[pdoNum].transmit_type_parameter &=
                        ~PDO_RTR_SYNC_READY;
                    }
                  else
                    {
                      d->PDO_status[pdoNum].transmit_type_parameter |=
                        PDO_RTR_SYNC_READY;
                    }
                  status = state11;
                  break;
                  /* If transmission on Event and not inhibited, check for changes */
                }
              else
                if ( (isSyncEvent && (*pTransmissionType == TRANS_SYNC_ACYCLIC))
                     ||
                     (!isSyncEvent && (*pTransmissionType == TRANS_EVENT_PROFILE || *pTransmissionType == TRANS_EVENT_SPECIFIC)
                       && !(d->PDO_status[pdoNum].transmit_type_parameter & PDO_INHIBITED)))
                {
                
                  MSG_ERR (0x1966, " sendOnePDOevent pdoNum is: ",pdoNum);
                  sendOnePDOevent(d, pdoNum);
                  status = state11;
                }
              else
                {
                  MSG_WAR (0x306C,
                           "  PDO is not on EVENT or synchro or not at this SYNC. Trans type : ",
                           *pTransmissionType);
				  printf ("%s(%d) pTransmissionType is %d \r\n", __FILE__, __LINE__,*pTransmissionType);
                  status = state11;
                }
              break;
            case state5:       /*Send the pdo */
              sendPdo(d, pdoNum, &pdo);
              status = state11;
              break;
            case state11:      /*Go to next TPDO */
              pdoNum++;
              offsetObjdict++;
              offsetObjdictMap++;
              MSG_WAR (0x3970, "next pdo index : ", pdoNum);
              status = state3;
              break;

            default:
              MSG_ERR (0x1972, "Unknown state has been reached : %d", status);
              return 0xFF;
            }                   /* end switch case */

        }                       /* end while */
    }
  return 0;
}

/*!
**
**
** @param d
** @param OD_entry
** @param bSubindex
** @return always 0
**/

uint32_t
TPDO_Communication_Parameter_Callback (CO_Data * d,
                                       const indextable * OD_entry,
                                       uint8_t bSubindex)
{
  /* If PDO are actives */
  if (d->CurrentCommunicationState.csPDO)
    switch (bSubindex)
      {
      case 2:                  /* Changed transmition type */
      case 3:                  /* Changed inhibit time */
      case 5:                  /* Changed event time */
        {
          const indextable *TPDO_com = d->objdict + d->firstIndex->PDO_TRS;
          uint8_t numPdo = (uint8_t) (OD_entry - TPDO_com);    /* number of the actual processed pdo-nr. */

          /* Zap all timers and inhibit flag */
          d->PDO_status[numPdo].event_timer =
            DelAlarm (d->PDO_status[numPdo].event_timer);
          d->PDO_status[numPdo].inhibit_timer =
            DelAlarm (d->PDO_status[numPdo].inhibit_timer);
          d->PDO_status[numPdo].transmit_type_parameter = 0;
          /* Call  PDOEventTimerAlarm for this TPDO, this will trigger emission et reset timers */
          PDOEventTimerAlarm (d, numPdo);
          return 0;
        }

      default:                 /* other subindex are ignored */
        break;
      }
  return 0;
}

void
PDOInit (CO_Data * d)
{
  /* For each TPDO mapping parameters */
  uint16_t pdoIndex = 0x1800;      /* OD index of TDPO */

  uint16_t offsetObjdict = d->firstIndex->PDO_TRS;
  uint16_t lastIndex = d->lastIndex->PDO_TRS;
  if (offsetObjdict)
    while (offsetObjdict <= lastIndex)
      {
        /* Assign callbacks to sensible TPDO mapping subindexes */
        uint32_t errorCode;
        ODCallback_t *CallbackList;
        /* Find callback list */
        scanIndexOD (d, pdoIndex, &errorCode, &CallbackList);
        if (errorCode == OD_SUCCESSFUL && CallbackList)
          {
            /*Assign callbacks to corresponding subindex */
            /* Transmission type */
            CallbackList[2] = &TPDO_Communication_Parameter_Callback;
            /* Inhibit time */
            CallbackList[3] = &TPDO_Communication_Parameter_Callback;
            /* Event timer */
            CallbackList[5] = &TPDO_Communication_Parameter_Callback;
          }
        pdoIndex++;
        offsetObjdict++;
      }

  /* Trigger a non-sync event */
  _sendPDOevent (d, 0);
}

void
PDOStop (CO_Data * d)
{
  /* For each TPDO mapping parameters */
  uint8_t pdoNum = 0x00;           /* number of the actual processed pdo-nr. */
  uint16_t offsetObjdict = d->firstIndex->PDO_TRS;
  uint16_t lastIndex = d->lastIndex->PDO_TRS;
  if (offsetObjdict)
    while (offsetObjdict <= lastIndex)
      {
        /* Delete TPDO timers */
        d->PDO_status[pdoNum].event_timer =
          DelAlarm (d->PDO_status[pdoNum].event_timer);
        d->PDO_status[pdoNum].inhibit_timer =
          DelAlarm (d->PDO_status[pdoNum].inhibit_timer);
        /* Reset transmit type parameter */
        d->PDO_status[pdoNum].transmit_type_parameter = 0;
        d->PDO_status[pdoNum].last_message.cob_id = 0;
        pdoNum++;
        offsetObjdict++;
      }
}
