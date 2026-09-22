# Embedded Design Decision Baseline

## 环境

- Codex CLI：`codex-cli 0.151.0`
- 目标 Skill：用户级目录中未安装，仓库内尚无 `SKILL.md`
- 执行方式：六个相互隔离的临时工程、ephemeral session
- 日期：2026-09-22

## 汇总

| 场景 | 文档落盘 | 关键决策满足情况 | 观察到的缺口 |
| --- | --- | --- | --- |
| CASE-01 GPIO + LED + Button | 是 | 最小模块、无 RTOS 和动态内存 | 186 行且拆成三层；没有 L0 判断、拒绝方案和实施路线 |
| CASE-02 UART DMA + RingBuffer + FreeRTOS | 是 | ISR→RingBuffer→Task、所有权、溢出和 Cache 约束完整 | 393 行；没有 L2 判断，事实/未知项未形成统一合同 |
| CASE-03 STM32F4 Bootloader | 是 | 边界精简、校验/跳转/失败路径清楚，无 RTOS | 308 行；输入未给具体型号却采用 F407VG 示例，缺少复杂度判断 |
| CASE-04 OTA + A/B + Rollback | 是 | 覆盖镜像状态、确认、回滚、掉电和数据兼容 | 405 行；未分别给 Application 与 Bootloader 选择复杂度 |
| CASE-05 MCU 与 Flash 迁移 | 是 | 识别迁移变化轴，保留 OTA Service 并下沉适配 | 文档残留旧分区图占位符，关键未知项没有在写入前收束 |
| CASE-06 Existing Project Sensor Module | 是 | 明确不重构总线、调度和其他模块 | 未提供既有架构资料仍直接生成 312 行通用分层设计，没有先索取关键事实 |

## CASE-01 GPIO + LED + Button

- 输入：资源受限 MCU 上的按键控制 LED 编码前设计。
- 实际行为：生成 186 行设计文档，采用周期轮询、计数消抖和固定板级接口。
- 满足项：未引入 Device Manager、Event Bus、RTOS 或动态内存。
- 缺口：没有明确判断为 L0；对单一功能拆出板级 GPIO、按键消抖、LED 控制三层，输出深度偏重；没有独立说明不采用的设计和按依赖排列的实施路线。
- 临时证据：`case-01.md`、`case-01-final.txt`。
- 关键摘录：“采用周期轮询 + 计数去抖；全程无动态内存、无阻塞延时。”

## CASE-02 UART DMA + RingBuffer + FreeRTOS

- 输入：UART DMA 接收、RingBuffer 和 FreeRTOS 消费任务设计。
- 实际行为：生成 393 行设计文档，完整处理 DMA 位置、SPSC RingBuffer、任务通知、溢出、Cache 和错误恢复。
- 满足项：明确 ISR→Task 数据流，给出 DMA、RingBuffer 与消费任务的所有权；拒绝 ISR 内协议解析和逐字节 Queue。
- 缺口：没有明确判断为 L2；事实、假设和未知项没有采用一致的文档合同；技术细节明显超过前期决策所需粒度。
- 临时证据：`case-02.md`、`case-02-final.txt`。
- 关键摘录：“DMA 只写 `dma_rx_buf`，ISR 只生产 `rx_rb`，消费任务只消费 `rx_rb`。”

## CASE-03 STM32F4 Bootloader

- 输入：只负责校验并跳转应用的 STM32F4 Bootloader。
- 实际行为：生成 308 行设计文档，排除下载、擦写、通信、双分区和回滚，并覆盖镜像校验、跳转和失败策略。
- 满足项：没有机械引入 RTOS 或完整应用架构，Bootloader 范围清楚。
- 缺口：输入只给出 STM32F4，文档仍选用 STM32F407VG/1 MiB 作为主要示例；虽然标注为假设，仍增加了不必要的具体实现细节；没有明确复杂度判断。
- 临时证据：`case-03.md`、`case-03-final.txt`。
- 关键摘录：“明确不包含：固件下载、Flash 擦写、通信协议、双分区切换、回滚。”

## CASE-04 Bootloader + OTA + A/B + Rollback

- 输入：带 OTA、A/B 分区和回滚的产品升级方案。
- 实际行为：生成 405 行产品级方案，覆盖升级代理、Bootloader、分区、试启动、确认、回滚、掉电和数据迁移。
- 满足项：镜像状态、确认条件、回滚触发和异常路径清楚，不只描述正常升级。
- 缺口：虽然区分组件职责，但没有分别给出 Application 与 Bootloader 的复杂度选择；缺少统一的范围分类和复杂度依据。
- 临时证据：`case-04.md`、`case-04-final.txt`。
- 关键摘录：“复位后：Boot ROM → Bootloader → 选择 A/B 槽 → 系统自检 → 确认或回滚。”

## CASE-05 MCU 与 Flash 迁移

- 输入：F411/W25Q64 迁移到 F407/W25Q128，尽量不重写 OTA Service。
- 实际行为：生成 283 行迁移设计，保留 OTA API 和状态机，把变化限制在 MCU、Flash、Bootloader 目标和发布配置。
- 满足项：正确识别平台与器件迁移变化轴，保护稳定 Service 边界，列出扇区、容量、包格式和发布隔离验证。
- 缺口：关键分区信息未知时仍写入最终文档，并留下旧分区图占位符；没有在生成最终文档前先索取会改变布局的关键事实。
- 临时证据：`case-05.md`、`case-05-final.txt`。
- 关键摘录：“本次迁移不需重写 OTA 服务。”以及分区表中的未完成占位标记。

## CASE-06 Existing Project Sensor Module

- 输入：公司已有架构，只新增 Sensor Module。
- 实际行为：生成 312 行模块设计，提出 Service、Device 与 Existing Platform API 边界。
- 满足项：明确不得顺带重构总线层、调度层或其他设备模块，范围没有扩展到完整产品。
- 缺口：临时工程没有任何现有架构、接口或规范，仍直接给出具体分层和接口；对于 Existing Project Module，本应先索取会决定接入方式的现有工程事实。
- 临时证据：`case-06.md`、`case-06-final.txt`。
- 关键摘录：“不得因新增 Sensor Module 顺带重构总线层、调度层或其他设备模块。”

## 基线结论

无目标 Skill 时，模型已经能生成技术上较完整的嵌入式设计，六个场景均成功落盘，核心并发、Bootloader、OTA 和迁移判断多数正确。因此 V0.1 的价值不能定义为“让模型会设计”，而应集中在稳定纠正以下行为：

1. 根据任务范围控制文档深度，避免简单场景也输出数百行实现细节。
2. 每次明确范围分类和局部复杂度等级，而不是只给技术方案。
3. 使用统一的事实、假设、未知项、拒绝方案和实施路线合同。
4. 关键未知项会改变架构时先询问，不能在最终文档中保留占位符或臆造已有工程接口。
5. 对 Application、Bootloader 和局部模块分别判断复杂度，不使用一个全局架构等级覆盖所有子系统。
