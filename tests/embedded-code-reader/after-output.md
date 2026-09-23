# UART DMA 接收源码阅读笔记

## 阅读范围
- 目标：梳理 fixture 中 UART DMA 接收的初始化、完成回调、通知和重新启动路径。
- 已阅读范围：`tests/embedded-code-reader/fixtures/uart-dma/include/` 下的配置、DMA、任务和 UART 服务头文件，以及 `src/main.c`、`src/dma.c`、`src/uart_service.c`。
- 未覆盖范围：真实芯片的 DMA/UART 寄存器配置、中断向量表、构建配置和 RTOS API 实现；当前 fixture 未提供这些内容。

## 流程概览
1. `main()` 先调用 `uart_service_start()`，再直接调用 `uart_rx_task()`（`tests/embedded-code-reader/fixtures/uart-dma/src/main.c:4-8`）。
2. 服务将状态设为 `UART_RECEIVING`，注册 `uart_dma_rx_done` 回调，并用 64 字节静态缓冲区启动 DMA 接收（`tests/embedded-code-reader/fixtures/uart-dma/src/uart_service.c:12-13,27-32`）。
3. `DMA1_Channel5_IRQHandler()` 在回调指针非空时，将 `pending_size` 传给已注册回调（`tests/embedded-code-reader/fixtures/uart-dma/src/dma.c:17-22`）。
4. 回调检查长度：超出缓冲区则进入 `UART_ERROR` 并返回；否则进入 `UART_DATA_READY`，通过 `xTaskNotifyFromISR()` 通知接收任务（`tests/embedded-code-reader/fixtures/uart-dma/src/uart_service.c:16-25`）。
5. 任务调用 `ulTaskNotifyTake()` 后检查状态；若数据就绪，则恢复 `UART_RECEIVING` 并再次启动 DMA（`tests/embedded-code-reader/fixtures/uart-dma/src/uart_service.c:34-43`）。

以上是 fixture 显示的调用关系。DMA 实现没有实际搬运数据，因此不能据此推断硬件接收行为。

## 文件与符号索引

|职责|文件 / 符号|证据|
|---|---|---|
|配置宏|`include/app_config.h` / `UART_RX_BACKEND`|`tests/embedded-code-reader/fixtures/uart-dma/include/app_config.h:4-6`|
|启动入口|`src/main.c` / `main`|`tests/embedded-code-reader/fixtures/uart-dma/src/main.c:4-8`|
|服务状态、缓冲区和回调|`src/uart_service.c` / `state`、`rx_buffer`、`uart_dma_rx_done`|`tests/embedded-code-reader/fixtures/uart-dma/src/uart_service.c:5-25`|
|DMA 接口及回调类型|`include/dma.h` / `dma_rx_done_callback_t`、`dma_start_rx`、`DMA1_Channel5_IRQHandler`|`tests/embedded-code-reader/fixtures/uart-dma/include/dma.h:4-10`|
|DMA 回调注册和中断分发|`src/dma.c` / `dma_set_rx_done_callback`、`DMA1_Channel5_IRQHandler`|`tests/embedded-code-reader/fixtures/uart-dma/src/dma.c:3-9,17-22`|
|通知接口声明|`include/task_api.h` / `xTaskNotifyFromISR`、`ulTaskNotifyTake`|`tests/embedded-code-reader/fixtures/uart-dma/include/task_api.h:4-6`|

## 配置与初始化

`app_config.h` 定义了轮询和 DMA 两个后端值，并将 `UART_RX_BACKEND` 设为 DMA（`tests/embedded-code-reader/fixtures/uart-dma/include/app_config.h:4-6`）。`main.c` 包含该头文件，但 fixture 中没有代码读取 `UART_RX_BACKEND` 或用它进行条件编译；`main()` 无条件调用 UART 服务。因此，这个宏表达了配置值，不能单独证明它控制了实际构建或后端选择（`tests/embedded-code-reader/fixtures/uart-dma/src/main.c:1-8`）。

`uart_service_start()` 先设置状态，再注册完成回调，最后把 `rx_buffer` 和其容量传给 `dma_start_rx()`（`tests/embedded-code-reader/fixtures/uart-dma/src/uart_service.c:27-32`）。DMA 实现只忽略缓冲区指针，并把容量存入 `pending_size`，没有配置外设或搬运字节（`tests/embedded-code-reader/fixtures/uart-dma/src/dma.c:11-15`）。

## 运行时调用链、数据流与状态

