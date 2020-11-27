/**
  ******************************************************************************
  * @file CAN/Normal/main.c 
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "platform_config.h"

/* test_master */
#include "canfestival.h"
#include "Master.h"
#include "Slave.h"
#include "TestMasterSlave.h"

#include <rtthread.h>

/** @addtogroup StdPeriph_Examples
  * @{
  */

/** @addtogroup CAN_Normal
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
CanTxMsg TxMessage;
CanRxMsg RxMessage;

/* Private function prototypes -----------------------------------------------*/
static void RCC_Configuration(void);
static void GPIO_Configuration(void);
static void NVIC_Configuration(void);  
static void CAN_Config(void);
static void LED_Display(uint8_t Ledstatus);

extern void TIM5_start(void);
static int  test_master(void);

#define 	RX_BUF_LEN		1024
Message   rx_msg_buf[RX_BUF_LEN];
uint32_t	rx_save, rx_read;

#define 	TX_BUF_LEN		1024
uint32_t 	tx_save, tx_read;
CanTxMsg 	tx_msg_buf[TX_BUF_LEN];

static rt_sem_t recv_sem = RT_NULL;
static rt_sem_t tran_sem = RT_NULL;

/* Private functions ---------------------------------------------------------*/

void can_master_init(void)
{
  /* System clocks configuration ---------------------------------------------*/
  RCC_Configuration();

  /* NVIC configuration ------------------------------------------------------*/
  NVIC_Configuration();

  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration();

	rx_save = 0;
	rx_read = 0;

	tx_save = 0;
	tx_read = 0;

  /* CAN configuration */
  CAN_Config();

  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

  /* turn off all leds*/
  LED_Display(5);
}

/**
  * @brief  Main program
  * @param  None
  * @retval : None
  */
void can_recv_thread(void* parameter)
{
	recv_sem = rt_sem_create("recvsem", 0, RT_IPC_FLAG_FIFO);
	tran_sem = rt_sem_create("transem", 0, RT_IPC_FLAG_FIFO);

	can_master_init();

	test_master();

  /* Infinite loop*/
  while(1)
  {	
		rt_sem_take(recv_sem, RT_WAITING_FOREVER);

		{
			uint32_t next;
			Message *pmsg;
			
			next = rx_read;
			pmsg = &rx_msg_buf[next];

    	/* Disable the Interrupt sources */
    	TIM5->DIER &= (uint16_t)~TIM_IT_CC1;
			canDispatch(&TestMaster_Data, pmsg);
			/* Enable the Interrupt sources */
    	TIM5->DIER |= TIM_IT_CC1;

			next++;
			if(next >= RX_BUF_LEN) next = 0;
			rx_read = next;
		}
  }
}

