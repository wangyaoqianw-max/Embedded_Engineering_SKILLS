---
name: embedded-code-development
metadata:
  version: "1.0.2"
description: Use when implementing, modifying, integrating, or porting code in an existing embedded firmware project, including code work in a fresh CubeMX-generated project that needs Keil project initialization and a pre-edit build baseline. Not for source-only analysis, architecture design, standalone build, unknown runtime debugging, independent testing, or code review.
---

# 嵌入式代码开发

把已明确的功能要求、缺陷修复或移植任务落实到已有嵌入式工程。覆盖 Implement、Modify 和 Port 三种模式；先服从目标工程，再做满足任务所需的最小改动。

## 适用边界

- Use when：新增或修改驱动、模块、Service、Platform、Application；接入已有组件；同步源码、配置和工程文件；修复本次改动引起的编译或链接问题。
- Do not use when：只读理解源码（转 embedded-code-reader）；需求或架构未定（转 embedded-design-decision）；只构建工程（转匹配的 GCC、Keil 或 EIDE Skill）；根因未知的运行故障（转 Debug）；只要求测试或审查（转 Test 或 Code Review）。
- 关键接口、资源所有权、目标平台或功能行为不明确且会改变实现时，先询问或回到设计阶段，不用代码选择代替缺失决策。

## 开发流程

开始定位或准备修改目标代码前，先检查目标工程根目录是否已有 `.codegraph/`。若已有，在源码搜索、阅读或编辑前优先用可用的 `codegraph_explore` 查询相关符号、调用方、依赖与影响范围，并将目标工程路径传给 MCP 的 `projectPath` 参数；MCP 工具不可用时，在目标工程目录使用 `codegraph explore`。用返回源码和关系结果缩小检查范围，并核对实施所需的项目配置与工程文件；只补读 CodeGraph 未覆盖的必要材料。若无索引，跳过 CodeGraph 并按现有流程探索；不要运行 `codegraph init` 或创建索引。CodeGraph 不能替代构建和任务要求的验证。

1. 阅读适用的 AGENTS.md、README、设计与构建规范，检查相关源码、调用方、配置、工程文件和当前变更。只扩展到支撑本次任务的范围。
2. 按当前用户要求、目标工程事实与规范、已确认设计、可访问的项目代码规范、Skill 通用建议的顺序解决冲突。C 代码规范优先查 Embedded_Engineering_Library 中的唯一维护版本；不复制到本 Skill。
3. 对刚由 CubeMX 生成且已有 Keil 工程文件的项目，按 [Keil 工程接入与构建基线](references/project-build-integration.md) 检查并初始化本地规范要求的 Keil 配置，再在修改应用代码前建立首次构建基线。此 Skill 负责工程接入配置；不从空目录创建 Keil 工程，也不猜测芯片、Target、链接布局或工具链。
4. 实施 Implement 或 Modify 时保持既有架构、接口、资源所有权和生成代码约定。新增、移动或改名源码时同步相关 Keil、CMake、Make、EIDE 或其他工程输入。
5. Port 模式按需读取 [代码移植与第三方接入](references/code-porting.md)，先确认来源、版本、许可、依赖和适配边界。
6. 修改完成后调用目标工程实际采用的构建 Skill 或项目指定构建入口。构建 Skill 负责执行构建；本 Skill 负责准备工程输入、判断构建时机、比较基线并修复本次代码或接入造成的问题。
7. 按用户要求和项目流程验证。报告改动范围、工程文件同步、构建 Target 与结果、已执行测试，以及未执行或无法确认的验证。构建通过不代表硬件行为已验证。

## 可测试性接口实施

- 落实已批准的测试接口和验收约束；需求会改变公共 API 或架构且未确定时，先回到设计阶段。简单功能不自动增加 CLI、Mock 或额外抽象。
- 测试适配与业务实现分离，复用正式业务 API、已有平台边界和诊断设施；保持 owner task/ISR 约定，不复制状态机，不绕过认证、保护或状态检查。
- 对已批准的物理操作软件入口，落实事件或业务 API 的注入层级，让物理输入和测试输入汇入同一业务路径；保留硬件链路的独立验证，不自动建设 CLI。
- 按设计落实输入/状态边界、超时、有界重试、隔离与恢复；非幂等操作结果未知时不能盲目重试，不静默丢错或返回假成功。缺少会改变行为的恢复决策时先澄清。
- 异步接口明确已接受、已完成和失败的响应，关联请求与最终结果；队列入队成功不作为业务完成证据。按设计实现受控状态恢复，处理未完成请求，避免清除真实业务数据。
- 测试专用源码、命令注册和配置按现有 Target/Profile 管理，默认从 Release 排除；数值功能开关使用 `#if`。必要产品诊断按需求保留，不强制新增构建类型。
- 按初始方案的局部验证设计实现并执行与改动相称的单元测试、模块自测和关键异常路径检查，核对输入、预期结果及通过条件；复杂系统测试的完整方案独立设计，不因测试被称为白盒或灰盒就扩大本次任务。
- 执行相关构建；有测试专用功能时检查 Release ELF/MAP 等产物的排除情况。报告已执行项及未验证项，不用诊断日志代替确定性结果，也不把软件注入当作硬件或 EMC 验证。

## 变更与协作边界

- 不因单个工程经验重设目录结构、输出路径、工具链或全局配置；目标工程的规范决定这些值。
- 不顺带重构无关代码，也不未经确认改变公共 API、共享数据结构或跨层架构。
- 对同一文件、紧耦合模块、共享状态机或正在设计的接口保持单一写入者。只有在至少两个工作流相互独立且并行能明显节省时间或提高质量时，才考虑子代理。
- 主代理统一决定架构边界、公共 API、跨层接口、共享数据和全局工程配置，并负责检查整合后的接口、工程文件、构建结果、测试状态和文档一致性。
- 构建期间不得并行修改参与构建的源码或配置，也不得清理构建输出。
