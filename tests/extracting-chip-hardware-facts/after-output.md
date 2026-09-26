# With-Skill Evaluation Output

## Variant A: schematic missing

## 任务与硬件身份

目标是 MT32X-LQFP64 上的 USART3 DMA 半双工 RS485。MCU 型号和封装已给出；芯片修订版未知。板卡版本、U5 的完整型号和实际接线均未提供。

## 工程事实

| 问题 | 结论 | 状态 | 来源及影响 |
|---|---|---|---|
| USART3 引脚候选 | PA2：AF8 TX，PA3：AF8 RX；PC10：AF7 TX，PC11：AF7 RX；四个引脚都在 LQFP64 封装上 | `CONFIRMED` | `MT32X-LQFP64 Datasheet, Rev. C` §2.1 表 4。资料没有说明哪组已接到 U5。 |
| 引脚电气限制 | PA2/PA3 为 5 V tolerant；PC10/PC11 不耐 5 V，输入不得超过 3.6 V | `CONFIRMED` | 同一 Datasheet §5.3 表 20。U5 的 RO 输出电压和逻辑电源未知，因此尚不能据此确认接口兼容性。 |
| USART3 时钟 | 使用 APB1 kernel clock；启用 USART3 的 `UE` 前须使能 GPIO 和 APB1 时钟 | `CONFIRMED` | `MT32X Reference Manual, Rev. 1.6` §17.4。具体时钟频率未提供，波特率配置值待定。 |
| DMA 映射与请求 | RX 可用 DMA1 Channel 4 Stream 1 或 Stream 3；TX 可用 Channel 4 Stream 3 或 Stream 4。相应设置 `USART3_CR3.DMAR` 或 `DMAT` | `CONFIRMED` | 同一 Reference Manual §11.2 表 11-7、§17.7。Stream 3 同时出现在 RX/TX 选项中，不能把同一个 DMA stream 同时分配给两边。 |
| DMA 与串口发送完成 | DMA transfer-complete 只表示内存传输结束；`USART3_SR.TC` 表示最终停止位已发出 | `CONFIRMED` | Reference Manual §17.7。切换 RS485 发送方向前，不能只凭 DMA 完成判定线上发送结束。 |
| 中断 | USART3 全局中断名为 `USART3_IRQn`；DMA 中断对应所选 DMA1 stream | `CONFIRMED` | Reference Manual §17.8。所选 stream 对应的具体 IRQ 符号应与设备头文件核对。 |
| RX DMA 的 IDLE 处理勘误 | `REV_A` 在 IDLE 后、读取 SR 和 DR 前禁用并重新启用 RX DMA stream，可能丢失 DMA 请求；规避顺序是先读 `USART3_SR`，再读 `USART3_DR`，之后再启用 stream。`REV_B` 不受影响 | 适用性 `UNRESOLVED` | `MT32X Errata Sheet, Rev. 1.1` §2.3.4。当前芯片修订版未知，不能把该修订版 workaround 说成所有版本都必须执行。 |

## 板级接线与可用候选

Datasheet 确认了芯片能力，但不能确认 PA2/PA3 或 PC10/PC11 实际连接到 U5。现阶段只能列出两组候选；没有资料支持把其中一组称为板上的“常用”或实际接线。

若只做固件 DMA 资源初选，可以考虑 RX 用 DMA1 Channel 4 Stream 1、TX 用 Channel 4 Stream 4，以免 RX 和 TX 争用 Stream 3。此项是基于手册映射的 `INFERRED` 建议，前提是这两个 stream 未被项目其他功能占用；尚未检查项目配置。

要确认实际接线，请提供原理图局部截图，画面需看清 MCU 的目标引脚和网络名、U5 的完整型号与 DI/RO/DE/RE 引脚、A/B 网络及外部接口网络。如果手头没有原理图，接线结论只能保持 `UNRESOLVED`。U5 的数据手册也需要用于确认逻辑电平、方向控制极性和收发器供电要求。

## 启动检查清单

1. 查明芯片修订版，并从原理图确认实际使用的 USART3 TX/RX 引脚。
2. 对照 U5 数据手册检查 RO 到 MCU RX 的电压限制；尤其是 PC10/PC11 输入不得超过 3.6 V。
3. 确认 U5 的 DE/RE 接法、有效电平及其由哪个 MCU 引脚控制；目前不能假定极性或是否将两者相连。
4. 获取实际 APB1 kernel clock 和通信双方要求的波特率，再配置 USART3；这些数值不能从现有摘录推出。
5. 使能 GPIO 与 APB1 时钟，再启用 USART3 `UE`。为 USART3 配置对应 TX/RX 复用功能。
6. 在 RX/TX DMA stream 中选择不冲突且未被项目占用的映射，并分别设置 `DMAR`/`DMAT`。
7. 若使用中断，启用 `USART3_IRQn` 及所选 DMA1 stream 对应的中断。
8. RS485 发送结束时，先等 DMA 传输完成，再等 `USART3_SR.TC` 确认最后停止位已发出；之后如何切换 DE/RE，需按 U5 实际接线和极性决定。
9. 若实现 IDLE 后重启 RX DMA stream，先确认芯片修订版，再按 §2.3.4 处理 `REV_A` 条件。