void can_send_thread(void *parameter)
{
	while(1)
	{
		rt_sem_take(tran_sem, RT_WAITING_FOREVER);

		{
			uint32_t next;
			uint8_t  mailbox_no;
			CanTxMsg *ptx_msg;
			
			next = tx_read;
			ptx_msg = &tx_msg_buf[next];

			mailbox_no = CAN_Transmit(CAN1, ptx_msg);
			if(mailbox_no != CAN_NO_MB)
			{
				next++;
				if(next >= TX_BUF_LEN) next = 0;
				tx_read = next;			
			}
			else
			{
				rt_sem_release(tran_sem);
			} 
		}
	}		
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval : None
  */
static void RCC_Configuration(void)
{
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
     initialize the PLL and update the SystemFrequency variable. */
  /* The System frequency should be set to HSE frequency */
  
  /* GPIO clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIO_CAN |RCC_APB2Periph_GPIO_LED | RCC_APB2Periph_GPIO_Key , ENABLE);

  /* CAN1 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
}

/**
  * @brief  Configures the GPIO.
  * @param  None
  * @retval : None
  */
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure GPIO_LED pin6, pin7, pin8 and pin9 as Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_LED, &GPIO_InitStructure);

  /* Configure CAN pin: RX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_RX;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
  
  /* Configure CAN pin: TX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_TX;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
  
  GPIO_PinRemapConfig(GPIO_Remap_CAN , ENABLE);

  /* (key button) as output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Key ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIO_Key, &GPIO_InitStructure);
}

/**
  * @brief  Configures the NVIC for CAN and joystick
  * @param  None
  * @retval : None
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Turn ON/OFF the dedicate led
  * @param Ledstatus: Led number from 0 to 3 
  * @retval : None
  */
void LED_Display(uint8_t Ledstatus)
{
  /* turn off all leds*/
  GPIO_WriteBit(GPIO_LED, GPIO_Pin_6, Bit_RESET);
  GPIO_WriteBit(GPIO_LED, GPIO_Pin_7, Bit_RESET);
  GPIO_WriteBit(GPIO_LED, GPIO_Pin_8, Bit_RESET);
  GPIO_WriteBit(GPIO_LED, GPIO_Pin_9, Bit_RESET);
  
  switch(Ledstatus)
  {
   case(1): GPIO_WriteBit(GPIO_LED, GPIO_Pin_6, Bit_SET);
            break;
   
   case(2): GPIO_WriteBit(GPIO_LED, GPIO_Pin_7, Bit_SET);
            break;
 
   case(3): GPIO_WriteBit(GPIO_LED, GPIO_Pin_8, Bit_SET);
            break;

   case(4): GPIO_WriteBit(GPIO_LED, GPIO_Pin_9, Bit_SET);
            break;
  default:
        break;
   }
}

/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval : None
  */
static void CAN_Config(void)
{
  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_12tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler = 2;
  CAN_Init(CAN1, &CAN_InitStructure);

  /* CAN filter init */
 	/* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode   = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale  = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow  = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow  = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
   /* transmit */
  TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x00;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 1;
}

#include <applicfg.h>
unsigned char canSend(CAN_PORT notused, Message *m)
{
	{
		uint32_t	i;
		uint32_t  next;
		static uint32_t led_state;
	
		next = tx_save + 1;
		if(next >= TX_BUF_LEN) next = 0;
		if(next != tx_read)
		{
			CanTxMsg *ptx_msg;

			ptx_msg = &tx_msg_buf[tx_save];
			ptx_msg->StdId = m->cob_id;
	
			if(m->rtr)
	  		ptx_msg->RTR = CAN_RTR_REMOTE;
			else
				ptx_msg->RTR = CAN_RTR_DATA;

	  	ptx_msg->IDE = CAN_ID_STD;
  		ptx_msg->DLC = m->len;
			for(i = 0; i < m->len; i++)
				ptx_msg->Data[i] = m->data[i];
	
			tx_save = next;

			rt_sem_release(tran_sem);
			return 0x00;																				  
		}
		if(++led_state & 0x01)
			GPIO_SetBits(GPIOF, GPIO_Pin_9);
		else
			GPIO_ResetBits(GPIOF, GPIO_Pin_9);
		return 0xFF;
	}
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	{
		Message *pmsg;
		uint32_t i, next;

		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

		next = (rx_save + 1) ;
		if(next >= RX_BUF_LEN) next = 0;
		if(next == rx_read) return;

		pmsg = &rx_msg_buf[rx_save];
		pmsg->cob_id = (uint16_t)RxMessage.StdId;		/**< message's ID */
		if(RxMessage.RTR == CAN_RTR_REMOTE)				/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
			pmsg->rtr = 1;		
		else
			pmsg->rtr = 0;

	  pmsg->len  = (uint8_t)RxMessage.DLC;					/**< message's length (0 to 8) */
		for(i = 0; i < pmsg->len; i++)
			pmsg->data[i] = RxMessage.Data[i];

		rx_save = next;
		rt_sem_release(recv_sem);
	}
}

static TimerCallback_t init_callback;

uint32_t OnMasterMap1Update(CO_Data* d, const indextable * unsused_indextable, uint8_t unsused_bSubindex)
{
	eprintf("OnMasterMap1Update:%d\n", MasterMap1);
	return 0;
}

s_BOARD MasterBoard = {"1", "1M"};

void help(void)
{
  eprintf("**************************************************************\n");
  eprintf("*  TestMasterSlave                                           *\n");
  eprintf("*                                                            *\n");
  eprintf("*  A simple example for PC. It does implement 2 CanOpen      *\n");
  eprintf("*  nodes in the same process. A master and a slave. Both     *\n");
  eprintf("*  communicate together, exchanging periodically NMT, SYNC,  *\n");
  eprintf("*  SDO and PDO. Master configure heartbeat producer time     *\n");
  eprintf("*  at 1000 ms for slave node-id 0x02 by concise DCF.         *\n");                                  
  eprintf("*                                                            *\n");
  eprintf("*   Usage:                                                   *\n");
  eprintf("*   ./TestMasterSlave  [OPTIONS]                             *\n");
  eprintf("*                                                            *\n");
  eprintf("*   OPTIONS:                                                 *\n");
  eprintf("*     -l : Can library [\"libcanfestival_can_virtual.so\"]     *\n");
  eprintf("*                                                            *\n");
  eprintf("*    Slave:                                                  *\n");
  eprintf("*     -s : bus name [\"0\"]                                    *\n");
  eprintf("*     -S : 1M,500K,250K,125K,100K,50K,20K,10K,none(disable)  *\n");
  eprintf("*                                                            *\n");
  eprintf("*    Master:                                                 *\n");
  eprintf("*     -m : bus name [\"1\"]                                    *\n");
  eprintf("*     -M : 1M,500K,250K,125K,100K,50K,20K,10K,none(disable)  *\n");
  eprintf("*                                                            *\n");
  eprintf("**************************************************************\n");
}

/***************************  INIT  *****************************************/
void InitNodes(CO_Data* d, uint32_t id)
{
	/****************************** INITIALISATION MASTER *******************************/
	if(strcmp(MasterBoard.baudrate, "none")){
 		RegisterSetODentryCallBack(&TestMaster_Data, 0x2000, 0, &OnMasterMap1Update);
		
		/* Defining the node Id */
		setNodeId(&TestMaster_Data, 0x01);

		/* init */
		setState(&TestMaster_Data, Initialisation);
	}
}

static StartTimerLoop(TimerCallback_t _init_callback) 
{
	init_callback = _init_callback;

	SetAlarm(NULL, 0, init_callback, 0, 0);
	TIM5_start();
}

/***************************  EXIT  *****************************************/
void Exit(CO_Data* d, uint32_t id)
{
	if(strcmp(MasterBoard.baudrate, "none")){
		
		masterSendNMTstateChange(&TestMaster_Data, 0x02, NMT_Reset_Node);    
    
   	//Stop master
		setState(&TestMaster_Data, Stopped);
	}
}

/****************************************************************************/
/****************************  test_master  *********************************/
/****************************************************************************/
static int test_master(void)
{
	if(strcmp(MasterBoard.baudrate, "none"))
	{
		TestMaster_Data.heartbeatError = TestMaster_heartbeatError;
		TestMaster_Data.initialisation = TestMaster_initialisation;
		TestMaster_Data.preOperational = TestMaster_preOperational;
		TestMaster_Data.operational = TestMaster_operational;
		TestMaster_Data.stopped = TestMaster_stopped;
		TestMaster_Data.post_sync = TestMaster_post_sync;
		TestMaster_Data.post_TPDO = TestMaster_post_TPDO;
		TestMaster_Data.post_emcy = TestMaster_post_emcy;
		TestMaster_Data.post_SlaveBootup=TestMaster_post_SlaveBootup;
	}

	// Start timer thread
	StartTimerLoop(&InitNodes);
	return 0;
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
