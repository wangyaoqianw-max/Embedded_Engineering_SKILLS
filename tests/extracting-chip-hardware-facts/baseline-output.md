# No-Skill Baseline Output

## Variant A: schematic missing

可先选 **PA2/PA3** 作为 USART3 的候选引脚：PA2 是 AF8 USART3_TX，PA3 是 AF8 USART3_RX，二者都在 LQFP64 封装上，且资料标注为 5 V tolerant。PC10/PC11 也是有效组合（AF7），但都不得超过 3.6 V。PA 的耐压标注不能代替对 U5 逻辑电压和引脚工作条件的核对。

### 初始化与 DMA 清单

- 按 PA2/PA3 配置 GPIO 复用功能 AF8；使用 **APB1 kernel clock**。先使能 GPIOA 和 APB1 时钟，再启用 USART3 的 `UE`。
- 可选一组不冲突的 DMA 映射：RX 用 DMA1 Channel 4 Stream 1，TX 用 DMA1 Channel 4 Stream 4。RX 配为外设到内存，TX 配为内存到外设；分别在 `USART3_CR3` 设置 `DMAR` 和 `DMAT`。
- 若处理 IDLE 或 USART 错误，使用 `USART3_IRQn`；DMA 使用所选 DMA1 Stream 1 和 Stream 4 对应的 IRQ。
- **DMA 传输完成不代表最后一位已发出。**发送结束、需要释放 RS485 驱动方向时，应等 `USART3_SR.TC`，确认最终停止位已离开发送器。
- 芯片修订未知，接收端按 REV_A 兼容处理：IDLE 后，重新使能 RX DMA stream 前先按顺序读取 `USART3_SR`、再读取 `USART3_DR`。REV_B 不受该问题影响。

### 接线仍需核实

PA2/PA3 只能确定 MCU 侧 USART3 候选脚。资料没有给出 U5 的型号、逻辑侧引脚、使能极性或板上连线，因此不能据此确认完整接线。核对原理图和 U5 数据手册后，再确认 PA2 到 U5 发送数据输入、U5 接收数据输出到 PA3 的连接，以及 DE/RE 控制、U5 电源与逻辑电平、RS485 A/B 端子和公共参考地。

波特率、校验位、DE 极性与时序、终端电阻和偏置也不能从现有资料得出；它们取决于系统时钟、通信要求、U5 规格和总线拓扑，不宜用“常见值”代填。

## Variant B: schematic region unreadable

建议先把 **PA2/PA3（AF8）** 作为 USART3 的固件候选：PA2 为 TX，PA3 为 RX，且两者均为 5 V 容忍脚。模糊的原理图无法确认板上是否真的把它们接到 U5；上板前仍需核对网络连接。PC10/PC11（AF7）也是有效组合，但不耐 5 V，输入不得超过 3.6 V。

**连线核对**

- PA2 / USART3_TX → U5 的发送数据输入；确认 U5 输入电平与 MCU 输出兼容。
- U5 的接收数据输出 → PA3 / USART3_RX。PA3 的 5 V 容忍不代表 U5 的实际输出电压已知，仍需核对。
- 核对 U5 的方向控制信号与 MCU GPIO 的连接、有效电平，以及 DE/RE 是否在板上并接；现有资料没有给出这些信息。
- 核对 U5 的总线端、终端与偏置网络。不要从模糊图中猜接法或电阻值。

**启动配置**

1. 先开 GPIO 与 APB1 时钟，再配置所选引脚复用并使能 USART3 的 `UE`。USART3 使用 APB1 kernel clock。
2. 波特率和帧格式须与总线另一端一致；现有资料没有提供协议参数，不能可靠地代填常用值。
3. DMA 可先选 USART3_RX：DMA1 Channel 4、Stream 1；USART3_TX：DMA1 Channel 4、Stream 4。两路使用不同 stream。
4. 在 USART3_CR3 中按需设置 `DMAR`、`DMAT`；中断分别使用 `USART3_IRQn` 和对应的 DMA1 stream IRQ。
5. 发送完成后，先等 DMA transfer-complete，再等 `USART3_SR.TC` 后释放发送方向控制。DMA 完成只表示内存搬运结束；`TC` 才表示最后一个停止位已发出。
6. 芯片修订未知时，接收 IDLE 处理按 REV_A 规避：重新使能 RX DMA stream 前，先依次读取 `USART3_SR`、`USART3_DR`。REV_B 不受该问题影响。
