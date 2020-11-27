#include"main.h"



extern void can_recv_thread(void);
extern unsigned char CanreveiveFlag;
extern Message m;
/**************************************************************************

**************************************************************************/
int main(void)
{
        
//	 int i;
     uart1_init(115200);
	  //printf("hello world......\n");
	  
	 /*while(1)
	  {
			  i++;
            printf("hello world\r\n");
			if(i > 5000) break;

	  }*/
		can_recv_thread();
	    while(1)
		{
       //printf("hello world.......\n");
           // if(CanreveiveFlag==1)
          //  {
					//		printf("hello world......\n");
          //  	 TIM5->DIER &= (uint16_t)~TIM_IT_CC1;
           //    canDispatch(&TestSlave_Data, &m);
			/* Enable the Interrupt sources */
           //   TIM5->DIER |= TIM_IT_CC1;
            //    CanreveiveFlag=0;
          //  }
		}


}//end main


