#include "sys.h"

u16 pumpData_A;
u8 singleA_Flag=0;

int main(void)
{	
	u8 i;
	u8 Rx_buffer1[32],Rx_buffer2[32],Rx_buffer3[32];
	InitAD7715();
	delay_init();	    	    //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(9600);	        //���ڳ�ʼ��Ϊ9600
	NRF24L01_Init();            //��ʼ��NRF24L01 
	
	while(NRF24L01_Check());
	
	while(1)
    { 
	   NRF24L01_RX_Mode();	
	   pumpData_A = GetADNum_Channel2();
       delay_ms(200);
	   while(NRF24L01_RxPacket(Rx_buffer1,Rx_buffer2,Rx_buffer3)==0)
	   {
		   if(Rx_buffer1[3]==0x12&&Rx_buffer2[0]==0&&Rx_buffer3[0]==0)
		   {
			   Process_msg(Rx_buffer1,6);//ͨ��Э�����
		   }
		   else
		   {
			   for(i=0;i<Rx_buffer1[0];i++)
			   {
				  while((USART2->SR&0X40)==0);
				  USART2->DR=Rx_buffer1[i+1];
			   }
			   for(i=0;i<Rx_buffer2[0];i++)
			   {
				  while((USART2->SR&0X40)==0);
				  USART2->DR=Rx_buffer2[i+1];
			   }
			   for(i=0;i<Rx_buffer3[0];i++)
			   {
				  while((USART2->SR&0X40)==0);
				  USART2->DR=Rx_buffer3[i+1];
			   }
		   }
	   } 
	   NRF24L01_TX_Mode();
	   if( singleA_Flag==1 )
	   {
		   Sent_A_data_to_PC();
	   }
	   LED=~LED;
	}
}
