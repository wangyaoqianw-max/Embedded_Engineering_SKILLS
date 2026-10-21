# 嵌入式软件开发工作流

这里保存面向**嵌入式软件工程师**的日常研发指南，**不是可自动执行的 Skills**。当前推荐 **V1.2**；V1.1 与 V1.0 作为历史基线保留，未经本次修改。

| 适用场景 | 当前 V1.2 | 历史 V1.1 | 历史 V1.0 |
| --- | --- | --- | --- |
| 个人学习、技术实验及作品 | [个人项目](V1.2/personal-project-v1.2.md) | [V1.1](V1.1/personal-project-v1.1.md) | [V1.0](V1.0/personal-project-v1.md) |
| 企业级完整固件或大型子系统 | [企业固件](V1.2/enterprise-firmware-v1.2.md) | [V1.1](V1.1/enterprise-firmware-v1.1.md) | [V1.0](V1.0/enterprise-firmware-v1.md) |
| 既有固件的功能、模块与缺陷变更 | [模块开发](V1.2/existing-module-v1.2.md) | [V1.1](V1.1/existing-module-v1.1.md) | [V1.0](V1.0/existing-module-v1.md) |

**V1.2 专题说明**：[工程文档体系与交付约定](V1.2/documentation-guidelines-v1.2.md)。

## V1.2 新增：文档生命周期

文档按职责区分为研发过程资料（需求、阶段设计/计划、handoff、review、验证证据）与对接/使用/发布所需的交付资料；**正式技术规范**（架构、API、通信协议、存储/升级格式等）是两种用途共享的单一权威来源，不复制两套。

- **检查点 A：模块/切片完成**。检查公共契约是否变化；需要时维护正式技术规范，并核对代码和测试；不需要时在现有任务/Review 记录 `DOC-NONE` 及理由。
- **检查点 B：集成/版本交付**。系统联调时完善必要的配置、接口和集成说明；版本交付前按接收方需要审核烧录/升级/维护手册、测试摘要和发布说明。
- **状态和证据**。文档批准、实现完成、Host/板级/系统验证必须分别陈述。允许按模块逐步生成限定范围的指南，但不能把它冒称为完整产品手册。
- **轻量裁剪**。已有目标项目的目录、流程、权限和质量门禁优先；不要求为每个切片创建文档，也不自动建立新的 Documentation Skill。

## V1.2 共同开发循环（延续 V1.1）

**外层**：澄清目标 → 核对事实／现有代码 → 确定必要公共契约 → 拆分可验收小任务。

**内层（每个切片）**：**Design 局部设计 → Implement 最小实现 → Verify 实际测试 → Feedback 纠偏与关闭/文档检查**，再进入下一切片。单项简单任务可只有一个切片。切片 FAIL 返回设计或编码；BLOCKED 记录环境阻碍；公共接口变化回外层；所有片完成后仍需跨模块集成、回归、审查与交付。

每片最小记录：目标、非目标、接口/资源边界、验收输入和预期、Host/板级测试环境、构建 Target、实际结果（PASS/FAIL/BLOCKED）、证据及未测项。**编译成功 ≠ Host 测试通过 ≠ 板级测试通过 ≠ 系统回归完成**。

## 当前已有的 5 个 Skill

| Skill | 职责 |
| --- | --- |
| [embedded-resource-acquisition](../skills/embedded-resource-acquisition/SKILL.md) | 技术资料盘点、来源与获取 |
| [extracting-chip-hardware-facts](../skills/extracting-chip-hardware-facts/SKILL.md) | 芯片/板级相关事实及使用限制 |
| [embedded-code-reader](../skills/embedded-code-reader/SKILL.md) | 现有固件代码与实际调用、数据/状态流 |
| [embedded-design-decision](../skills/embedded-design-decision/SKILL.md) | 项目与模块的软件设计决策 |
| [embedded-code-development](../skills/embedded-code-development/SKILL.md) | 已明确需求的代码实现、修改和移植 |

## 未来候选 Skill（以下全部尚未开发）

| 候选名称 | 单一职责 | 初步优先级 |
| --- | --- | --- |
| embedded-task-decomposition | 设计/需求 → 可独立验收的切片、前后依赖与状态；不重新做架构 | 高 |
| embedded-test-verification | 独立执行和核验单元/板级/回归测试证据；不把未执行结果当 PASS | 高 |
| embedded-code-review | 对照 Spec、Standards、Evidence 审查代码差异；不直接代替实现 | 高 |
| embedded-debugging | 根因未知时复现、缩小、假设、定向观测和回归 | 按需 |
| embedded-requirements-clarification | 对复杂/歧义需求定向追问、输出可验收条件；不替代设计 | 按需 |
| embedded-work-handoff | 跨会话/跨任务保留代码基线、切片状态和待确认项 | 最后考虑 |

**候选不是现有能力，也不是必须开发的清单。** 每项开发前先确认现有 Skill 能否满足任务，搜集失败基线并按 [AGENTS.md](../AGENTS.md) 验证显式、隐式、边界和误触发。构建、烧录、调试仍沿用真实项目已有工具，不假定仓库已有 Build/Debug/Test Skill。

参考：[Matt Pocock — Skills For Real Engineers](https://github.com/mattpocock/skills) 的需求澄清、Tracer Bullet、TDD 反馈与独立审查，以及 [本仓库的设计决策框架](../skills/embedded-design-decision/references/decision-framework.md)。
