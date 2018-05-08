#ifndef _DELAY_H
#define _DELAY_H

#include "stdutils.h"

/* Count to generate 1ms delay at 20Mhz clock.
  The value is obtained by toggling the Port pins to generate 2Kz square wave */
#define C_CountForOneMsDelay_U16 300u


/***************************************************************************************************
                 PreCompile configurations to enable/disable the functions
 ***************************************************************************************************
PreCompile configuration to enable or disable the API's.
 1.Required interfaces can be enabled/disabled by configuring its respective macros to 1/0.
***************************************************************************************************/
#define    ENABLE_DELAY_sec     0
/**************************************************************************************************/

void DELAY_us(uint16_t us_count);
void DELAY_ms(uint16_t ms_count);
void DELAY_sec(uint16_t var_delaySecCount_u16);

#endif