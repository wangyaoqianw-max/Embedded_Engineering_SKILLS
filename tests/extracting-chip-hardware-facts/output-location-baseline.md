# Output-location no-Skill baseline

## Environment and procedure

- Date: 2026-09-26
- Two separate fresh evaluator contexts; neither was given the Skill or regression rubric, asked not to inspect the parent repository, and restricted to its own disposable fixture copy. No network access was used.
- Both requests were phrased as a natural hardware-document organization task and did not specify an output path.
- Current directory for each evaluation was set to the nested `Firmware/Src/` folder.
- Fixture source: `output-location-scenario.md` and the matching project layout under `fixtures/`.

## Results

| Fixture | Observed result | Criteria |
|---|---|---|
| Documented convention | Created a non-empty Markdown report in `02_Doc/Hardware_Analysis/`, followed the existing `<chip>_<feature>_<YYYYMMDD>.md` name format, and returned a link. The report kept the actual board mapping unresolved and cited source sections. | `OUTPUT-01`, `OUTPUT-02`, `OUTPUT-03`, `OUTPUT-05` pass for this fixture. |
| No convention + collision | Returned a technically useful answer in chat but created no Markdown report. It therefore did not select the fallback folder/name or demonstrate collision handling; the pre-existing sentinel stayed unchanged. | `OUTPUT-01` fails. `OUTPUT-04` and `OUTPUT-05` fail. Fallback root selection and collision behavior are not exercised. |

The meaningful gap is that the no-Skill behavior depends on project-specific instructions: it saved a report when the project had explicit guidance, but answered inline without an artifact when the project had no convention. The Skill must make Markdown persistence the default, detect the project root from the nested working directory, and apply the standard fallback safely.

## Evaluator responses (local path redacted)

In the response below, the machine-local absolute prefix and Markdown link target are redacted to keep the repository portable. The project-relative report path and response content are retained; this repository copy is intentionally not clickable.

### Documented-convention fixture response

> ### 给用户的回复
>
> 已整理 USART3 引脚候选、DMA 映射、时钟与中断要点，以及 RS485 收发切换和 REV_A 勘误注意事项，并保存到工程内 `02_Doc/Hardware_Analysis/MT32X_USART3_DMA_RS485_20260926.md`。（原始回复提供的本地文件链接已脱敏；本副本仅保留相对路径。）
>
> 现有资料无法确认板上实际选用哪组 USART3 引脚，也没有 U5 的 TX/RX、DE/RE 接线图；报告将这些列为待确认项。
>
> ### 报告文件
>
> 已创建，路径见上。
>
> ### 执行的命令
>
> 工作目录设为 fixture 的 `Firmware\Src`。在该 fixture 内执行了目录枚举、文件读取、目标文件存在性检查、写入及回读：
>
> - `Get-ChildItem -Force`
> - `Get-ChildItem -LiteralPath . -Force -Recurse | Select-Object FullName, Length`
> - `Resolve-Path ..\..` 配合 `Get-ChildItem -Force -Recurse`，枚举项目文件
> - `Get-Content -LiteralPath ... -Raw`，读取项目说明和硬件资料
> - `Test-Path -LiteralPath ...`，检查报告目标是否已存在
> - `[System.IO.File]::WriteAllText(...)`，创建报告
> - `Get-Content -LiteralPath ... -Raw`，回读报告核对内容

### No-convention fixture

> ### 给用户的回复
>
> 根据工程现有资料，USART3 可用两组引脚：
>
> | 信号 | 引脚 | 复用功能 | 电气限制 |
> |---|---|---|---|
> | TX / RX | PA2 / PA3 | AF8 | 5 V 容忍 |
> | TX / RX | PC10 / PC11 | AF7 | 不耐 5 V，电压不得超过 3.6 V |
>
> 四个引脚均存在于 LQFP64 封装。实际选哪一组还不能从现有板级资料确认：工程说明原理图中 MCU 与 U5（RS485 收发器）附近的网络标号无法辨认，因此无法确认 USART3 实际连接到哪组引脚，也无法确认 DE、/RE 的 GPIO 与有效电平。不要把上述引脚复用表当作板上连线已核实。
>
> 开发时可按以下顺序处理：
>
> 1. 配置选定引脚的 GPIO 复用功能；先使能 GPIO 和 APB1 时钟，再使能 USART3 的 UE。USART3 使用 APB1 kernel clock。
> 2. RX 可选 DMA1 Channel 4 Stream 1 或 3；TX 可选 Channel 4 Stream 3 或 4。收发不能占用同一 DMA stream，可选 RX Stream 1 + TX Stream 3，或 RX Stream 3 + TX Stream 4；还要检查这些 stream 是否被工程中的其他 DMA 功能占用。
> 3. 在 USART3_CR3 中按方向使能 DMAR 和 DMAT，并配置对应 DMA stream。USART3 使用 `USART3_IRQn`；DMA 中断使用对应的 DMA1 stream IRQ。
> 4. RS485 发送方向不能只依据 DMA 传输完成来切换：DMA 完成表示内存搬运结束，USART3_SR.TC 才表示最后一个停止位已离开发送器。若用 GPIO 控制收发方向，应等 USART3 TC 后再撤销发送使能，以免截断尾部数据。DE、/RE 的具体引脚和极性需以可读的原理图及收发器资料确认。
>
> 另有一项芯片勘误需要结合硅版本处理：REV_A 在 IDLE 后，如果先禁用并重新使能 USART3_RX DMA stream、之后才读取状态和数据寄存器，可能丢失 DMA 请求。勘误给出的规避顺序是重新使能 stream 前先读 USART3_SR，再读 USART3_DR。工程的硅版本未知；REV_B 不受此问题影响。
>
> 来源：`HardwareDocs/Datasheet.md` §2.1、§5.3；`HardwareDocs/ReferenceManual.md` §11.2、§17.4、§17.7、§17.8；`HardwareDocs/Errata.md` §2.3.4；`HardwareDocs/Board/README.md`。
>
> ### 执行记录
>
> - 未创建报告文件，也未修改文件。
> - 命令均在 fixture 的 `Firmware\Src` 目录下运行，且仅读取 fixture 内容：
>   - `Get-Location; Get-ChildItem -Force; Get-ChildItem -Recurse -File | Select-Object -ExpandProperty FullName`
>   - `Get-ChildItem` 枚举 fixture 根目录与文件；`Get-Content` 读取 `README.md`、`Firmware\Src\main.c`。
>   - `Get-Content` 读取 `HardwareDocs\Datasheet.md`、`ReferenceManual.md`、`Errata.md`、`Board\README.md` 和现有 hardware-analysis 文档。
> - 未使用网络。

