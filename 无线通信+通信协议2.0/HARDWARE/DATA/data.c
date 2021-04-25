#include "data.h"

void Sent_A_data_to_PC(void)
{
	u8 send_buffer[11];

	send_buffer[0] = 10;                //�������鳤��
	send_buffer[1] = 0x81;              //Э��ͷ
	send_buffer[2] = 0x0a;              //����֡����
	send_buffer[3] = 0x21;              //��������
	send_buffer[4] = 0x0a;              //ָ����
	send_buffer[5] = pumpData_A>>8;     //A·��ǿ���ֽ�
	send_buffer[6] = pumpData_A&0x00ff; //A·��ǿ���ֽ�
	send_buffer[7] = 0;
	send_buffer[8] = 0;
	send_buffer[9] = send_buffer[1] + send_buffer[2] + send_buffer[3] + send_buffer[4] + 
	                 send_buffer[5] + send_buffer[6] + send_buffer[7] + send_buffer[8];
	send_buffer[10] = 0x82;             //Э��β
	NRF24L01_TxPacket(send_buffer);     //���͸�����
}

void Process_msg(u8 *pBuf,u8 len)
{
	u8 i;
	u8 Cmd_msg[6];
	u8 ctr,Sum_check=0;
	
	for(ctr=0; ctr<len; ctr++) Cmd_msg[ctr]=*(pBuf + ctr + 1);
	
	for(i=0;i<4;i++) Sum_check+=Cmd_msg[i];//У���

	if(Cmd_msg[0]==0x81 && Cmd_msg[1]==0x06 && Cmd_msg[2]==0x12 && Cmd_msg[4] == Sum_check && Cmd_msg[5]==0x82)
	{
		if(Cmd_msg[3]==0x0a)
		{
			singleA_Flag=1;//����A·��ǿ��־λ��1
		}
		else if(Cmd_msg[3]==0xfa)
		{
			singleA_Flag=0;//����A·��ǿ��־λ��0
		}
		else
			return;
	}
}


