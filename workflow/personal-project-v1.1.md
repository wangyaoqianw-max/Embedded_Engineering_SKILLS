# 个人嵌入式软件项目学习与开发工作流（V1.1）

- 版本：V1.1
- 日期：2026-10-20
- 适用：个人学习、技术探索、独立固件作品、小型验证项目
- 目标：功能可运行、技术可解释、过程可复现、经验可迁移
- 定位：工程师的操作指南；AI 与 Skill 可辅助执行，但不是流程成立的前提


## V1.1 重要变化：外层学习目标，内层逐切片闭环

先确定学习目标、板卡资料、最小架构和关键公共接口，再按依赖拆分功能。**每个子任务都执行局部 Design → Implement → Verify → Feedback**；简单实验可以只保留一个子任务。切片应有可观察结果，而非只有“某层代码写完”。例如 UART DMA 先通过基本数据通路，再验证缓冲回绕、IDLE/HT/TC 事件协作、任务消费及异常恢复；已有能力应直接复用。

切片任务卡最少记录：本片目标、影响范围、资源所有者、验收用例及预期、Host/板级环境、实际测试结果。AI 可辅助生成设计与代码，但工程师需自己解释核心原理；验证证据不能由 AI 口述代替。下文每一阶段列有现有可用 Skill 和**仅供规划的未开发候选 Skill**。


## 1. 使用原则与边界

1. **学习与交付双目标**：既要实现功能，又要形成可以自己解释、修改和调试的能力；不能用 AI 生成代码的速度代替学习结果。
2. **先澄清关键问题，再编码**：AI 可以反向追问，但只问会影响设计、接口、资源或验证的问题。简单任务不要进行冗长访谈。
3. **最小可验证闭环**：将工作拆为能运行和观察结果的小功能，而不是先建好所有层级再一次性联调。
4. **验证先于“完成”**：必须区分已编写、已编译、Host 测试通过、目标板验证通过；缺少证据不得声明已完成。
5. **必要复杂度**：不因学习“架构”就为单个 LED、传感器或驱动制造过多层级；复杂度由实际需求与风险决定。
6. **可回退、可复盘**：保留能恢复的版本节点、重要决策和失败经验；不需要记录每一轮聊天。

软件工程师需要阅读原理图、Datasheet、Reference Manual 和 Errata，核实引脚、时钟、DMA、IRQ、总线等事实，并完成板级调试；本流程不扩展到硬件电路设计、结构、生产工艺等其他岗位的工作流程。

## 2. 选择执行深度

| 情形 | 建议路径 | 最小必要记录 |
| --- | --- | --- |
| 单一概念或简单外设实验 | 轻量：目标 → 实现 → 验证 → 讲清原理 | 一条验收结果、关键现象 |
| 新驱动、协议或 RTOS 功能 | 标准：完整步骤，合并非必要文档 | 简要设计、任务记录、测试证据 |
| Bootloader / OTA / 并发 / 掉电一致性等高风险主题 | 强化：先验证关键假设，覆盖失败路径 | 关键决策、异常用例、板级证据 |

路径可在过程中升级。选择“轻量”不意味着可以跳过与风险相关的安全检查。

## 3. 工作流程

### P0｜确定学习目标与项目边界

**本阶段 Skill**：现有可用：无需专门 Skill。**可能开发（未开发）**：embedded-requirements-clarification（目标歧义）。


- **输入**：想掌握的知识点、已有技能、可用板卡与工具。
- **操作**：写清本轮要回答的技术问题；确定可展示的行为、明确不做什么；核对 MCU 型号、板卡修订版、现有仓库与工具链。
- **输出**：一段目标说明、功能完成条件、学习完成条件。
- **出口**：能回答“要实现什么”“为什么学”“什么证据证明成功”；否则缩小范围。

示例：不是“研究 UART”，而是“在指定 STM32 板上使用 DMA Circular + IDLE 接收变长帧，正确处理缓冲区回绕，并能解释读写索引与并发边界”。

### P1｜需求质询与技术资料核实

