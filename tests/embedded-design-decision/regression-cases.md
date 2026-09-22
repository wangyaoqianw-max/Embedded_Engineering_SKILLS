# Embedded Design Decision Regression Cases

## 通用前置条件

- 每个正向场景都给出明确输出目录和文件名。
- 设计文档必须区分事实、假设和未知项。
- 不允许修改被分析工程的业务代码或配置。
- 不允许用所有子系统共用的单一复杂度掩盖局部差异。

## CASE-01 GPIO + LED + Button

输入：为资源受限 MCU 上的一个按键控制 LED 功能做编码前设计，输出到指定的 `case-01.md`。

必须观察到：采用 L0 或等价的直接实现；说明最小模块职责。

不得观察到：Device Manager、Event Bus、通用插件框架、RTOS 任务体系。

## CASE-02 UART DMA + RingBuffer + FreeRTOS

输入：设计 UART DMA 接收、RingBuffer 和 FreeRTOS 消费任务，输出到指定的 `case-02.md`。

必须观察到：ISR 到 Task 的数据流；UART 或接收缓冲区的明确所有者；并发与溢出处理。

不得观察到：在 ISR 中执行业务解析；没有依据的多层通用消息总线。

## CASE-03 STM32F4 Bootloader

输入：设计只负责校验并跳转应用的 STM32F4 Bootloader，输出到指定的 `case-03.md`。

必须观察到：Bootloader 独立评估且保持精简；启动、校验、跳转和失败路径清楚。

不得观察到：机械引入 RTOS、完整五层应用架构或动态插件系统。

## CASE-04 Bootloader + OTA + A/B + Rollback

输入：设计带 OTA、A/B 分区和回滚的产品升级方案，输出到指定的 `case-04.md`。

必须观察到：Application 与 Bootloader 分别选择复杂度；镜像状态、确认、回滚和掉电一致性明确。

不得观察到：把 Application 与 Bootloader 当作同一运行模型；只描述正常升级路径。

## CASE-05 MCU 与 Flash 迁移

输入：现有产品从 F411/W25Q64 迁移到 F407/W25Q128，要求尽量不重写 OTA 服务，输出到指定的 `case-05.md`。

必须观察到：识别平台和器件迁移这一真实变化轴；保护稳定 Service 边界；列出需重新验证的容量、扇区和时序事实。

不得观察到：直接重写 OTA 业务；宣称两个器件完全兼容而不验证。

## CASE-06 Existing Project Sensor Module

输入：公司已有架构，只新增 Sensor Module，输出到指定的 `case-06.md`。

必须观察到：先读取并服从现有架构；只设计模块、接口、所有权和接入点。

不得观察到：重新设计整个产品；无依据替换现有平台或 RTOS 模型。

## TRIGGER-01 显式调用

使用 `$embedded-design-decision` 并给出输出目录和文件名，必须生成设计文档。

## TRIGGER-02 隐式调用

不写 Skill 名称，但明确请求嵌入式编码前架构设计并给出输出目录，必须生成设计文档。

## TRIGGER-03 缺少输出目录

请求嵌入式设计但不提供目录，必须先询问；不得创建设计文档或自行选择目录。

## NONTRIGGER-01 直接编码

仅要求实现 CRC 函数时，不应扩展成项目设计文档。

## NONTRIGGER-02 构建或烧录

仅要求编译或烧录现有工程时，不应输出架构设计文档。

## NONTRIGGER-03 纯概念解释

仅询问 Strategy 模式含义时，应直接解释概念，不应要求输出目录。

## 当前回归执行记录

- 日期：2026-09-22
- junction：已创建并验证，用户级 Skill 目录指向仓库源码。
- TRIGGER-01 显式调用：未验证。Codex CLI 进入模型请求后连续出现 `stream disconnected before completion: Incomplete response returned, reason: adapter_eof`，未生成输出文档。
- TRIGGER-02 隐式调用：未验证。同一上游 Responses 流错误，未生成输出文档。
- TRIGGER-03 缺少输出目录：未验证。同一批会话在模型请求阶段失败。
- NONTRIGGER-01 至 NONTRIGGER-03：未执行，因为同一外部 Responses 服务故障已阻断顺序重试。
- 结论：本次失败属于 Codex CLI 上游 Responses 流/网络状态，不能作为 Skill 行为 FAIL；真实触发行为保持未验证。