## Captured report body from documented-convention fixture

The actual generated report is preserved below so the baseline remains reviewable without the ignored scratch workspace.

```markdown
# MT32X USART3 + DMA + RS485 硬件事实整理

## 引脚功能

数据手册列出两组可选的 USART3 引脚复用：

| MCU 引脚 | 复用功能 | 电气限制 |
| --- | --- | --- |
| PA2 / PA3 | AF8：USART3_TX / USART3_RX | LQFP64 封装有引出；5 V tolerant |
| PC10 / PC11 | AF7：USART3_TX / USART3_RX | LQFP64 封装有引出；非 5 V tolerant，输入不得超过 3.6 V |

两组是候选路由，不能据此判断当前板卡实际连接了哪一组。开发前应以板级原理图或 PCB 网络确认 MCU 到 U5 的 TX/RX 连线，再配置对应引脚和 AF。现有板级资料只说明 U5 是 RS485 收发器；原理图不可用，因此 U5 的 MCU 侧信号、总线 A/B 以及 DE/RE 控制脚映射均未核实。不能从这些资料推定 DE/RE GPIO。

## USART3、DMA 与中断配置依据

- USART3 使用 APB1 内核时钟。启用 GPIO 和 APB1 时钟后再置 USART3 UE。
- USART3_RX 可选 DMA1 Channel 4 Stream 1 或 Stream 3；USART3_TX 可选 DMA1 Channel 4 Stream 3 或 Stream 4。收发同时工作时应分配不同 stream，并检查它们未被工程中的其他 DMA 功能占用；例如 RX 用 Stream 1、TX 用 Stream 4 符合资料列出的映射。
- 使能接收和发送 DMA 请求时，分别设置 USART3_CR3 的 DMAR、DMAT。具体 DMA 数据宽度、地址自增、循环模式等参数未由现有摘录规定。
- USART3 全局中断名为 USART3_IRQn；DMA 中断使用所选 DMA1 stream 对应的 IRQ。
- DMA 传输完成只表示内存传输结束，不代表最后一个停止位已发出。发送方向若由软件控制 RS485 发送使能，应等待 USART3_SR.TC 后再切回接收；但当前资料没有给出 U5 的 DE/RE 接法或收发切换时序。

## 注意事项与待确认项

1. PA2/PA3 的 5 V tolerant 属性不等于已确认其与 U5 逻辑侧兼容；PC10/PC11 不得超过 3.6 V。RS485 总线侧应通过收发器连接，现有资料未提供 U5 电气参数。
2. MT32X 勘误仅指出 REV_A 在 USART3_RX 使用 IDLE 处理、DMA stream 被关闭并重新启用的场景中，可能丢失 DMA 请求。REV_A 的规避顺序是在重新使能 stream 前先读 USART3_SR、再读 USART3_DR；REV_B 不受此项影响。当前项目硅版本未知，应先确认版本；若使用该场景，按此顺序处理。
3. 现有 `Firmware/Src/main.c` 只是空的合成入口，没有可供核对的 USART3、DMA 或 RS485 初始化实现。此前 UART1 的分析记录也明确不能作为 USART3 板级接线证据。

## 资料来源

- `HardwareDocs/Datasheet.md`：§2.1 Table 4、§5.3 Table 20
- `HardwareDocs/ReferenceManual.md`：§11.2 Table 11-7、§17.4、§17.7、§17.8
- `HardwareDocs/Errata.md`：§2.3.4
- `HardwareDocs/Board/README.md`：U5 与原理图可用性说明
```