**本阶段 Skill**：现有可用：[embedded-resource-acquisition](../skills/embedded-resource-acquisition/SKILL.md)；[extracting-chip-hardware-facts](../skills/extracting-chip-hardware-facts/SKILL.md)；已有源码则 [embedded-code-reader](../skills/embedded-code-reader/SKILL.md)。**可能开发（未开发）**：暂无：已有资料与硬件事实工具。


- **输入**：P0 目标、当前硬件和工程事实。
- **操作**：请 AI 挑出会导致返工的未知项（如 DMA 模式、缓冲区满策略、任务通知语义）；自己优先查官方手册、原理图与 Errata，必要时做最小实验验证。
- **输出**：已确认事实、待验证假设、关键约束、资料出处。
- **出口**：涉及外设能力、引脚连接、时钟、中断和资源归属的关键前提已有可信依据；无法确认的内容明确列为待验证，不能当事实编码。

可选工具：资料缺项时使用 [embedded-resource-acquisition](../skills/embedded-resource-acquisition/SKILL.md)；提取芯片和板卡事实时使用 [extracting-chip-hardware-facts](../skills/extracting-chip-hardware-facts/SKILL.md)。

### P2｜最小充分设计

**本阶段 Skill**：现有可用：[embedded-design-decision](../skills/embedded-design-decision/SKILL.md)（真实设计取舍时）。**可能开发（未开发）**：暂无：复用现有设计 Skill。


- **输入**：已核实事实与功能约束。
- **操作**：先画清数据流、调用关系与执行上下文；再决定主循环 / ISR / DMA / RTOS Task 的职责、状态所有权、接口、缓存边界与错误处理。比较有实际差异的候选方案，记录为何选择当前方案。
- **输出**：简要设计与局部验证策略；复杂决策按需用 ADR 记录，术语不一致时维护共享词汇表。
- **出口**：关键接口、资源所有者、正常路径、至少一类关键失败路径和验证方式明确；拒绝无证据的过度抽象。

可以调用 [embedded-design-decision](../skills/embedded-design-decision/SKILL.md)；该 Skill 只负责设计，不自动实施。

### P3｜按可验证切片制定计划

**本阶段 Skill**：现有可用：[embedded-design-decision](../skills/embedded-design-decision/SKILL.md)（已有实施路线，不是任务管理 Skill）。**可能开发（未开发）**：embedded-task-decomposition（依赖与验收切片）。


- **输入**：P2 设计。
- **操作**：按依赖关系拆为可独立验证的小阶段（Tracer Bullet）；为每个阶段写明输入、交付行为、测试方式和完成标准。优先构建最早能观察到真实信号的闭环。
- **输出**：简短任务清单，包含依赖与验收标准。
- **出口**：下一项任务足够小，可以在一个工作周期内完成并明确判断通过或失败。

示例（UART DMA）：建立构建基线 → 接收固定数据 → 验证回绕 → 验证 IDLE/HT/TC 协作 → 验证溢出恢复 → 与 FreeRTOS 消费任务集成。顺序以项目实际依赖为准。

### P4｜每个切片的设计—实现—验证—反馈小循环

**适用范围**：个人项目的每个可验证子任务；如果无法合理拆分，则将整个任务视为一个切片。已有通过验证的切片是下一切片的基线。**小循环必须发生在每个切片内部，而不是做完全部设计和编码后只验证一次。**

#### P4-D｜Design：切片局部设计

- **输入**：该切片目标、外层确认的接口与硬件事实、已通过的前置切片。
- **动作**：确认需要修改的范围、接口/任务/ISR/DMA 与资源所有权、核心状态流、正常与关键失败路径；为本片制定测试输入、预期结果、环境与失败判据。
- **输出及出口条件**：简短切片设计卡（目标/非目标、边界、取舍、验收用例、前置依赖）；没有会改变实现的关键未知项。
- **已有 Skill**：[embedded-design-decision](../skills/embedded-design-decision/SKILL.md)（有实质设计取舍时）；[embedded-code-reader](../skills/embedded-code-reader/SKILL.md)（已有工程需核对调用关系时）。
- **未来候选 Skill（未开发）**：embedded-requirements-clarification（仅歧义阻碍设计时）；不再另造重复的设计 Skill。

#### P4-I｜Implement：最小编码与构建

