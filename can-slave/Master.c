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
#include <stm32f10x.h>
#include <stdbool.h>
#include "Master.h"
#include "Slave.h"
#include "TestMasterSlave.h"

extern s_BOARD MasterBoard;
/*****************************************************************************/
void TestMaster_heartbeatError(CO_Data* d, uint8_t heartbeatID)
{
	eprintf("TestMaster_heartbeatError %d\n", heartbeatID);
}

/********************************************************
 * ConfigureSlaveNode is responsible to
 *  - setup master RPDO 1 to receive TPDO 1 from id 2
 *  - setup master RPDO 2 to receive TPDO 2 from id 2
 ********************************************************/
void TestMaster_initialisation(CO_Data* d)
{
	uint32_t PDO1_COBID = 0x0182; 
	uint32_t PDO2_COBID = 0x0282;
	uint32_t size = sizeof(uint32_t); 
	uint32_t SINC_cicle=0;
	uint8_t data_type = 0;
	
	eprintf("TestMaster_initialisation\n");

	/*****************************************
	 * Define RPDOs to match slave ID=2 TPDOs*
	 *****************************************/
	writeLocalDict( &TestMaster_Data, /*CO_Data* d*/
			0x1400, /*uint16_t index*/
			0x01, /*uint8_t subind*/ 
			&PDO1_COBID, /*void * pSourceData,*/ 
			&size, /* uint8_t * pExpectedSize*/
			RW);  /* uint8_t checkAccess */
			
	writeLocalDict( &TestMaster_Data, /*CO_Data* d*/
			0x1401, /*uint16_t index*/
			0x01, /*uint8_t subind*/ 
			&PDO2_COBID, /*void * pSourceData,*/ 
			&size, /* uint8_t * pExpectedSize*/
			RW);  /* uint8_t checkAccess */
					
}

// Step counts number of times ConfigureSlaveNode is called
static int init_step = 0;

/*Froward declaration*/
static void ConfigureSlaveNode(CO_Data* d, uint8_t nodeId);

/**/
static void CheckSDOAndContinue(CO_Data* d, uint8_t nodeId)
{
	uint32_t abortCode;	
	if(getWriteResultNetworkDict (d, nodeId, &abortCode) != SDO_FINISHED)
		eprintf("Master : Failed in initializing slave %2.2x, step %d, AbortCode :%4.4x \n", nodeId, init_step, abortCode);

	/* 完成与此节点的最后一次SDO传输 */
	closeSDOtransfer(&TestMaster_Data, nodeId, SDO_CLIENT);

	ConfigureSlaveNode(d, nodeId);
}

/********************************************************
 * ConfigureSlaveNode is responsible to
 *  - setup slave TPDO 1 transmit type
 *  - setup slave TPDO 2 transmit type
 *  - switch to operational mode
 *  - send NMT to slave
 ********************************************************
 * This an example of :
 * Network Dictionary Access (SDO) with Callback 
 * Slave node state change request (NMT) 
 ********************************************************
 * This is called first by TestMaster_post_SlaveBootup
 * then it called again each time a SDO exchange is
 * finished.
 ********************************************************/

//extern uint32_t usebuf;

static void ConfigureSlaveNode(CO_Data* d, uint8_t nodeId)
{
	/* 主站通过DCF精简配置从站节点ID 0x02的心跳生成器时间为1000 ms */
	 
	uint8_t Transmission_Type = 0x01;
	uint8_t res;
	//eprintf("Master : ConfigureSlaveNode %2.2x\n", nodeId);

	switch(++init_step){
		case 1: /* 第一步：设置要在SYNC上传输的从站的TPDO 1 */
			//eprintf("Master : set slave %2.2x TPDO 1 transmit type\n", nodeId);
			res = writeNetworkDictCallBack (d, /*CO_Data* d*/
					nodeId, /*uint8_t nodeId*/
					0x1800, /*uint16_t index*/
					0x02, /*uint8_t subindex*/
					1, /*uint8_t count*/
					0, /*uint8_t dataType*/
					&Transmission_Type,/*void *data*/
					CheckSDOAndContinue, /*SDOCallback_t Callback*/
                    0); /* use block mode */
					break;
		
		case 2:	/*Second step*/
			//eprintf("Master : set slave %2.2x TPDO 2 transmit type\n", nodeId);
			writeNetworkDictCallBack (d, /*CO_Data* d*/
					nodeId, /*uint8_t nodeId*/
					0x1801, /*uint16_t index*/
					0x02, /*uint16_t index*/
					1, /*uint8_t count*/
					0, /*uint8_t dataType*/
					&Transmission_Type,/*void *data*/
					CheckSDOAndContinue, /*SDOCallback_t Callback*/
                    0); /* use block mode */
					break;
		case 3: 
		
		/****************************** START *******************************/
		
			/* Put the master in operational mode */
			setState(d, Operational);
		 
			/* 要求从节点进入操作模式 */
			masterSendNMTstateChange (d, nodeId, NMT_Start_Node);

//			usebuf = 1;
	}
}


