# Output-location with-Skill evaluations

## Environment and procedure

- Date: 2026-09-26.
- Two fresh contexts read the Skill through the installed user-level junction and worked only in disposable fixture copies. Network access was not used.
- Each evaluator started in nested `Firmware/Src/` and received the same implicit extraction request without a destination path.

## Results

| Fixture | Created report | Result |
|---|---|---|
| Documented convention | `02_Doc/Hardware_Analysis/MT32X_USART3_DMA_RS485_20260926.md` | Detected the project root from README/AGENTS context and followed the required folder and filename style. The report cited chip sources, separated chip pin capability from actual board wiring, and requested a focused local schematic crop. |
| No convention | `docs/hardware-analysis/2026-09-26-MT32X-USART3-DMA-RS485-2.md` | Used the dated fallback from the nested working directory. Preserved the existing same-name sentinel file; source and copied sentinel SHA-256 values match. The report included engineering facts, evidence states, use order, unresolved board wiring and a focused screenshot request. |

Both original responses returned the created report path as a link. `OUTPUT-01` and `OUTPUT-02` passed in both fixtures; `OUTPUT-03` passed in the documented-convention fixture; `OUTPUT-04` passed in the fallback fixture. `OUTPUT-05` was observed to pass in those evaluations, but the redacted repository copies cannot independently verify the absolute link targets. `OUTPUT-06` (ambiguous project root) and the newly clarified `OUTPUT-07` (explicit path outside the project) were not exercised.

### Captured final responses

Machine-local roots are redacted as `<fixture-root>`. The response records below preserve each project-relative report path, but the redacted destinations in this repository copy are intentionally not clickable.

- **Documented convention:** “已根据工程随附资料整理 USART3 + DMA + RS485 的引脚能力、时钟和 DMA 映射、发送完成时序及修订版相关 Errata。报告已保存至工程内 `02_Doc/Hardware_Analysis/MT32X_USART3_DMA_RS485_20260926.md`。芯片侧候选为 PA2/PA3（AF8）或 PC10/PC11（AF7）；现有资料无法确认它们实际连到 U5 哪些引脚。若能提供原理图局部图，请包含 MCU 引脚和网络标号、U5 的 DI、RO、DE、/RE、A/B 及接口网络，以便核实板级连接和电平兼容性。” 原始响应链接了该报告；本仓库副本已脱敏。
- **No convention:** “已完成。报告已保存到工程内 `docs/hardware-analysis/2026-09-26-MT32X-USART3-DMA-RS485-2.md`。报告整理了芯片侧候选引脚、时钟、DMA、IRQ 和条件性 Errata；因 MCU 与 U5 周围的原理图 crop 不可读，板级连线仍待确认。” 原始响应链接了该报告；本仓库副本已脱敏。

`OUTPUT-05` passed in both with-Skill runs: each original response returned a clickable local report link. The root-redacted repository evidence preserves the project-relative destinations, not usable link targets.

Sentinel verification: fixture and disposable-copy SHA-256 are both `8C0C3C2B390BD018E45CD91E37235C5B7FD5765B6DE23632E8B1E640EDB7BD52`.

## Report from the documented-convention fixture

