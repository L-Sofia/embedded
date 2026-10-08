#include <avr/io.h>
#include "uart.h"
#include <stdio.h>

static uint8_t sbuf[80];
static uint8_t sbuf_idx = 0;

static UASART_line_rcv_t m_line_rcv;

void USART_PutChar(uint8_t data)
{
    // Чекаємо завершення передачі даних
    while (!(UCSR0A & (1 << UDRE0)))
    {
        ;
    }
    // Почати передачу даних
    UDR0 = data;
}

uint8_t USART_Receive(void)
{

    while (!(UCSR0A & (1 << RXC0)))
    {
        ;
    }
    // Повертаємо прийняті дані
    return UDR0;
}

static int uart_putch(char ch, FILE *stream)
{
    if (ch == '\n')
        USART_PutChar('\r');
    USART_PutChar(ch);
    return 0;
}

static int uart_getch(FILE *stream)
{
    uint8_t ch;
    ch = USART_Receive();
    USART_PutChar(ch);
    return ch;
}

static FILE uart_stream = FDEV_SETUP_STREAM(uart_putch, uart_getch, _FDEV_SETUP_RW);

void USART_Init(uint32_t baud)
{
    // Розрахунок швидкості(U2X = 1)
    UBRR0H = (uint8_t)((F_CPU / (8 * baud) - 1) >> 8);
    UBRR0L = (uint8_t)(F_CPU / (8 * baud) - 1);
    UCSR0A = 1 << U2X0;
    // Включити передавач та приймач
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // Налаштування фрейму передачі/прийому даних
    // 8-біт даних, 1-стоп біт, без перевірки парності
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    // Define Output/Input Stream
    stdout = stdin = &uart_stream;
}

void USART_set_callback(UASART_line_rcv_t clbk)
{
    m_line_rcv = clbk;
}

void USART_poll(void)
{
    uint8_t ch;

    if (UCSR0A & (1 << RXC0))
    {
        ch = UDR0;
        USART_PutChar(ch);

        if (ch == '\r' || ch == '\n')
        {
            // end of line
            sbuf[sbuf_idx] = 0;
            if (m_line_rcv != ((void *)0))
            {
                m_line_rcv(sbuf);
            }
            sbuf_idx = 0;
            printf("$ ");
        }
        else
        {
            if (sbuf_idx < sizeof(sbuf) - 1)
            {
                sbuf[sbuf_idx++] = ch;
            }
            else
            {
                printf("\r\nError: command too long!\r\n");
                sbuf_idx = 0;
            }
        }
    }
}

void printlog(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}