- **动作**：只实现当前切片的可观察行为；同步必要工程文件/配置，遵守项目架构和生成代码边界。纯逻辑或可控测试接缝适合 Red → Green → Refactor；不要求所有硬件依赖路径都先写 Host TDD。
- **输出及出口条件**：本片 diff、实际 Target 的构建结果和可供测试的产物；**编译通过不是验证通过**。
- **已有 Skill**：[embedded-code-development](../skills/embedded-code-development/SKILL.md)。
- **未来候选 Skill（未开发）**：暂无新编码 Skill 需求；避免复制现有 Implement/Modify/Port。

#### P4-V｜Verify：实际执行验收

- **动作**：运行计划中的 Host 测试、目标板功能/边界/异常用例和必要时序/资源测量。对 DMA、ISR、RTOS、OTA/Flash 等行为，应依据风险取得 J-Link/RTT/逻辑分析仪/复位与故障注入等真实证据。
- **输出及出口条件**：记录环境、版本、输入、预期、实际、证据位置和 PASS/FAIL/BLOCKED。源码证据、编译、Host、板级、系统回归分别报告；没有执行不得写 PASS。
- **已有 Skill**：[embedded-code-development](../skills/embedded-code-development/SKILL.md) 负责已批准的局部测试入口、模块自测和实现相关检查；**尚无独立测试执行 Skill**，仍需真实工程工具和人员执行。
- **未来候选 Skill（未开发）**：embedded-test-verification（独立执行、记录并核对测试证据）。

#### P4-F｜Feedback：分析结果、修正、回归、关闭切片

- **PASS**：对照需求/工程规范做轻量自检并保存已验证 commit、测试证据与任务状态，然后开始下一项依赖切片。
- **FAIL**：保留最小复现和观测；实现缺陷回 P4-I；局部接口/算法错误回 P4-D；若波及公共 API、存储格式或全局资源所有权，则升级到外层设计/批准阶段，同时检查之前通过的切片是否受影响。
- **BLOCKED**：说明缺失设备、环境或依赖及恢复条件；不允许假设测试通过或推进依赖任务。
- **已有 Skill**：[embedded-code-reader](../skills/embedded-code-reader/SKILL.md)（读码定位）、[embedded-design-decision](../skills/embedded-design-decision/SKILL.md)（需要回设计时）、[embedded-code-development](../skills/embedded-code-development/SKILL.md)（明确缺陷后修复）。
- **未来候选 Skill（未开发）**：embedded-code-review（独立审查）、embedded-debugging（根因不明）、embedded-work-handoff（长任务交接）。

**退出条件**：本切片验收证据成立，且与已验证基线的必要兼容检查通过。**所有切片通过仍需外层跨切片集成回归与交付/复盘。**

### P5｜跨切片集成、目标板回归与故障诊断

**本阶段 Skill**：现有可用：[embedded-code-reader](../skills/embedded-code-reader/SKILL.md)（理解跨模块路径）、[embedded-code-development](../skills/embedded-code-development/SKILL.md)（必要修复）。**可能开发（未开发）**：embedded-test-verification（集成/板测证据）、embedded-debugging。


- **输入**：可构建固件、预期行为和关键风险。
- **操作**：按风险选择 Host 测试、板级测试、异常输入、长稳/压力与故障注入。使用 J-Link、RTT、日志、逻辑分析仪等工具采集原始证据；比较预期与实测，而不是仅看串口打印“成功”。
- **输出**：测试条件、实际结果、失败证据、未测项清单。
- **出口**：本轮明确的功能与关键异常路径得到与其风险相称的验证。无法执行的验证标为未验证，不等同于通过。

嵌入式特有的检查视任务选择：时序 / 最坏延迟、缓存越界、ISR-Task 并发、DMA 所有权、栈与堆、复位状态、掉电恢复、Flash 擦写边界等。测试专用注入不替代真实硬件电气行为验证。

### P6｜审查与费曼复盘

**本阶段 Skill**：现有可用：[embedded-design-decision](../skills/embedded-design-decision/SKILL.md)（发现架构错误后重新决策）；人工费曼复盘。**可能开发（未开发）**：embedded-code-review（需求/规范/证据）。


