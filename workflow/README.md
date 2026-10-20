# 嵌入式软件开发工作流

这里保存面向**嵌入式软件工程师**的日常工作指南，并不是会自动执行的 Skills。当前推荐 **V1.1**，历史 V1 保留不修改。

| 适用场景 | 当前 V1.1 | 历史 V1 |
| --- | --- | --- |
| 个人学习、技术实验及作品 | [个人项目](personal-project-v1.1.md) | [V1](personal-project-v1.md) |
| 企业级完整固件或大型子系统 | [企业固件](enterprise-firmware-v1.1.md) | [V1](enterprise-firmware-v1.md) |
| 既有固件的功能、模块与缺陷变更 | [模块开发](existing-module-v1.1.md) | [V1](existing-module-v1.md) |

## V1.1 共同循环

**外层**：澄清目标 → 核对事实／现有代码 → 确定必要公共契约 → 拆分可验收小任务。

**内层（每个切片）**：**Design 局部设计 → Implement 最小实现 → Verify 实际测试 → Feedback 纠偏与关闭**，再进入下一切片。单项简单任务可只有一个切片。切片 FAIL 返回设计或编码；BLOCKED 记录环境阻碍；公共接口变化回外层；所有片完成后仍需跨模块集成、回归、审查与交付。

每片最小记录：目标、非目标、接口/资源边界、验收输入和预期、Host/板级测试环境、构建 Target、实际结果（PASS/FAIL/BLOCKED）、证据及未测项。**编译成功 ≠ Host 测试通过 ≠ 板级测试通过 ≠ 系统回归完成**。

## 当前已有的 5 个 Skill

| Skill | 职责 |
| --- | --- |
| [embedded-resource-acquisition](../skills/embedded-resource-acquisition/SKILL.md) | 技术资料盘点、来源与获取 |
| [extracting-chip-hardware-facts](../skills/extracting-chip-hardware-facts/SKILL.md) | 芯片/板级相关事实及使用限制 |
| [embedded-code-reader](../skills/embedded-code-reader/SKILL.md) | 现有固件代码与实际调用、数据/状态流 |
| [embedded-design-decision](../skills/embedded-design-decision/SKILL.md) | 项目与模块的软件设计决策 |
| [embedded-code-development](../skills/embedded-code-development/SKILL.md) | 已明确需求的代码实现、修改和移植 |

## 未来候选 Skill 路线图（以下全部**尚未开发**）

| 候选名称 | 单一职责 | 初步优先级 |
| --- | --- | --- |
| embedded-task-decomposition | 设计/需求 → 可独立验收的切片、前后依赖与状态；不重新做架构 | 高 |
| embedded-test-verification | 独立执行和核验单元/板级/回归测试证据；不把未执行结果当 PASS | 高 |
| embedded-code-review | 对照 Spec、Standards、Evidence 审查代码差异；不直接代替实现 | 高 |
| embedded-debugging | 根因未知时复现、缩小、假设、定向观测和回归，给出可核对根因 | 按需 |
| embedded-requirements-clarification | 对复杂/歧义需求定向追问、输出可验收条件；不替代设计 | 按需 |
| embedded-work-handoff | 跨会话/跨任务保留代码基线、切片状态和待确认项 | 最后考虑 |

**候选不是现有能力，也不是必须开发的清单。** 每项开发前先验证现有 Skill 是否能满足任务、搜集稳定失败基线并按 [AGENTS.md](../AGENTS.md) 完成正向、边界和误触发测试。构建、烧录、调试仍沿用真实项目已有工具，不假定此仓库已有 Build/Debug/Test Skill。候选先以工程复用价值、清晰边界和可验证性决定是否开发。

参考：[Matt Pocock — Skills For Real Engineers](https://github.com/mattpocock/skills) 的需求澄清、Tracer Bullet、TDD 反馈与独立审查，以及 [本仓库的设计决策框架](../skills/embedded-design-decision/references/decision-framework.md)。
