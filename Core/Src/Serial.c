/* *****************************************************************************
 *
 *
 *******************************************************************************

/*******************************************************************************
 * Files inclusion
 ******************************************************************************/
#include "Serial.h"
#include <stdarg.h>
/*******************************************************************************
 * Internal Macros
 ******************************************************************************/


/*******************************************************************************
 * Static function declaration
 ******************************************************************************/


/* *****************************************************************************
 * Variables
 ******************************************************************************/
 uint8_t aRxBuffer[255];

 bool dataComing_b = false;

 Serial_tst serial_st;
/* *****************************************************************************
 * Static function definition
 ******************************************************************************/



/* *****************************************************************************
 * Public function definition
 ******************************************************************************/
void Serial_Write(Serial_tst *serial_pst, uint8_t *buff_pu8, uint16_t len_u16)
{
    HAL_StatusTypeDef ret = HAL_ERROR;

    ret = HAL_UART_Transmit(serial_pst->uartHdl_pst, buff_pu8, len_u16, 0xFFFF);
//    HAL_Delay(1);
    if(ret == HAL_OK)
    {
        if(HAL_UART_Receive_IT(serial_pst->uartHdl_pst, (uint8_t *)&g_incomming, 1)==HAL_OK)
        {

        }
    }
}

void Serial_Init(struct Serial *serial_pst)
{
    rb_init(&serial_pst->rbRx_st, USART_RX_BUF_SIZE, serial_pst->rx_buf);
}

uint16_t Serial_Available(struct Serial *serial_pst)
{
    uint16_t ret = rb_full_count(&serial_pst->rbRx_st);
    if(ret > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

void Serial_Read(struct Serial *serial_pst, uint8_t* incomming_u8)
{
    static int count = 0;
    uint8_t ret = rb_remove(&serial_pst->rbRx_st);
    if(ret == 0xB5)
    {
        count++;
    }
    *incomming_u8 = ret;

    return;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static int i = 0;
    int j = 0;
    if(huart->Instance == USART2)
    {
        aRxBuffer[i] = g_incomming;
        i++;

        rb_push_insert(&serial_st.rbRx_st, g_incomming);
        if(HAL_UART_Receive_IT(serial_st.uartHdl_pst, (uint8_t *)&g_incomming, 1)==HAL_OK)
        {

        }
        if(i == 256)
        {
            j = i;
        }
        if(g_incomming == 0xB5)
        {
            j = i;
        }
    }
}

void vprint(const char *fmt, va_list argp)
{
    char string[200];
    if(0 < vsprintf(string,fmt,argp)) // build string
    {
        printf("\r%s\n", string);
//        DWT_Delay_us(5);
    }
}

void my_printf(const char *fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    vprint(fmt, argp);
    va_end(argp);
}