- **输入**：实际代码、设计约束和测试证据。
- **操作**：从两个角度审查：①是否实现了原始要求；②是否符合工程规则、资源与架构约束。对高风险行为另核对目标板证据。然后脱离 AI，尝试解释机制、取舍、失败场景及调试过程。
- **输出**：修复清单、关键解释、尚未掌握的问题。
- **出口**：重要问题已修复或明确记录；自己能够讲清核心原理，并能指出至少一种方案限制。讲不清的部分回到实验或源码分析。

### P7｜沉淀、版本记录和下一轮计划

**本阶段 Skill**：现有可用：Git 与项目笔记；无现成专门 Skill。**可能开发（未开发）**：embedded-work-handoff（跨会话交接）。


- **输入**：通过验证的切片、复盘结果。
- **操作**：保留最有复用价值的知识：设计理由、关键源码位置、测试输入/结果、踩坑与待解决问题；提交代码与必要文档。区分“已完成的能力”和“下一轮想学的内容”。
- **输出**：可恢复的版本点、可复用笔记、后续任务清单。
- **出口**：重新打开仓库时，不依赖旧聊天即可知道当前完成情况、验证边界与下一步。

## 4. AI 协作约定

- **导师**：先检查我的解释与假设；核心概念可通过追问、反例和小实验引导，但用户要求直接答案时直接回答。
- **研究助手**：给出处与适用型号/版本，区分事实、推导、未知；不可虚构寄存器、硬件连接或测试结果。
- **实现助手**：在已批准设计内做最小代码改动，说明实际修改与验证。
- **审查者**：分别审查规格符合性、规范符合性和硬件验证证据；不因为同一 AI 自己写了代码就默认正确。
- **最终责任**：由工程师确认设计、运行测试和决定完成状态。

## 5. 每个功能切片的最小完成标准（DoD）

- [ ] 目标行为和边界条件可明确陈述。
- [ ] 实现与现有工程约束一致。
- [ ] 构建状态、Target 和结果有记录。
- [ ] 约定的测试已执行，包含必要的异常/边界用例。
- [ ] 板级相关结论有目标板证据；没有证据的结论标为未验证。
- [ ] 核心技术原理可以独立解释；后续问题已记录。

## 6. 经验回退规则

- **需求冲突** → 回 P0/P1，修正目标或补齐事实。
- **接口/所有权不清** → 回 P2，不用代码擅自决定架构。
- **测试连续失败** → 最小化复现、提出可证伪假设、增加定向观测，再修复。
- **功能正确但无法解释** → 回 P6/P1 做解释与实验，不急着追加新功能。
- **发现新增风险** → 提高验证等级和文档深度，不强行维持轻量路径。

## 7. 方法参考

- [Matt Pocock — Skills For Real Engineers](https://github.com/mattpocock/skills)：借鉴需求追问、共享术语/ADR、可验证垂直切片、反馈循环、双维代码审查和回顾；本流程面向嵌入式个人学习进行了重新设计。
- 本仓库 [设计决策框架](../skills/embedded-design-decision/references/decision-framework.md)：以项目事实、最低必要复杂度和风险驱动验证为先。

## V1.1 Skill 开发方向与约束

跨三套流程最值得先试验的候选是 **embedded-task-decomposition**（把设计拆成可验证切片）、**embedded-test-verification**（独立执行并核实 Host/板级/回归证据）、**embedded-code-review**（分开 Spec/Standards/Evidence 审查）。**embedded-debugging**（根因未知）、**embedded-requirements-clarification**（需求歧义）、**embedded-work-handoff**（跨会话长任务）按实际高频失败场景再决定是否开发。

**这些都尚未开发**；不要调用不存在的 Skill。一个候选 Skill 只解决一个独立、重复的工程问题；先建立失败基线，再按仓库 [AGENTS.md](../AGENTS.md) 验证显式触发、隐式触发、误触发与核心行为。完整路线图在 [工作流索引](README.md)，参考 [Matt Pocock 的方法](https://github.com/mattpocock/skills) 和 [本仓库设计框架](../skills/embedded-design-decision/references/decision-framework.md)。
