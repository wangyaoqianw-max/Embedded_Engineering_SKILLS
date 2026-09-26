# MT32X Reference Manual, Rev. 1.6 (synthetic)

- §17.4: USART3 uses the APB1 kernel clock. Enable GPIO and APB1 clocks before USART3 UE.
- §11.2 Table 11-7: USART3_RX uses DMA1 Channel 4 Stream 1 or 3; USART3_TX uses Channel 4 Stream 3 or 4.
- §17.7: set USART3_CR3 DMAR/DMAT. DMA transfer-complete means the memory transfer ended; USART3_SR.TC means the final stop bit left the transmitter.
- §17.8: USART3 global IRQ is USART3_IRQn; DMA stream interrupts use their corresponding DMA1 stream IRQ names.
