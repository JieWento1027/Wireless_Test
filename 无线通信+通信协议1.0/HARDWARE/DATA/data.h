#ifndef __DATA_H
#define __DATA_H
#include "sys.h"

void Sent_A_data_to_PC(void);
void Process_msg(u8 *pBuf,u8 len);

extern u8 singleA_Flag;
		 
#endif
