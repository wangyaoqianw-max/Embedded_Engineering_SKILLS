---
name: embedded-design-decision
description: 在嵌入式产品或模块编码前，根据项目范围、现有工程、硬件资源、实时性、可靠性和演化约束生成工程设计决策文档。用于架构复杂度、模块边界、依赖方向、运行模型、资源所有权、关键接口和实施路线设计；不用于直接编码、构建、烧录、调试、测试执行或纯概念解释。
---

# Embedded Design Decision

## 前置条件

要求用户指定设计文档输出目录。未指定时只询问目录，不开始最终设计；目录与文件路径不清时先澄清。检查目标目录中的现有文档和命名规范，同名文件不得直接覆盖。

## 工作流

1. 读取需求、现有工程、架构文档和项目规则。
2. 将输入区分为已验证事实、假设和未知项；会改变架构的未知项先询问。
3. 读取 [decision-framework.md](references/decision-framework.md)，分类 Enterprise Product、Personal Project 或 Existing Project Module。
4. 识别架构驱动因素，为各子系统选择满足已知约束的最低复杂度。
5. 设计模块边界、依赖方向、运行模型、资源所有权和关键接口。
6. 检查过度设计与设计不足，明确不采用的方案及理由。
7. 按依赖关系生成分阶段实施路线。
8. 把完整 Markdown 设计文档写入用户指定目录并以 UTF-8 回读核验。

## 参考资料路由

- 每次读取 [decision-framework.md](references/decision-framework.md)。
- 涉及抽象、变化隔离、接口或设计模式时，读取 [设计模式_GoF_嵌入式软件蒸馏.md](references/设计模式_GoF_嵌入式软件蒸馏.md)。
- 涉及 Task、ISR、IPC、实时性、内存或并发时，读取 [freertos-deep-research-report.md](references/freertos-deep-research-report.md)。
- 涉及启动、升级、分区、回滚或镜像安全时，读取 [bootloader-ota-deep-research-report.md](references/bootloader-ota-deep-research-report.md)。
- 存在相似工程场景时读取 [engineering-experience.md](references/engineering-experience.md)；候选经验不能覆盖稳定规则。
- 不无条件加载全部参考文件。当前项目的可验证事实优先于通用资料。

## 文档合同

设计文档按任务范围和复杂度调整深度。简单功能保持简洁；不适用内容可合并说明，不为满足目录而扩写。文档覆盖：

- 背景与目标、任务范围。
- 已验证事实、假设和未知项。
- 关键约束与架构驱动因素。
- 各子系统的复杂度选择及依据。
- 模块边界、依赖方向、运行模型、资源所有权和关键接口。
- 资源规划、可靠性和故障策略。
- 不采用的设计及理由。
- 风险、验证项和按依赖关系排列的实施路线。

关键未知项未关闭时不写含占位符的最终方案；先询问，或在用户明确接受条件式设计后输出备选分支。

## 停止边界

文档写入并核验后停止。不得继续修改业务代码、构建配置或工程结构，不执行构建、烧录、调试或测试。聊天回复只给出文件位置、核心结论和未确认项。
