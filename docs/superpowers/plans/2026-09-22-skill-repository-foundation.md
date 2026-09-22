# 嵌入式 Skill 仓库基础设施实施计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 创建可提交、可验证且不包含具体 Skill 的 `Embedded_Engineering_SKILLS` 仓库基础框架。

**Architecture:** 使用四个根级治理文件定义仓库用途、安全边界和演进规则，使用 `skills/`、`docs/`、`tests/`、`tools/` 分隔正式 Skill、设计资料、验证材料和共享辅助工具。空目录通过 `.gitkeep` 纳入版本控制，真实内容出现后移除对应占位文件。

**Tech Stack:** Markdown、Git、PowerShell、MIT License

**Spec:** `docs/superpowers/specs/2026-09-22-skill-repository-foundation-design.md`

## Global Constraints

- 本阶段仅初始化仓库治理和目录框架，不创建任何具体 Skill。
- `skills/` 是正式 Skill 的唯一源码位置。
- 通用嵌入式源码资产归入 `Embedded_Engineering_Library`。
- 通用构建、烧录、调试和通信工具实现归入 `Embedded_Engineering_TOOLS`。
- 仓库文件不得包含密钥、隐私、公司内部材料或本机绑定绝对路径。
- 每个 Skill 必须独立设计和验证；S01 不属于本计划。
- 不提交或推送，除非用户另行明确要求。

## Review Focus

- 仓库边界：README 和 AGENTS 必须对 SKILLS、Library、TOOLS 的归属给出一致结论。
- 空目录跟踪：`skills/`、`tests/`、`tools/` 必须包含 `.gitkeep`，`docs/` 已由设计与计划文件保持非空。
- 范围控制：`skills/` 下不得提前出现 `embedded-design-decision` 或其他具体 Skill。
- 路径可移植性：仓库文件不得出现 `E:\`、`C:\` 或用户目录硬编码。
- 许可证边界：MIT 仅覆盖仓库原创内容，不覆盖未来引入的第三方材料。

---

### Task 1: 创建仓库治理文件和基础目录

**Files:**

- Create: `README.md`
- Create: `AGENTS.md`
- Create: `LICENSE`
- Create: `CHANGELOG.md`
- Create: `skills/.gitkeep`
- Create: `tests/.gitkeep`
- Create: `tools/.gitkeep`
- Verify: `docs/superpowers/specs/2026-09-22-skill-repository-foundation-design.md`
- Verify: `docs/superpowers/plans/2026-09-22-skill-repository-foundation.md`

**Interfaces:**

- Consumes: 已确认的 S00 设计说明。
- Produces: 后续 S01 可以直接添加 `skills/<skill-name>/SKILL.md` 的受治理仓库框架。

- [ ] **Step 1: 运行初始化前基线检查**

Run:

```powershell
$requiredRootFiles = @('README.md', 'AGENTS.md', 'LICENSE', 'CHANGELOG.md')
$unexpected = $requiredRootFiles | Where-Object { Test-Path -LiteralPath $_ }
if ($unexpected) { throw "根文件已存在，停止覆盖：$($unexpected -join ', ')" }
if (Get-ChildItem -LiteralPath 'skills' -Directory -ErrorAction SilentlyContinue) {
    throw 'skills/ 中已存在具体 Skill，超出 S00 范围'
}
```

Expected: 命令无输出并以退出码 `0` 结束；若目标文件已存在则停止，不覆盖未知内容。

- [ ] **Step 2: 创建 `README.md`**

文件必须按以下顺序包含：

```markdown
# Embedded Engineering Skills

面向 Codex/Agent 的嵌入式工程工作流与决策 Skill 源码仓库。

## 仓库定位
- 保存可重复使用的嵌入式 AI 工作流和工程决策 Skill。
- 不保存通用源码资产或通用工具实现。

## 仓库边界
- `Embedded_Engineering_SKILLS`：Agent Skill 及其测试、设计和仓库级辅助材料。
- `Embedded_Engineering_Library`：可跨项目复用的嵌入式源码资产。
- `Embedded_Engineering_TOOLS`：构建、烧录、调试和通信等通用工具实现。

## 目录结构

```text
.
├── skills/   # 正式 Skill 的唯一源码
├── docs/     # 仓库治理、设计说明和开发记录
├── tests/    # Skill 触发范围与行为回归材料
└── tools/    # 多个 Skill 共用的仓库级辅助工具
```

## 开发流程

1. 明确任务边界并完成设计。
2. 在没有目标 Skill 的条件下运行场景，记录失败基线。
3. 创建最小 Skill，使基线场景通过。
4. 验证显式调用、隐式触发、误触发边界和核心行为。
5. 完成一个 Skill 的验收后，再开始下一个 Skill。

