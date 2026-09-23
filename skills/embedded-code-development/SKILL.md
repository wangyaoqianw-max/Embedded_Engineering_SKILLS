---
name: embedded-code-development
description: Use when implementing, modifying, integrating, or porting code in an existing embedded firmware project, including code work in a fresh CubeMX-generated project that needs Keil project initialization and a pre-edit build baseline. Not for source-only analysis, architecture design, standalone build, unknown runtime debugging, independent testing, or code review.
---

# 嵌入式代码开发

把已明确的功能要求、缺陷修复或移植任务落实到已有嵌入式工程。覆盖 Implement、Modify 和 Port 三种模式；先服从目标工程，再做满足任务所需的最小改动。

## 适用边界

- Use when：新增或修改驱动、模块、Service、Platform、Application；接入已有组件；同步源码、配置和工程文件；修复本次改动引起的编译或链接问题。
- Do not use when：只读理解源码（转 embedded-code-reader）；需求或架构未定（转 embedded-design-decision）；只构建工程（转匹配的 GCC、Keil 或 EIDE Skill）；根因未知的运行故障（转 Debug）；只要求测试或审查（转 Test 或 Code Review）。
- 关键接口、资源所有权、目标平台或功能行为不明确且会改变实现时，先询问或回到设计阶段，不用代码选择代替缺失决策。

## 开发流程

1. 阅读适用的 AGENTS.md、README、设计与构建规范，检查相关源码、调用方、配置、工程文件和当前变更。只扩展到支撑本次任务的范围。
2. 按当前用户要求、目标工程事实与规范、已确认设计、可访问的项目代码规范、Skill 通用建议的顺序解决冲突。C 代码规范优先查 Embedded_Engineering_Library 中的唯一维护版本；不复制到本 Skill。
3. 对刚由 CubeMX 生成且已有 Keil 工程文件的项目，按 [Keil 工程接入与构建基线](references/project-build-integration.md) 检查并初始化本地规范要求的 Keil 配置，再在修改应用代码前建立首次构建基线。此 Skill 负责工程接入配置；不从空目录创建 Keil 工程，也不猜测芯片、Target、链接布局或工具链。
4. 实施 Implement 或 Modify 时保持既有架构、接口、资源所有权和生成代码约定。新增、移动或改名源码时同步相关 Keil、CMake、Make、EIDE 或其他工程输入。
5. Port 模式按需读取 [代码移植与第三方接入](references/code-porting.md)，先确认来源、版本、许可、依赖和适配边界。
6. 修改完成后调用目标工程实际采用的构建 Skill 或项目指定构建入口。构建 Skill 负责执行构建；本 Skill 负责准备工程输入、判断构建时机、比较基线并修复本次代码或接入造成的问题。
7. 按用户要求和项目流程验证。报告改动范围、工程文件同步、构建 Target 与结果、已执行测试，以及未执行或无法确认的验证。构建通过不代表硬件行为已验证。

## 变更与协作边界

- 不因单个工程经验重设目录结构、输出路径、工具链或全局配置；目标工程的规范决定这些值。
- 不顺带重构无关代码，也不未经确认改变公共 API、共享数据结构或跨层架构。
- 对同一文件、紧耦合模块、共享状态机或正在设计的接口保持单一写入者。只有在至少两个工作流相互独立且并行能明显节省时间或提高质量时，才考虑子代理。
- 主代理统一决定架构边界、公共 API、跨层接口、共享数据和全局工程配置，并负责检查整合后的接口、工程文件、构建结果、测试状态和文档一致性。
- 构建期间不得并行修改参与构建的源码或配置，也不得清理构建输出。
