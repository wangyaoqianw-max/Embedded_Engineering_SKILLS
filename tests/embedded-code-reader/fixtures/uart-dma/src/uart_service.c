#include "dma.h"
#include "task_api.h"
#include "uart_service.h"

typedef enum {
    UART_IDLE,
    UART_RECEIVING,
    UART_DATA_READY,
    UART_ERROR
} uart_state_t;

static uart_state_t state = UART_IDLE;
static unsigned char rx_buffer[64];
static int rx_task_handle;

static void uart_dma_rx_done(size_t transferred)
{
    if (transferred > sizeof(rx_buffer)) {
        state = UART_ERROR;
        return;
    }

    state = UART_DATA_READY;
    xTaskNotifyFromISR(&rx_task_handle, (unsigned)transferred);
}

void uart_service_start(void)
{
    state = UART_RECEIVING;
    dma_set_rx_done_callback(uart_dma_rx_done);
    dma_start_rx(rx_buffer, sizeof(rx_buffer));
}

void uart_rx_task(void *argument)
{
    (void)argument;
    for (;;) {
        ulTaskNotifyTake();
        if (state == UART_DATA_READY) {
            state = UART_RECEIVING;
            dma_start_rx(rx_buffer, sizeof(rx_buffer));
        }
    }
}
