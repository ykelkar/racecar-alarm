#ifndef _GPIO_H
#define _GPIO_H

#include "stdutils.h"


typedef enum
{
    PA_0,PA_1,PA_2,PA_3,PA_4,PA_5,PA_6,PA_7,
    PB_0,PB_1,PB_2,PB_3,PB_4,PB_5,PB_6,PB_7,
    PC_0,PC_1,PC_2,PC_3,PC_4,PC_5,PC_6,PC_7,
    PD_0,PD_1,PD_2,PD_3,PD_4,PD_5,PD_6,PD_7,
    PE_0,PE_1,PE_2,PE_3,PE_4,PE_5,PE_6,PE_7,
	PF_0,PF_1,PF_2,PF_3,PF_4,PF_5,PF_6,PF_7,
	PG_0,PG_1,PG_2,PG_3,PG_4,PG_5,PG_6,PG_7,
    P_NC = 0xff
}gpioPins_et;


void GPIO_PinDirection(gpioPins_et enm_pinNumber, uint8_t v_pinDirn_u8);
void GPIO_PinWrite(gpioPins_et enm_pinNumber, uint8_t v_pinValue_u8);
uint8_t GPIO_PinRead(gpioPins_et enm_pinNumber);

#endif