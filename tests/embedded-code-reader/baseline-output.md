# UART DMA 接收源码阅读笔记

## 流程概览

1. `src/main.c` 先调用 `uart_service_start()`，随后调用 `uart_rx_task(0)`。接收任务进入无限循环，因此正常情况下会持续等待 DMA 接收通知。
2. `src/uart_service.c` 的 `uart_service_start()` 将状态设为 `UART_RECEIVING`，注册完成回调 `uart_dma_rx_done()`，再把静态缓冲区 `rx_buffer`（64 字节）和容量交给 `dma_start_rx()`。
3. DMA 完成中断入口 `DMA1_Channel5_IRQHandler()`（`src/dma.c`）检查回调是否已注册；已注册时，以本次记录的长度调用回调。
4. `uart_dma_rx_done()` 检查传输长度。长度大于缓冲区容量时，状态设为 `UART_ERROR` 并直接返回；否则设为 `UART_DATA_READY`，通过 `xTaskNotifyFromISR()` 通知接收任务，并把传输长度作为通知值传入。
5. `uart_rx_task()` 等待 `ulTaskNotifyTake()`。被唤醒且状态为 `UART_DATA_READY` 时，它把状态恢复为 `UART_RECEIVING`，并用同一缓冲区重新启动一次 64 字节 DMA 接收。

因此，代码呈现的是“启动单缓冲区 DMA → 中断回调通知任务 → 任务重新挂起 DMA”的循环。没有看到接收数据解析或把数据交给其他模块的步骤。

## 代码边界

- `include/app_config.h` 将 `UART_RX_BACKEND` 配为 DMA，但当前 `src/uart_service.c` 没有使用这些宏做后端分支；轮询接收路径在这组源码中不可见。
- 当前 `src/dma.c` 是简化实现：`dma_start_rx()` 不操作硬件或填充缓冲区，只保存容量；中断处理函数把这个容量作为传输长度回调。因此这里不能据此确认真实 DMA 的配置、实际收到的字节数、UART 空闲/超时判帧或缓存一致性处理。
- 长度超出 64 字节时，回调只设置 `UART_ERROR`，不通知任务；任务侧也没有看到错误恢复逻辑。正常完成路径传入的通知长度也没有在任务中读取或用于处理数据。
