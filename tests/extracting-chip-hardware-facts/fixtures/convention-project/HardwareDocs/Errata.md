# MT32X Errata Sheet, Rev. 1.1 (synthetic)

- §2.3.4: On silicon REV_A, USART3_RX DMA request may be lost if the DMA stream is disabled and re-enabled before reading USART3_SR then USART3_DR after IDLE. Workaround: read SR and then DR before re-enabling. REV_B is not affected. The project silicon revision is unknown.
