#include <avr/io.h>
#include "stdutils.h"
#include "gpio.h"





/***************************************************************************************************
                  void GPIO_PinDirection(gpioPins_et enm_pinNumber, uint8_t v_pinDirn_u8)
 ***************************************************************************************************
 * I/P Arguments: 
                gpioPins_et: pin number which needs to be updated.
                             Refer the gpio.h for pin numbering.
                gpioDirection_et: Direction to be configured ie. INPUT/OUTPUT.
                              

 * Return value    : none

 * description :This function sets the specified direction as INPUT/OUTPUT.  
 ***************************************************************************************************/
void GPIO_PinDirection(gpioPins_et enm_pinNumber, uint8_t v_pinDirn_u8)
{

    uint8_t v_portNumber_u8;


    v_portNumber_u8 =  (enm_pinNumber>>3);  // Divide the pin number by 8 go get the PORT number
    enm_pinNumber  =   enm_pinNumber & 0x07;  // lower 3-bits contains the bit number of a byte 

    /* Go to particular port after decoding from the pin number and 
        set the direction as specified*/
    switch(v_portNumber_u8)
    {
#ifdef DDRA        
    case 0:
        util_UpdateBit(DDRA,enm_pinNumber,v_pinDirn_u8);
        break;
#endif        


#ifdef DDRB
    case 1:
        util_UpdateBit(DDRB,enm_pinNumber,v_pinDirn_u8);
        break;
#endif 

        
#ifdef DDRC
    case 2:
        util_UpdateBit(DDRC,enm_pinNumber,v_pinDirn_u8);
        break;
#endif 

        
#ifdef DDRD        
    case 3:
        util_UpdateBit(DDRD,enm_pinNumber,v_pinDirn_u8);
        break;
#endif      
    
    }                
}









/***************************************************************************************************
            void GPIO_PinWrite(gpioPins_et enm_pinNumber, uint8_t v_pinValue_u8)
 ***************************************************************************************************
 * I/P Arguments: 
                gpioPins_et: pin number which needs to be updated.
                             Refer the gpio.h for pin numbering.
                gpioValue_et: Value to be updated(LOW/HIGH) on the selected pin.
                              

 * Return value    : none

 * description :This function updates the specified value on the selected pin.  
                Before updating the pins status, the pin function should be selected and then
                the pin should be configured as OUTPUT 
***************************************************************************************************/
void GPIO_PinWrite(gpioPins_et enm_pinNumber, uint8_t v_pinValue_u8)
{

    uint8_t v_portNumber_u8;


    v_portNumber_u8 =  (enm_pinNumber>>3);  // Divide the pin number by 8 go get the PORT number
    enm_pinNumber  =   enm_pinNumber & 0x07;  // lower 3-bits contains the bit number of a byte 

    /* Go to particular port after decoding from the pin number and 
        update the value of the specified pin*/
    switch(v_portNumber_u8)
    {
#ifdef PORTA        
    case 0:
        util_UpdateBit(PORTA,enm_pinNumber,v_pinValue_u8);
        break;
#endif        

        
#ifdef PORTB
    case 1:
        util_UpdateBit(PORTB,enm_pinNumber,v_pinValue_u8);
        break;
#endif        

        
#ifdef PORTC        
    case 2:
        util_UpdateBit(PORTC,enm_pinNumber,v_pinValue_u8);
        break;
#endif        

        
#ifdef PORTD        
    case 3:
        util_UpdateBit(PORTD,enm_pinNumber,v_pinValue_u8);
        break;
#endif   


#ifdef PORTE
    case 4:
        util_UpdateBit(PORTE,enm_pinNumber,v_pinValue_u8);
        break;
#endif        

        
#ifdef PORTF        
    case 5:
        util_UpdateBit(PORTF,enm_pinNumber,v_pinValue_u8);
        break;
#endif        

        
#ifdef PORTG        
    case 6:
        util_UpdateBit(PORTG,enm_pinNumber,v_pinValue_u8);
        break;
#endif      
    }                
}








/***************************************************************************************************
                    uint8_t GPIO_Read(gpioPins_et enm_pinNumber)
 ***************************************************************************************************
 * I/P Arguments: 
                gpioPins_et: pin number which needs to be read.
                             Refer the gpio.h for pin numbering.

 * Return value    : 
                 uint8_t:    Status of the specified pin.

 * description :This function returns the status of the selected pin.
                Before reading the pins status, the pin should be configured as INPUT 
 ***************************************************************************************************/
uint8_t GPIO_PinRead(gpioPins_et enm_pinNumber)
{
    uint8_t v_portNumber_u8;
    uint8_t returnStatus = 0;

    v_portNumber_u8 =  (enm_pinNumber>>3); // Divide the pin number by 8 go get the PORT number
    enm_pinNumber  =     enm_pinNumber & 0x07; // lower 3-bits contains the bit number of a byte 

    /* Go to particular port after decoding from the pin number and read the pins status */
    switch(v_portNumber_u8)     
    { 
#ifdef PINA    
    case 0: 
        returnStatus = util_IsBitSet(PINA,enm_pinNumber);
        break;
#endif        

#ifdef PINB
    case 1:
        returnStatus = util_IsBitSet(PINB,enm_pinNumber);
        break;
#endif          

#ifdef PINC
    case 2:
        returnStatus = util_IsBitSet(PINC,enm_pinNumber);
        break;
#endif        
        
        
#ifdef PIND
    case 3:
        returnStatus = util_IsBitSet(PIND,enm_pinNumber);
        break;
#endif   


#ifdef PINE    
    case 4: 
        returnStatus = util_IsBitSet(PINE,enm_pinNumber);
        break;
#endif        


#ifdef PINF
    case 5:
        returnStatus = util_IsBitSet(PINF,enm_pinNumber);
        break;
#endif

        
#ifdef PING
    case 6:
        returnStatus = util_IsBitSet(PING,enm_pinNumber);
        break;
#endif             
    }                         
    return returnStatus;
}
