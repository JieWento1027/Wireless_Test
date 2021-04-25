#include "sys.h"

#define LED PBout(1)

void InitAD7715(void);
void spi_write(uint8_t spi_data);
uint16_t spi_read(void);    
uint16_t GetADNum(void);


