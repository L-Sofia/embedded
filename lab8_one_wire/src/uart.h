#ifndef UART_H_
#define UART_H_

#include <stdint.h>

typedef void (*UASART_line_rcv_t)(uint8_t *);

void USART_Init(uint32_t baud);
void USART_PutChar(uint8_t data);
uint8_t USART_Receive(void);

void USART_set_callback(UASART_line_rcv_t clbk);
void USART_poll(void);

#if defined(DEBUG)
void printlog(const char *fmt, ...);
#define LOG_DEBUG(tg, fmt, ...) \
    printlog("D [%s] (%s:%d) " fmt "\n", tg, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define LOG_DEBUG(tg, fmt, ...)
#endif

#endif