# Conflicting board-source no-Skill baseline

## Environment and procedure

- Date: 2026-09-26.
- A separate no-Skill evaluator worked in a disposable copy from nested `Firmware/Src/`, read only the synthetic fixture, and did not use the network.
- Prompt: “For this board's USART3_RX, should I configure PA3 or PC11? Please organize the evidence and explain what we need to check before implementation.”

## Captured response

目前不能仅凭现有资料确定应配置 PA3 还是 PC11，因为两份 Rev. C 板级资料对同一条 `U5_RO` 网络给出了相互冲突的连接：

| 证据 | `U5_RO` 连接 | 文件声称的版本 |
|---|---|---|
| `HardwareDocs/Board/Schematic_RevC.md` | MCU PA3 | 当前 Rev. C 原理图 |
| `HardwareDocs/Board/Assembly_Netlist_RevC.md` | MCU PC11 | 当前 Rev. C 装配网表 |

芯片数据手册显示，PA3 可设为 AF8 `USART3_RX`，PC11 可设为 AF7 `USART3_RX`，两者都具备该复用功能。因此芯片能力无法解决板级资料的冲突。若最终确认装配网表对应实际板卡，应选 PC11；若确认原理图对应实际板卡，应选 PA3。

实现前请先确认哪份资料对应这块板的已发布 Rev. C 构建，并通过原始 ECAD/netlist 或板上连通性确认 U5.RO 实际接到哪个 MCU 引脚。还要核对所用 MCU 型号和封装、对应引脚的 AF 配置，以及 U5_RO 的输出电压与 MCU 输入限制：数据手册称 PA3 为 5 V tolerant，而 PC11 不耐 5 V、不得超过 3.6 V。资料冲突解决并确认电气兼容后，再确定 USART3_RX 引脚配置。

## Baseline result

- It noticed that the board documents disagree and kept both mappings visible.
- It cited both board files and separated chip alternate-function capability from board wiring.
- It described the electrical-risk difference and recommended checking the released build and actual board.
- It conditionally recommended one pin depending on which document proves authoritative; it did not label the board mapping `CONFLICT` or explicitly say neither pin can yet be selected.
- It did not ask for a focused screenshot as a fallback if the authoritative source cannot be supplied.
- No Markdown report was created in the disposable project.

## Execution boundary

Only the fixture project files were inspected; no Skill, parent repository, or network was used. The evaluator also recorded its inspection commands in its response; those commands are omitted here because the captured answer and observed file outcome are the baseline evidence.
