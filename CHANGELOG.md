# Changelog

本文件记录正式发布及仓库级重要变化，不记录日常开发流水账。

格式参考 [Keep a Changelog](https://keepachangelog.com/zh-CN/1.1.0/)，版本遵循 [Semantic Versioning](https://semver.org/lang/zh-CN/)。

## [Unreleased]

### Added

- 初始化仓库治理文件与基础目录。

## embedded-design-decision / embedded-code-development — v1.0.2 — 2026-10-15

### Changed

- 两个 Skill 的 `metadata.version` 更新为 `1.0.2`。
- 增加按风险裁剪的鲁棒性原则，包括异常边界、有界恢复、非幂等重试限制及失败证据。
- 初始设计纳入必要的局部验证输入、预期结果、环境和通过条件，由开发阶段落实单元测试和模块自测；复杂测试方案独立设计。
- 明确物理操作的软件入口、统一业务路径、注入层级与硬件覆盖边界，按需复用接口，不强制建设 CLI。
- 验证采用简单人工场景走查、结构及差异检查；真实触发、固件构建与板级行为未验证。

## embedded-design-decision / embedded-code-development — v1.0.1 — 2026-10-09

### Changed

- 两个 Skill 在 `metadata.version` 中分别记录版本 `1.0.1`。
- 设计 Skill 增加按需可控制性、可观测性、可复位性、可替换性与测试构建隔离决策，保持独立测试设计边界和最低必要复杂度。
- 开发 Skill 增加已批准测试接口实施、异步完成反馈、受控恢复与 Release 产物排除检查要求。
- 各补充三个简单回归场景；本次采用人工场景走查与结构校验，不宣称完成真实触发、固件构建或板级验证。
