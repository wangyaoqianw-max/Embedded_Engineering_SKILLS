#ifndef DMA_H
#define DMA_H

#include <stddef.h>

typedef void (*dma_rx_done_callback_t)(size_t transferred);

void dma_set_rx_done_callback(dma_rx_done_callback_t callback);
void dma_start_rx(void *buffer, size_t capacity);
void DMA1_Channel5_IRQHandler(void);

#endif