波特率、USART 参数、DMA 数据宽度、DE/RE 电平、总线偏置和终端电阻等均未在提供的资料中确定；在拿到项目参数、U5 资料和接线证据前，不应填入猜测值。

## Variant B: schematic region unreadable

## 任务与硬件身份

目标是为虚构的 MT32X-LQFP64 配置 USART3 DMA 半双工 RS485。芯片修订版、板卡版本、U5 完整型号和实际网络连接均未确认。以下结论仅依据题目提供的资料摘录；摘录未提供页码。

## 工程事实

| 项目 | 结论 | 状态与影响 | 来源 |
|---|---|---|---|
| USART3 引脚候选 | PA2 是 AF8 USART3_TX，PA3 是 AF8 USART3_RX；PC10/PC11 也可分别作为 AF7 TX/RX。四个引脚均在 LQFP64 封装上 | `CONFIRMED` 芯片能力；实际是否连到 U5 为 `UNRESOLVED` | Datasheet Rev. C，§2.1 表4 |
| 电气属性 | PA2/PA3 为 5 V tolerant；PC10/PC11 不耐 5 V，输入不得超过 3.6 V | `CONFIRMED`。PA 引脚的耐受说明不能单独证明与 U5 的逻辑电平兼容 | Datasheet Rev. C，§5.3 表20 |
| 时钟与使能 | USART3 使用 APB1 kernel clock；使能 USART3 的 UE 前先使能 GPIO 和 APB1 时钟 | `CONFIRMED`。实际时钟源、频率和波特率计算还需项目时钟配置 | Reference Manual Rev. 1.6，§17.4 |
| DMA 映射 | RX：DMA1 Channel 4、Stream 1 或 3；TX：DMA1 Channel 4、Stream 3 或 4。CR3 中 DMAR/DMAT 分别使能请求 | `CONFIRMED`。可先考虑 RX Stream 1、TX Stream 4，避免两方向共用 Stream 3；这是基于映射的资源分配建议，仍需检查工程中是否已占用 | Reference Manual Rev. 1.6，§11.2 表11-7、§17.7 |
| 发送完成与方向切换 | DMA stream transfer-complete 只表示内存传输结束；USART3_SR.TC 表示最后一个停止位已发出 | `CONFIRMED`。切换收发方向前应依据 USART3_SR.TC，而不能只依据 DMA 完成 | Reference Manual Rev. 1.6，§17.7 |
| 中断 | USART3 全局中断名为 USART3_IRQn；DMA 使用对应 DMA1 stream IRQ 名称 | `CONFIRMED` | Reference Manual Rev. 1.6，§17.8 |
| Errata | REV_A 在 IDLE 事件后，若 DMA stream 尚未读取 USART3_SR 和 USART3_DR 就被重新使能，RX DMA 请求可能丢失；workaround 是先读 SR，再读 DR，再重新使能。REV_B 不受影响 | 修订版未知，适用性为 `UNRESOLVED` | Errata Sheet Rev. 1.1，§2.3.4 |

## 暂定方案与接线清单

可以把 **PA2/PA3（AF8）** 作为 MCU 侧候选对：它们是资料列出的 USART3 引脚，且资料给出的输入耐压限制比 PC10/PC11 宽。但这只是引脚能力选择，**不能据此认定板上已连到 U5**，也不能据此认定 U5 的逻辑电平兼容。

拿到清晰原理图并确认网络后，再核对以下连接：

- USART3_TX → U5 的 DI；U5 的 RO → USART3_RX。
- 若 U5 有 DE、/RE 控制脚，确认它们的逻辑极性、是否连接同一控制信号，以及 MCU 对应 GPIO；这些信息目前未提供。
- 确认 U5 电源、逻辑侧电压、A/B 网络及连接器对应关系。A/B 标注和终端、偏置配置应以 U5 资料及总线拓扑为准。
- Baud rate、数据位、校验位、停止位应从通信协议或对端配置取得；现有摘录没有定义这些值。

## 软件配置顺序

1. 确认原理图实际连接后，再配置对应 GPIO 的复用功能；若确认使用候选对，PA2/PA3 使用 AF8。
2. 使能 GPIO 与 APB1 时钟，按项目时钟树核对 USART3 kernel clock，然后配置与对端匹配的串口参数。
3. 按工程资源情况分配 DMA；可先评估 RX 使用 DMA1 Channel 4 Stream 1、TX 使用 Channel 4 Stream 4。设置 USART3_CR3 的 DMAR/DMAT。
4. 半双工发送后，等待 USART3_SR.TC 再改变 DE/RE 方向；DMA 完成中断本身不代表线路发送结束。
5. 若 RX 流程会在 IDLE 后停流并重启，且芯片修订版尚未查明，应避免在读取状态和数据寄存器前重启 stream；若重启，按摘录所述顺序先读 USART3_SR、再读 USART3_DR、然后重新使能 stream。该条件与 workaround 来自 REV_A 勘误，修订版确认后再判断其适用性。

## 待确认

请提供放大的原理图局部截图，至少能看清 MCU 的 PA2、PA3、PC10、PC11 引脚及网络标号，U5 的 DI、RO、DE、/RE、A、B 引脚、电源，以及连接器侧网络。还需确认芯片修订版和 U5 完整型号；在此之前，板级接线、U5 电气兼容性及 Errata 适用性都无法确认。