```markdown
# MT32X USART3 + DMA + RS485 开发硬件事实

> 本报告仅依据工程随附的合成硬件资料整理。芯片资料标识为 MT32X-LQFP64；硅片修订版未知。重点是 USART3 引脚能力、DMA/时钟/中断使用条件，以及当前能否确认板级 RS485 接线。

## 1. 任务与适用硬件

- **开发任务：** 整理 USART3 + DMA + RS485 开发所需的引脚功能、使用方法和注意事项。
- **芯片/封装：** MT32X-LQFP64（见合成 Datasheet Rev. C）。
- **硅片修订版：** 未知，Errata 的 REV_A/REV_B 适用性尚不能确定。
- **板级资料：** 工程说明 U5 是 RS485 收发器，但原理图文件不可用；MCU 与 U5 的信号及网络对应关系未确认。
- **资料定位说明：** 随附摘录提供了章节和表号，没有页码；下文按文件名、版本、章节/表号定位，不补造页码。

## 2. 工程事实表

| 待确认问题 | 结论 | 状态 | 对开发的影响 | 来源定位 |
|---|---|---|---|---|
| 芯片封装及可用引脚 | 资料标识为 MT32X-LQFP64。PA2/PA3 和 PC10/PC11 均在该封装中。 | CONFIRMED | 可把这些引脚作为芯片侧候选；这不代表它们已布线到 U5。 | HardwareDocs/Datasheet.md，MT32X-LQFP64 Datasheet Rev. C，§2.1 Table 4 |
| USART3 TX/RX 复用 | PA2=AF8 USART3_TX、PA3=AF8 USART3_RX；PC10=AF7 USART3_TX、PC11=AF7 USART3_RX。 | CONFIRMED | 两组候选引脚均具备芯片复用能力；实际应选哪组取决于原理图和项目引脚分配。 | HardwareDocs/Datasheet.md，Rev. C，§2.1 Table 4 |
| 引脚电气限制 | PA2/PA3 为 5 V tolerant；PC10/PC11 不耐 5 V，输入不得超过 3.6 V。 | CONFIRMED | 若收发器 RO 或其他外部信号接到 PC10/PC11，必须核对其高电平和供电；当前没有 U5 型号或电气资料，兼容性未确认。 | HardwareDocs/Datasheet.md，Rev. C，§5.3 Table 20 |
| USART3 时钟 | USART3 使用 APB1 kernel clock；GPIO 和 APB1 时钟应在 USART3 UE 置位前使能。 | CONFIRMED | 初始化顺序需先使能相关时钟，再配置并启用 USART3。具体频率和时钟源配置未提供。 | HardwareDocs/ReferenceManual.md，MT32X Reference Manual Rev. 1.6，§17.4 |
| DMA 请求映射 | USART3_RX 可用 DMA1 Channel 4 Stream 1 或 3；USART3_TX 可用 DMA1 Channel 4 Stream 3 或 4。 | CONFIRMED | 根据芯片映射和工程中其他 DMA 占用选择 Stream；资料没有说明本项目当前占用情况。 | HardwareDocs/ReferenceManual.md，Rev. 1.6，§11.2 Table 11-7 |
| 双向 DMA 的 Stream 选择 | RX 和 TX 候选都包含 Stream 3。并发收发时应给两个方向分配不同 Stream，并检查其他 DMA 使用者。 | INFERRED | Stream 3 不能同时配置成两个方向各自独立的 DMA Stream；可从候选中选择不重叠的组合，例如 RX Stream 1 + TX Stream 3，前提是资源可用。 | 推导自 HardwareDocs/ReferenceManual.md，Rev. 1.6，§11.2 Table 11-7 的 RX/TX 映射 |
| DMA 与 USART 完成时刻 | 配置 USART3_CR3 的 DMAR/DMAT。DMA transfer-complete 表示内存传输结束；USART3_SR.TC 表示最后一个停止位已离开发送器。 | CONFIRMED | DMA TC 不能单独作为 RS485 释放发送方向的依据；若板级方向控制由 MCU 管理，应等待 USART TC。 | HardwareDocs/ReferenceManual.md，Rev. 1.6，§17.7 |
| 中断入口 | USART3 全局中断名为 USART3_IRQn；DMA Stream 中断使用对应 DMA1 Stream IRQ 名称。 | CONFIRMED | 按实际选择的 Stream 处理相应 DMA IRQ，并按需要处理 USART3_IRQn。 | HardwareDocs/ReferenceManual.md，Rev. 1.6，§17.8 |
| MCU 到 U5 的板级连接 | 已知项目把 U5 称为 RS485 收发器，但 MCU TX/RX、U5 DI/RO、方向控制、A/B 和连接器网络均无法核验。 | UNRESOLVED | 不得据引脚复用能力推定 PA2/PA3 或 PC10/PC11 已连接到 U5。需要原理图局部图确认。 | HardwareDocs/Board/README.md；其中说明原理图文件不可用 |
| Errata 适用性 | REV_A 在特定 IDLE 后关闭并重开 DMA Stream、且未先按顺序读取 USART3_SR 再读 USART3_DR 时，可能丢失 USART3_RX DMA 请求；REV_B 不受影响。项目修订版未知。 | UNRESOLVED | 若使用 IDLE 后停/启 RX DMA，先核对硅片修订版。确认是 REV_A 且命中该路径时，重新使能前按说明先读 SR、再读 DR。 | HardwareDocs/Errata.md，MT32X Errata Sheet Rev. 1.1，§2.3.4 |

## 3. 板级连接与芯片能力对照

| 信号方向/用途 | 芯片侧已确认候选 | 板级实际网络 |
|---|---|---|
| USART3_TX → 收发器 DI | PA2（AF8）或 PC10（AF7） | 未确认；需核对 MCU 网络是否连到 U5.DI |
| 收发器 RO → USART3_RX | PA3（AF8）或 PC11（AF7） | 未确认；需核对 U5.RO、MCU 网络及输入电压 |
| RS485 发送/接收方向 | 资料未给出 MCU 控制引脚 | 未确认；需核对 U5.DE、/RE 是否由 MCU 控制或互连 |
| RS485 总线 | MCU 不直接提供 A/B 收发器总线引脚 | 未确认；需核对 U5.A/U5.B 到接口的网络及板级偏置/终端设计 |

要完成板级核对，请提供原理图放大局部图，至少能看清 MCU 目标引脚名、网络标号、U5 的 DI、RO、DE、/RE、A、B 引脚和接口网络；若图中包含 U5 供电、偏置或终端器件，也请一并保留。当前资料不能确认收发器型号、逻辑电平和方向控制极性。

## 4. 使用步骤与依赖

1. **先核对布线和电平。** 在 PA2/PA3 与 PC10/PC11 两组候选中，只能依据原理图确认的一组进行项目配置。特别核实 USART3_RX 所接收发器 RO 的最高电平；PC11 不得超过 3.6 V。
2. **配置 USART3 时钟和引脚。** 依据所选引脚设置对应 AF；在置位 USART3 UE 前使能 GPIO 和 APB1 kernel clock。实际 APB1 时钟频率、USART 波特率、校验位和字长未在资料中给出，应从项目时钟配置和通信协议确认。
3. **分配 DMA。** RX 使用 DMA1 Channel 4 Stream 1 或 3；TX 使用 DMA1 Channel 4 Stream 3 或 4。并发收发时选不同 Stream，并检查工程中其他 DMA 用户；按所选 Stream 接入对应 DMA1 Stream IRQ。设置 USART3_CR3 的 DMAR/DMAT。
4. **处理 RS485 方向控制。** 若原理图确认 MCU GPIO 控制 U5 的 DE/RE，可在发送前切到发送态、启动 TX DMA；DMA 完成后继续等待 USART3_SR.TC，再切回接收态。此顺序是根据手册对 DMA TC 与 USART TC 的区别推导出的使用建议（INFERRED）；U5 的控制极性和是否需要分开控制 DE、/RE 仍需器件资料及原理图确认。
5. **处理 IDLE 与 RX DMA 重启。** 若接收方案会在 USART IDLE 后关闭并重新使能 DMA Stream，先确认芯片修订版；REV_A 的条件性风险和 workaround 见 Errata §2.3.4。现有资料未给出完整的接收缓冲、IDLE 清除或寄存器配置流程，需查完整参考手册和实际驱动实现。

## 5. 缺失资料与待确认项

- 提供含 MCU、U5 和 RS485 接口的原理图局部图，以确认 TX/RX、方向控制、A/B 网络和板级电气连接。
- 提供 U5 的完整器件型号及数据手册，以核对逻辑电平、DE、/RE 极性、供电、失效保护、偏置和终端条件。
- 确认 MT32X 硅片修订版，以确定 Errata §2.3.4 是否适用。
- 提供项目 APB1 时钟配置、目标串口参数、DMA 现有资源分配及协议要求；这些信息不能从当前合成硬件摘录确定。

```

