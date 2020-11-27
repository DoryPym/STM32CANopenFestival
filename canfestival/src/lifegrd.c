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

/*!
** @file   lifegrd.c
** @author Edouard TISSERANT
** @date   Mon Jun  4 17:19:24 2007
**
** @brief
**
**
*/

#include <data.h>
#include "lifegrd.h"
#include "canfestival.h"
#include "dcf.h"
#include "sysdep.h"


void ConsumerHearbeatAlarm(CO_Data* d, uint32_t id);


void ProducerHearbeatAlarm(CO_Data* d, uint32_t id);

uint32_t OnHearbeatProducerUpdate(CO_Data* d, const indextable * unsused_indextable, uint8_t unsused_bSubindex);

/*!
**
**
** @param d
** @param nodeId
**
** @return
**/
e_nodeState getNodeState (CO_Data* d, uint8_t nodeId)
{
  e_nodeState networkNodeState = Unknown_state;
  #if NMT_MAX_NODE_ID>0
  if(nodeId < NMT_MAX_NODE_ID)
    networkNodeState = d->NMTable[nodeId];
  #endif
  return networkNodeState;
}

/*! 
** The Consumer Timer Callback
**
** @param d
** @param id
**/
void ConsumerHearbeatAlarm(CO_Data* d, uint32_t id)
{
  uint8_t nodeId = (uint8_t)(((d->ConsumerHeartbeatEntries[id]) & (uint32_t)0x00FF0000) >> (uint8_t)16);
  /*MSG_WAR(0x00, "ConsumerHearbeatAlarm", 0x00);*/

  /* timer have been notified and is now free (non periodic)*/
  /* -> avoid deleting re-assigned timer if message is received too late*/
  d->ConsumerHeartBeatTimers[id]=TIMER_NONE;
  
  /* set node state */
  d->NMTable[nodeId] = Disconnected;
  /*! call heartbeat error with NodeId */
  (*d->heartbeatError)(d, nodeId);
}

/*!
**
**
** @param d
** @param m
**/
void proceedNODE_GUARD(CO_Data* d, Message* m )
{
  uint8_t nodeId = (uint8_t) GET_NODE_ID((*m));

  if((m->rtr == 1) )
    /*!
    ** Notice that only the master can have sent this
    ** node guarding request
    */
    {
			
      /*!
      ** Receiving a NMT NodeGuarding (request of the state by the
      ** master)
      ** Only answer to the NMT NodeGuarding request, the master is
      ** not checked (not implemented)
      */
			MSG_WAR(0x000,"nodeId is ",nodeId);
			MSG_WAR(0x000,"*d->bDeviceNodeId is ",*d->bDeviceNodeId);
      if (nodeId == *d->bDeviceNodeId )
        {	
          Message msg;
          uint16_t tmp = *d->bDeviceNodeId + 0x700;
          msg.cob_id = LEndian16(tmp);
          msg.len = (uint8_t)0x01;
          msg.rtr = 0;
          msg.data[0] = d->nodeState;
			
					printf("2222222222222\r\n");
          if (d->toggle)
            {
              msg.data[0] |= 0x80 ;
              d->toggle = 0 ;
            }
          else
            d->toggle = 1 ;
          /* send the nodeguard response. */
          MSG_WAR(0x3130, "Sending NMT Nodeguard to master, state: ", d->nodeState);
          canSend(d->canHandle,&msg );
       }
    }else{ /* Not a request CAN */
      /* The state is stored on 7 bit */
			
      e_nodeState newNodeState = (e_nodeState) ((*m).data[0] & 0x7F);
       printf("33333333333333333\r\n");
      MSG_WAR(0x3110, "Received NMT nodeId : ", nodeId);
      
      if (d->NMTable[nodeId] != newNodeState)
      {
				printf("4444444444444444444\r\n");
        (*d->post_SlaveStateChange)(d, nodeId, newNodeState);
        /* the slave's state receievd is stored in the NMTable */
        d->NMTable[nodeId] = newNodeState;
      }

      /* Boot-Up frame reception */
      if ( d->NMTable[nodeId] == Initialisation)
      {
				printf("555555555555555555\r\n");
          /*
          ** The device send the boot-up message (Initialisation)
          ** to indicate the master that it is entered in
          ** pre_operational mode
          */
          MSG_WAR(0x3100, "The NMT is a bootup from node : ", nodeId);
          /* call post SlaveBootup with NodeId */
		  (*d->post_SlaveBootup)(d, nodeId);
      }

      if( d->NMTable[nodeId] != Unknown_state ) {
        uint8_t index, ConsummerHeartBeat_nodeId ;
				printf("6666666666666666666666666666\r\n");
        for( index = (uint8_t)0x00; index < *d->ConsumerHeartbeatCount; index++ )
          {
            ConsummerHeartBeat_nodeId = (uint8_t)( ((d->ConsumerHeartbeatEntries[index]) & (uint32_t)0x00FF0000) >> (uint8_t)16 );
            if ( nodeId == ConsummerHeartBeat_nodeId )
              {
                TIMEVAL time = ( (d->ConsumerHeartbeatEntries[index]) & (uint32_t)0x0000FFFF ) ;
                /* Renew alarm for next heartbeat. */
                DelAlarm(d->ConsumerHeartBeatTimers[index]);
                d->ConsumerHeartBeatTimers[index] = SetAlarm(d, index, &ConsumerHearbeatAlarm, MS_TO_TIMEVAL(time), 0);
              }
          }
      }
    }
}