void TestMaster_preOperational(CO_Data* d)
{
	eprintf("TestMaster_preOperational\n");
}

void TestMaster_operational(CO_Data* d)
{
	eprintf("TestMaster_operational\n");
}

void TestMaster_stopped(CO_Data* d)
{
	eprintf("TestMaster_stopped\n");
}

void TestMaster_post_sync(CO_Data* d)
{
	eprintf("TestMaster_post_sync\n");
	eprintf("Master: %d %d %d %d %d %d %d %d %d %x %x %d %d\n",
		MasterMap1,
		MasterMap2,
		MasterMap3, 
		MasterMap4,
		MasterMap5,
		MasterMap6,
		MasterMap7,
		MasterMap8,
		MasterMap9,
		MasterMap10,
		MasterMap11,
		MasterMap12,
		MasterMap13);
}

void TestMaster_post_emcy(CO_Data* d, uint8_t nodeID, uint16_t errCode, uint8_t errReg)
{
	eprintf("Master received EMCY message. Node: %2.2x  ErrorCode: %4.4x  ErrorRegister: %2.2x\n", nodeID, errCode, errReg);
}

char query_result = 0;
char waiting_answer = 0;

static void CheckSDO(CO_Data* d, uint8_t nodeId)
{
	uint32_t abortCode;
	static bool bLEDOn = false;

	if(getWriteResultNetworkDict (d, nodeId, &abortCode) != SDO_FINISHED)
	{
		eprintf("Master : Failed in changing Slave's transmit type AbortCode :%4.4x \n", abortCode);
		if(bLEDOn == true)
		{
			bLEDOn = false;
			GPIO_WriteBit(GPIOF, GPIO_Pin_9, Bit_RESET);
		}
		else
		{
			bLEDOn = true;
			GPIO_WriteBit(GPIOF, GPIO_Pin_9, Bit_SET);
		}		
	}

	/* Finalise last SDO transfer with this node */
	closeSDOtransfer(&TestMaster_Data, nodeId, SDO_CLIENT);
}

static int MasterSyncCount = 0;
void TestMaster_post_TPDO(CO_Data* d)
{
	eprintf("TestMaster_post_TPDO MasterSyncCount = %d \n", MasterSyncCount);

	if(MasterSyncCount % 17 == 0){
		eprintf("Master : Ask RTR PDO (0x1402)\n");
		sendPDOrequest(&TestMaster_Data, 0x1402 );
		sendPDOrequest(&TestMaster_Data, 0x1403 );
	}
	if(MasterSyncCount % 50 == 0){
		uint8_t transmitiontype;
		eprintf("Master : Change slave's transmit type to 0xFF\n");
		transmitiontype = 0xFF;
		writeNetworkDictCallBack (&TestMaster_Data, /*CO_Data* d*/
					2, 								/*uint8_t nodeId*/
					0x1802, 					/*uint16_t index*/
					0x02, 						/*uint16_t index*/
					1, 								/*uint8_t count*/
					0, 								/*uint8_t dataType*/
					&transmitiontype,	/*void *data*/
					CheckSDO, 				/*SDOCallback_t Callback*/
          0); 							/* use block mode */
	}   
	if(MasterSyncCount % 50 == 25){
		uint8_t transmitiontype = 0x00;
		eprintf("Master : Change slave's transmit type to 0x00\n");
		writeNetworkDictCallBack (&TestMaster_Data, /*CO_Data* d*/
					2, /*uint8_t nodeId*/
					0x1802, /*uint16_t index*/
					0x02, /*uint16_t index*/
					1, /*uint8_t count*/
					0, /*uint8_t dataType*/
					&transmitiontype,/*void *data*/
					CheckSDO, /*SDOCallback_t Callback*/
          0); /* use block mode */
	}
	MasterSyncCount++;
}

void TestMaster_post_SlaveBootup(CO_Data* d, uint8_t nodeid)
{
	eprintf("TestMaster_post_SlaveBootup %x\n", nodeid);
	
	ConfigureSlaveNode(d, nodeid);
}