- **初始化直调：** `main()` → `uart_service_start()` → `dma_set_rx_done_callback(uart_dma_rx_done)` → `dma_start_rx(rx_buffer, sizeof(rx_buffer))`。回调注册发生在启动接收之前（`tests/embedded-code-reader/fixtures/uart-dma/src/main.c:4-8`、`tests/embedded-code-reader/fixtures/uart-dma/src/uart_service.c:27-32`）。
- **中断到回调：** `DMA1_Channel5_IRQHandler()` 检查回调指针后通过函数指针调用它；注册点和分发点分别见 `tests/embedded-code-reader/fixtures/uart-dma/src/uart_service.c:27-32`、`tests/embedded-code-reader/fixtures/uart-dma/src/dma.c:17-22`。fixture 没有展示是谁或什么硬件条件触发该中断。
- **长度与通知：** 在当前 stub 中，`dma_start_rx()` 将容量存为 `pending_size`，中断处理程序又将其作为 `transferred` 传入回调。服务以 64 字节作为缓冲区容量，所以按这段实现走通时回调收到的是 64，而不是从实际 UART/DMA 状态读取的字节数（`tests/embedded-code-reader/fixtures/uart-dma/src/uart_service.c:13,31`、`tests/embedded-code-reader/fixtures/uart-dma/src/dma.c:11-21`）。
- **数据消费：** `rx_buffer` 被传给 DMA 接口，但 stub 丢弃了指针；任务只检查状态并重新启动接收，没有读取或解析缓冲区内容（`tests/embedded-code-reader/fixtures/uart-dma/src/dma.c:11-15`、`tests/embedded-code-reader/fixtures/uart-dma/src/uart_service.c:34-42`）。
- **状态变化：** 初值为 `UART_IDLE`；服务启动后为 `UART_RECEIVING`；长度不超容量时回调改为 `UART_DATA_READY`，任务处理通知后改回 `UART_RECEIVING`；长度超容量时回调改为 `UART_ERROR` 并直接返回（`tests/embedded-code-reader/fixtures/uart-dma/src/uart_service.c:5-12,16-24,27-32,34-42`）。错误分支没有通知任务；任务也没有处理或清除 `UART_ERROR` 的分支（同文件 `:16-25,34-43`）。
- **任务关系：** `main()` 直接调用一个无限循环的 `uart_rx_task()`，没有展示任务创建过程（`tests/embedded-code-reader/fixtures/uart-dma/src/main.c:4-8`、`tests/embedded-code-reader/fixtures/uart-dma/src/uart_service.c:34-43`）。`rx_task_handle` 在可读代码中没有显式赋值，通知目标如何关联到真实任务未展示（同文件 `:14,24`）。

## 模块协作
UART 服务持有缓冲区和状态，并通过 DMA 接口提交接收请求、注册完成回调；DMA 层保存回调指针并在 IRQ handler 中分发；UART 回调通过任务通知 API 将完成事件传给接收循环（`tests/embedded-code-reader/fixtures/uart-dma/src/uart_service.c:13-24,27-42`、`tests/embedded-code-reader/fixtures/uart-dma/src/dma.c:3-22`）。`state` 会在回调与任务代码中读写，但 fixture 没有展示并发同步或内存可见性约定。

## 未确认事项
- DMA/UART 外设是否实际启动、如何填充缓冲区、传输长度如何取得，以及 IRQ 如何连接到向量表，均未由 fixture 展示。
- `ulTaskNotifyTake()` 是否阻塞、如何清除通知值，以及 `xTaskNotifyFromISR()` 的调度语义无法从头文件声明确认。
- `uart_rx_task()` 在该 fixture 中由 `main()` 直接调用；实际 RTOS 任务创建与 `rx_task_handle` 初始化方式未展示。
- 缓冲区没有被消费或解析；该 fixture 不能说明完整 UART 协议处理流程。
- 超长传输进入 `UART_ERROR` 后没有恢复路径；错误后的系统行为无法从当前代码确认。

## 建议阅读顺序
1. `tests/embedded-code-reader/fixtures/uart-dma/src/main.c` — 查看程序入口及服务/任务调用顺序。
2. `tests/embedded-code-reader/fixtures/uart-dma/src/uart_service.c` — 查看接收状态、回调、通知和重新启动逻辑。
3. `tests/embedded-code-reader/fixtures/uart-dma/src/dma.c` — 核对 DMA stub 的启动行为及 IRQ 到回调的分发。
4. `tests/embedded-code-reader/fixtures/uart-dma/include/app_config.h`、`include/dma.h`、`include/task_api.h` — 对照宏、DMA 和任务通知接口声明。