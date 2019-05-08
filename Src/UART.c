#include "UART.h"

void UART_Init(void)
{
    uint32_t err_code;
    const app_uart_comm_params_t comm_params =
    {
        IMU_RX,
        IMU_TX,
        IMU_RTS,
        IMU_CTS,
        APP_UART_FLOW_CONTROL_DISABLED,
        true,
        NRF_UART_BAUDRATE_115200
    };
    APP_UART_FIFO_INIT(&comm_params,
                        UART_RX_BUF_SIZE,
                        UART_TX_BUF_SIZE,
                        uart_error_handle,
                        APP_IRQ_PRIORITY_LOWEST,
                        err_code);
    APP_ERROR_CHECK(err_code);
    __asm{NOP};
}

void uart_error_handle(app_uart_evt_t * p_event)
{
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR)
    {
        //APP_ERROR_HANDLER(p_event->data.error_communication);
    }
    else if (p_event->evt_type == APP_UART_FIFO_ERROR)
    {
        //APP_ERROR_HANDLER(p_event->data.error_code);
    }
}

void UART_Send_Multiple(uint8_t * data, uint8_t len)
{
    for(int i = 0; i < len; i++)
    {
        while (app_uart_put(data[i]) != NRF_SUCCESS);
    }
}
