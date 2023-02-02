 /* 
 * File:   application.c
 * Author: Mohamed_Nagy
 * https://www.linkedin.com/in/mohamednagyofficial/
 * Created on January 27, 2022, 8:41 PM
 */

#include "application.h"
Std_ReturnType ret = E_NOT_OK;
void usart_tx_int_handeler();
void usart_rx_int_handeler();
void usart_overRun_handeler();
void usart_framing_handeler();

led_t led_1 = 
{
    .port_name = PORTC_INDEX,
    .pin = GPIO_PIN0,
    .led_status = LED_OFF
};

led_t led_2 = 
{
    .port_name = PORTC_INDEX,
    .pin = GPIO_PIN1,
    .led_status = LED_OFF
};

usart_t usart_1 = 
{
    .baudrate = 9600,
    .baudrate_gen_gonfig = BAUDRATE_ASYN_8BIT_lOW_SPEED,
            
    .usart_tx_cfg.usart_tx_enable = EUSART_ASYNCHRONOUS_TX_ENABLE,
    .usart_tx_cfg.usart_tx_interrupt_enable = EUSART_ASYNCHRONOUS_INTERRUPT_TX_ENABLE,
    .usart_tx_cfg.usart_tx_9bit_enable = EUSART_ASYNCHRONOUS_9Bit_TX_DISABLE,
    
    .usart_rx_cfg.usart_rx_enable = EUSART_ASYNCHRONOUS_RX_ENABLE,
    .usart_rx_cfg.usart_rx_interrupt_enable = EUSART_ASYNCHRONOUS_INTERRUPT_RX_ENABLE,
    .usart_rx_cfg.usart_rx_9bit_enable = EUSART_ASYNCHRONOUS_9Bit_RX_DISABLE,    

    .EUSART_FramingErrorHandler = usart_framing_handeler,
    .EUSART_OverrunErrorHandler = usart_overRun_handeler,
    .EUSART_RxDefaultInterruptHandler = usart_rx_int_handeler,
    .EUSART_TxDefaultInterruptHandler = usart_tx_int_handeler
};

volatile uint32 rx_counter, tx_counter;
uint8 tx_data, rx_data;

int main() 
{ 
    application_intialize();
    while(1)
    {    
        
    }   
    return (EXIT_SUCCESS);
}

void application_intialize(void)
{
    ecu_layer_intialize();
    ret = led_initialize(&led_1);
    ret = led_initialize(&led_2);
    ret = EUSART_ASYNC_Init(&usart_1);
}

void usart_tx_int_handeler()
{
    tx_counter++;
}

void usart_rx_int_handeler()
{
    rx_counter++;
    ret = EUSART_ASYNC_ReadByteBlocking(&rx_data);
    if(rx_data == 'a')
    {
        ret = led_turn_on(&led_1);
        ret = EUSART_ASYNC_WriteStringBlocking("LED 1 ON\r", 9);
    }
    else if(rx_data == 'b')
    {
        ret = led_turn_off(&led_1);
        ret = EUSART_ASYNC_WriteStringBlocking("LED 1 OFF\r", 10);
    }
    else if(rx_data == 'c')
    {
        ret = led_turn_on(&led_2);
        ret = EUSART_ASYNC_WriteStringBlocking("LED 2 ON\r", 9);
    }
    else if(rx_data == 'd')
    {
        ret = led_turn_off(&led_2);
        ret = EUSART_ASYNC_WriteStringBlocking("LED 2 OFF\r", 10);
    }
    else
    {
        ret = led_turn_off(&led_1);
        ret = led_turn_off(&led_2);
        ret = EUSART_ASYNC_WriteStringBlocking("LED 1 and 2 OFF\r", 16);
    }
}

void usart_overRun_handeler()
{
    ret = EUSART_ASYNC_RX_Restart();
}


void usart_framing_handeler()
{
    uint8 framingErrorData;
    ret = EUSART_ASYNC_ReadByteBlocking(&framingErrorData);
}
