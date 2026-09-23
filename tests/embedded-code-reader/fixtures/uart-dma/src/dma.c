#include "dma.h"

static dma_rx_done_callback_t rx_done_callback;
static size_t pending_size;

void dma_set_rx_done_callback(dma_rx_done_callback_t callback)
{
    rx_done_callback = callback;
}

void dma_start_rx(void *buffer, size_t capacity)
{
    (void)buffer;
    pending_size = capacity;
}

void DMA1_Channel5_IRQHandler(void)
{
    if (rx_done_callback != 0) {
        rx_done_callback(pending_size);
    }
}