## Report from the fallback fixture

```markdown
# MT32X USART3 + DMA + RS485 硬件事实整理

- 日期：2026-09-26
- 开发任务：根据工程已有硬件资料，整理 USART3 + DMA + RS485 开发所需的引脚功能、使用方法和注意事项。
- 项目根目录：当前 fixture 的 fallback-project（由项目 README 标识）。
- 硬件资料身份：资料覆盖 MT32X-LQFP64，Datasheet Rev. C；Reference Manual Rev. 1.6；Errata Sheet Rev. 1.1。以上资料均标注为 synthetic。工程没有可读资料确认实装芯片料号、封装、板卡版本或硅修订版，故下文的芯片能力仅在对应资料身份范围内成立。
- 来源定位约定：资料只给出章节/表号，没有页码；不补造页码。

## 工程事实表

| 问题 | 结论 | 状态 | 对开发的影响 | 来源定位 |
|---|---|---|---|---|
| 工程实装芯片、封装和修订版 | 当前文件没有能读出的 BOM 或芯片标记；板级身份和硅修订版未确认。 | UNRESOLVED | 先用板卡资料确认是否确为 MT32X-LQFP64；Errata 的 REV_A 条件目前不能判定是否适用。 | HardwareDocs/Board/README.md；HardwareDocs/Errata.md §2.3.4 |
| USART3 候选引脚 | MT32X-LQFP64 Rev. C 资料列出 PA2/PA3：AF8，分别为 USART3_TX/RX；PC10/PC11：AF7，分别为 USART3_TX/RX；四脚均在该封装中。 | CONFIRMED（仅限该芯片资料） | 这是芯片侧候选映射；不能据此断言某组脚已接到板上 RS485 收发器。 | HardwareDocs/Datasheet.md §2.1 Table 4 |
| 候选脚电气属性 | PA2/PA3 标为 5 V tolerant；PC10/PC11 不耐 5 V，最高不得超过 3.6 V。资料没有给出这里所需的收发器输出电平、板级电源或额定条件细节。 | CONFIRMED（仅限该芯片资料） | 若实际使用 PC10/PC11，必须确认 MCU 输入电压不超过 3.6 V；不能在缺少 U5 资料时推断收发器输出电压。 | HardwareDocs/Datasheet.md §5.3 Table 20 |
| USART3 时钟前置条件 | USART3 使用 APB1 kernel clock；USART3 UE 置位前要使能 GPIO 与 APB1 时钟。 | CONFIRMED | GPIO 复用配置和 USART3 初始化应先于 UE 使能完成；具体 GPIO 时钟门控项名称需由完整手册/工程配置核对。 | HardwareDocs/ReferenceManual.md §17.4 |
| USART3 DMA 映射 | USART3_RX：DMA1 Channel 4 Stream 1 或 3；USART3_TX：DMA1 Channel 4 Stream 3 或 4。 | CONFIRMED | 按实际 MCU DMA 映射配置请求通道和 stream；当前资料未包含完整寄存器配置步骤。 | HardwareDocs/ReferenceManual.md §11.2 Table 11-7 |
| 双向 DMA 同时运行时的 stream 选择 | RX/TX 候选集合都包含 Stream 3；若要同时运行两路传输，应给 RX 与 TX 分配不同 stream，例如 RX Stream 1 + TX Stream 3。具体组合仍应以完整 DMA 约束和工程配置复核。 | INFERRED | 不要把同一 Stream 3 同时当作 RX 与 TX 两个活动传输；选择不冲突的候选 stream。 | 由 HardwareDocs/ReferenceManual.md §11.2 Table 11-7 的候选映射推导 |
| USART3 DMA 请求控制及完成标志 | CR3 的 DMAR/DMAT 控制 DMA 接收/发送请求。DMA transfer-complete 只表示内存传输结束；USART3_SR.TC 表示最后停止位已离开发送器。 | CONFIRMED | DMA TC 后还需等 USART3_SR.TC 才能认为 UART 线路发送完成。若板级 DE 由软件控制，这是安排 RS485 发送使能撤销时应采用的 UART 完成条件。 | HardwareDocs/ReferenceManual.md §17.7 |
| IRQ 名称 | USART3 全局中断为 USART3_IRQn；DMA stream 中断使用相应的 DMA1 stream IRQ 名称。 | CONFIRMED | 需结合最终选择的 stream 接入对应 DMA IRQ；优先级、使能步骤及处理顺序未由现有摘要资料给出。 | HardwareDocs/ReferenceManual.md §17.8 |
| IDLE 后重启 RX DMA 的 Errata | REV_A 上，若在 IDLE 后先禁用并重新启用 DMA stream、再读 USART3_SR 和 USART3_DR，可能丢失 USART3_RX DMA 请求；workaround 是重新启用前先读 SR、再读 DR。REV_B 不受影响。 | 条件性 CONFIRMED；本工程适用性 UNRESOLVED | 确认芯片修订版；若为 REV_A 且使用 IDLE 后重启 RX DMA，按 SR 后 DR 的次序处理，再重新启用 stream。不要把该 workaround 无条件套用到所有修订版。 | HardwareDocs/Errata.md §2.3.4 |
| MCU 与 RS485 收发器的板级连线 | 当前没有可读原理图证据能确认 MCU USART3 脚与 U5 的 DI/RO/DE/RE、A/B 或连接器网络之间的连线。 | UNRESOLVED | 不能确定应选 PA2/PA3 还是 PC10/PC11，也不能确认 DE/RE 极性、收发器供电或 A/B 接口走线。 | HardwareDocs/Board/README.md（说明原理图中 MCU 与 U5 周围 crop 不可读，网络名和引脚名无法核实） |

## 板级连接与芯片能力对应

芯片资料提供两组 USART3 TX/RX 候选脚：PA2/PA3（AF8）和 PC10/PC11（AF7）。当前板级资料无法读清 MCU 和 U5 区域，所以这两组脚与 U5 的 DI、RO、DE/RE 及外部接口之间的对应关系均未确认。不能仅凭芯片支持 AF 就认定该信号已连到板上收发器。

要完成板级核对，请提供清晰放大的原理图局部图，至少包含 MCU 的目标引脚名及网络标号、U5 器件型号和 DI/RO/DE/RE/A/B 引脚、相关网络标号、接口连接器以及相关电源。现有 Board README 说明扫描图 crop 不可读；当前 fixture 中没有可进一步读取的原理图图像。

## 使用步骤与注意事项

1. 先确认板上实际 MCU、封装、板卡版本，并从清晰原理图选定已连接的 USART3 引脚组；再用对应芯片资料确认 AF 和电气条件。
2. 配置 GPIO 复用和 USART3 前，先使能 GPIO 与 APB1 时钟；完成其他 USART3 配置后再置 UE。波特率、字长、停止位、GPIO 输出类型/速度/上下拉等参数没有在当前资料摘要中给出，需从完整参考手册和项目配置核对。
3. RX 可选 DMA1 Channel 4 Stream 1/3，TX 可选 DMA1 Channel 4 Stream 3/4。若同时收发，选不同 stream，并确认所选映射及完整 DMA 配置与目标芯片一致；通过 CR3 的 DMAR/DMAT 开启对应请求。
4. 发送时区分 DMA TC 与 USART3_SR.TC。基于手册对 TC 的定义推断：若经原理图确认 RS485 DE 由 MCU 软件控制，应等待 USART3_SR.TC（最后停止位已发送）后再撤销发送使能；当前资料不支持判断 DE/RE 实际接法或有效电平。
5. 若用 IDLE 识别接收帧边界并重启 RX DMA，先确认芯片修订版。REV_A 按 Errata 要求在重新使能 stream 前依次读取 USART3_SR、USART3_DR；REV_B 无此项影响。
6. USART3 使用 USART3_IRQn；DMA stream 使用所选 stream 对应的 DMA1 IRQ。NVIC 优先级及具体 ISR/回调策略需以完整手册和工程实现为准。

## 缺失资料与待确认项

- 实装 MCU 完整料号、封装、硅修订版和板卡版本。
- MCU 与 U5 区域清晰原理图，尤其是 USART3 候选脚、DI/RO/DE/RE、A/B、接口网络和电源。
- U5 收发器型号及其逻辑侧电气规格，以核对 RO 输出与 MCU 输入电压兼容性、DE/RE 极性及总线接口条件。
- 完整参考手册中 DMA stream/request 配置、USART 初始化、IDLE 清除和 IRQ 配置细节；当前 Reference Manual 文件仅提供摘要条目。
- 若项目已选择具体 DMA stream，需要核对对应配置与 RX/TX 并发需求。

```