## 本地加载

本仓库中的 `skills/` 是唯一源码。进入触发测试阶段后，将单个
`skills/<skill-name>` 目录链接到用户级 `.agents/skills/<skill-name>`；
不要把整个用户级 Skill 目录指向本仓库。

## 许可证
原创内容采用 MIT License；第三方材料遵循自身许可证。
```

- [ ] **Step 3: 创建 `AGENTS.md`**

文件使用中文，并明确以下强制规则：

```markdown
# AGENTS.md

## 基础行为
- 默认使用中文。
- 只处理用户明确要求的范围；不主动创建额外 Skill。
- 修改前阅读根 README、目标 Skill 的 SKILL.md 及相关测试。
- 无法验证的信息标记为未验证，不得猜测。

## 仓库边界
- 本仓库只保存嵌入式 AI 工作流和工程决策 Skill。
- 通用源码资产归 Library；通用工具实现归 TOOLS。

## Skill 结构
- 每个 Skill 使用 `skills/<skill-name>/` 独立目录。
- `SKILL.md` 为必需入口；仅按需创建 scripts、references、assets、agents。
- 禁止硬编码本机绝对路径。

## 开发与验证
- 一个 Skill 对应明确、可重复的任务或决策过程。
- 创建或修改 Skill 前先建立失败基线。
- 修改后验证显式调用、隐式触发、误触发边界和核心行为。
- 每次只开发和验收一个 Skill。

## 安全与交付
- 禁止密钥、隐私、公司内部代码和其他保密材料。
- 删除、覆盖、大规模移动、提交、推送和发布需要明确授权。
- 交付时报告实际改动、验证结果和未验证项。
```

- [ ] **Step 4: 创建 `LICENSE` 和 `CHANGELOG.md`**

`LICENSE` 使用标准 MIT License，版权行为：

```text
Copyright (c) 2026 Embedded Engineering Skills contributors
```

`CHANGELOG.md` 使用 Keep a Changelog 的精简结构：

```markdown
# Changelog

本文件记录正式发布及仓库级重要变化，不记录日常开发流水账。

格式参考 Keep a Changelog，版本遵循 Semantic Versioning。

## [Unreleased]

### Added

- 初始化仓库治理文件与基础目录。
```

- [ ] **Step 5: 创建可跟踪的基础目录**

创建三个空占位文件：

```text
skills/.gitkeep
tests/.gitkeep
tools/.gitkeep
```

不在 `docs/` 创建 `.gitkeep`，因为设计和计划文件已经使该目录可被 Git 跟踪。

- [ ] **Step 6: 验证目录、范围和关键内容**

Run:

```powershell
$required = @(
    'README.md',
    'AGENTS.md',
    'LICENSE',
    'CHANGELOG.md',
    'skills/.gitkeep',
    'tests/.gitkeep',
    'tools/.gitkeep',
    'docs/superpowers/specs/2026-09-22-skill-repository-foundation-design.md',
    'docs/superpowers/plans/2026-09-22-skill-repository-foundation.md'
)
$missing = $required | Where-Object { -not (Test-Path -LiteralPath $_) }
if ($missing) { throw "缺少文件：$($missing -join ', ')" }

$skillDirectories = Get-ChildItem -LiteralPath 'skills' -Directory
if ($skillDirectories) { throw "S00 不应包含具体 Skill：$($skillDirectories.Name -join ', ')" }

$repoText = Get-ChildItem -Recurse -File -Include '*.md','LICENSE' |
    ForEach-Object { Get-Content -LiteralPath $_.FullName -Raw -Encoding UTF8 }
if ($repoText -match '[A-Za-z]:\\') { throw '仓库文档包含本机绝对路径' }

$readme = Get-Content -LiteralPath 'README.md' -Raw -Encoding UTF8
foreach ($term in @('Embedded_Engineering_SKILLS', 'Embedded_Engineering_Library', 'Embedded_Engineering_TOOLS')) {
    if ($readme -notmatch [regex]::Escape($term)) { throw "README 缺少仓库边界：$term" }
}
```

Expected: 命令无输出并以退出码 `0` 结束。

- [ ] **Step 7: 执行 Git 格式与范围检查**

Run:

```powershell
git diff --check
git status --short
```

Expected:

- `git diff --check` 无输出。
- `git status --short` 只显示 S00 根文件、三个 `.gitkeep`、设计说明和实施计划。
- 不出现具体 Skill、构建产物、临时文件或本机配置。

- [ ] **Step 8: 交付但不提交**

汇报新增文件、验证命令和结果。保留工作区未提交状态；仅在用户明确要求后执行 `git add`、`git commit` 或推送。
