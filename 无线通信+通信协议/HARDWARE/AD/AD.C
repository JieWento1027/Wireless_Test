#include "AD.h"

void InitAD7715(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTB时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
	
	//LED引脚
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置成推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//AD7715时钟引脚
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;//PB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置成推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB10
	
	//AD7715串行数据输入端
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;//PB12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置成推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB12
	
	//AD7715串行数据输出端
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;//PB13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //设置成浮空输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB13

    //AD7715逻辑输出端
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;//PB14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PB14设置成输入，默认上拉	  
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB14 
    spi_write(0x10);   //设置增益为1；同时设定下次为设置设置寄存器；
    spi_write(0x62);   //设置校准为自标定，主频率为大于2M，更新频率为50Hz,单极，使用片内寄存器，系统时钟频率设置为2.4576MHz；
                       //官方网站上提到，大于2M时，CLK应设为1，以便提供足够的电流给芯片工作
}

uint16_t GetADNum(void)
{
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)!= 0);   //等待转换完成；（PB14接DRDY—，当完成全部16位的读操作时，此引脚为1）
    spi_write(0x38); //设置增益为1；同时设定下次为读取数据寄存器。
    return spi_read();  
}

/*******************************
    向SPI器件写入一个字节数据
*******************************/
void spi_write(uint8_t spi_data)
{
	uint8_t i;    
    for(i=0;i<8;i++)  
    {
        if((spi_data&0x80)==0x80)
		    GPIO_SetBits(GPIOB, GPIO_Pin_12);  
        else 
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	    GPIO_SetBits(GPIOB, GPIO_Pin_10);
        GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	    GPIO_SetBits(GPIOB, GPIO_Pin_10);
	    spi_data=(spi_data<<1);
    }	
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
}


/********************************
  从SPI器件读出两个字节数据
********************************/
uint16_t spi_read()
{ 
	
    uint8_t p,spi_dat1,spi_dat2;
    uint16_t spi_dat=0;
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)!= 0);    
    for (p=0;p<8;p++)  
    {
		
        GPIO_SetBits(GPIOB, GPIO_Pin_10);
        GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	    GPIO_SetBits(GPIOB, GPIO_Pin_10);
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)==1)
		    spi_dat1|=0x01;  
        else spi_dat1&=~0x01;
            spi_dat1=(spi_dat1<<1);  
    }  
    for (p=0;p<8;p++)  
    {
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
        GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	    GPIO_SetBits(GPIOB, GPIO_Pin_10);
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)==1)
		    spi_dat2|=0x01;  
        else spi_dat2&=~0x01;
            spi_dat2=(spi_dat2<<1);  
    } 
    spi_dat|=spi_dat1;
    spi_dat=(spi_dat<<8);
    spi_dat|=spi_dat2;
    return spi_dat;  
}
