#include "usart1.h"
#include "stdio.h" 
////////////////////////////////////////////////////////////////////////////////// 	 


 //////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
_ttywrch(int ch)
{
    ch=ch;
}
//???fputc?? 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//????,??????   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


 
#if EN_USART1_RX   //???????
//??1??????
//??,??USARTx->SR??????????   	
u8 USART_RX_BUF[USART_REC_LEN];     //????,??USART_REC_LEN???.
//????
//bit15,	??????
//bit14,	???0x0d
//bit13~0,	??????????
u16 USART_RX_STA=0;       //??????	  

//???IO ??2 
//bound:???
void uart1_init(u32 bound){
    //GPIO????
    GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
	  
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//??USART2,GPIOA??
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		//USART_DeInit(USART1);  //????2
	 //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
    GPIO_Init(GPIOA, &GPIO_InitStructure); //???PA2
   
    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //???PA3

   //Usart2 NVIC ??

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
		NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
  
   //USART ?????

	  USART_InitStructure.USART_BaudRate = bound;//?????9600;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	  USART_InitStructure.USART_Parity = USART_Parity_No;//??????
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????

    USART_Init(USART1, &USART_InitStructure); //?????
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//????
    USART_Cmd(USART1, ENABLE);                    //???? 

}

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 


//void USART2_IRQHandler(void)                	//??1??????
//	{
//	u8 Res;

//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //????(?????????0x0d 0x0a??)
//		{
//		Res =USART_ReceiveData(USART2);//(USART1->DR);	//????????
//		
//	
//     } 

//} 
#endif	