/*! The Consumer Timer Callback
**
**
** @param d
** @param id
**/
void ProducerHearbeatAlarm(CO_Data* d, uint32_t id)
{
  if(*d->ProducerHeartBeatTime)
    {
      Message msg;
      /* Time expired, the heartbeat must be sent immediately
      ** generate the correct node-id: this is done by the offset 1792
      ** (decimal) and additionaly
      ** the node-id of this device.
      */
      uint16_t tmp = *d->bDeviceNodeId + 0x700;
      msg.cob_id = LEndian16(tmp);
      msg.len = (uint8_t)0x01;
      msg.rtr = 0;
      msg.data[0] = d->nodeState; /* No toggle for heartbeat !*/
      /* send the heartbeat */
      MSG_WAR(0x3130, "Producing heartbeat: ", d->nodeState);
      canSend(d->canHandle,&msg );

    }else{
      d->ProducerHeartBeatTimer = DelAlarm(d->ProducerHeartBeatTimer);
    }
}

/*! This is called when Index 0x1017 is updated.
**
**
** @param d
** @param unsused_indextable
** @param unsused_bSubindex
**
** @return
**/
uint32_t OnHeartbeatProducerUpdate(CO_Data* d, const indextable * unsused_indextable, uint8_t unsused_bSubindex)
{
  heartbeatStop(d);
  heartbeatInit(d);
  return 0;
}

/*!
**
**
** @param d
**/
void heartbeatInit(CO_Data* d)
{
  
  uint8_t index; /* Index to scan the table of heartbeat consumers */
  MSG_WAR(heartbeatInit, "has go into heartbeatInit...........\r\n",0);
  RegisterSetODentryCallBack(d, 0x1017, 0x00, &OnHeartbeatProducerUpdate);

  d->toggle = 0;

  for( index = (uint8_t)0x00; index < *d->ConsumerHeartbeatCount; index++ )
    {
      TIMEVAL time = (uint16_t) ( (d->ConsumerHeartbeatEntries[index]) & (uint32_t)0x0000FFFF ) ;
      /* MSG_WAR(0x3121, "should_time : ", should_time ) ; */
      if ( time )
        {
          d->ConsumerHeartBeatTimers[index] = SetAlarm(d, index, &ConsumerHearbeatAlarm, MS_TO_TIMEVAL(time), 0);
        }
    }

  if ( *d->ProducerHeartBeatTime )
    {
      TIMEVAL time = *d->ProducerHeartBeatTime;
      d->ProducerHeartBeatTimer = SetAlarm(d, 0, &ProducerHearbeatAlarm, MS_TO_TIMEVAL(time), MS_TO_TIMEVAL(time));
    }
}

/*!
**
**
** @param d
**/
void heartbeatStop(CO_Data* d)
{
  uint8_t index;
  for( index = (uint8_t)0x00; index < *d->ConsumerHeartbeatCount; index++ )
    {
      d->ConsumerHeartBeatTimers[index] = DelAlarm(d->ConsumerHeartBeatTimers[index]);
    }

  d->ProducerHeartBeatTimer = DelAlarm(d->ProducerHeartBeatTimer);
}

/*!
**
**
** @param heartbeatID
**/
void _heartbeatError(CO_Data* d, uint8_t heartbeatID){}
void _post_SlaveBootup(CO_Data* d, uint8_t SlaveID){}
void _post_SlaveStateChange(CO_Data* d, uint8_t nodeId, e_nodeState newNodeState){}

