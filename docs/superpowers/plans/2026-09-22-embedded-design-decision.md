# Embedded Design Decision Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 创建并验证 `embedded-design-decision` Skill，使其能把嵌入式编码前设计决策写入用户指定目录下的 Markdown 文档。

**Architecture:** 使用一个精简 `SKILL.md` 作为触发、流程和资料路由入口，以 `decision-framework.md` 保存稳定决策规则，以 `engineering-experience.md` 接收尚未成熟的工程经验。三个现有长报告只在相关问题出现时读取；仓库级测试材料记录无 Skill 基线和可观察的行为回归结果。

**Tech Stack:** Codex Skill Markdown、YAML、PowerShell、Codex CLI 0.151.0、`skill-creator` 的 `quick_validate.py`。

**Spec:** `docs/superpowers/specs/2026-09-22-embedded-design-decision-design.md`

## Global Constraints

- Skill 名称和目录固定为 `embedded-design-decision`。
- 用户未指定输出目录时必须先询问，不能自行选择默认目录。
- 最终交付必须是用户指定目录下的一份 Markdown 设计文档。
- 设计完成后停止，不修改业务代码、构建配置或工程结构。
- V0.1 不创建脚本、资产、模板目录或新的功能范围。
- 保留三个现有参考文件的内容和文件名，不做顺手整理或重写。
- 单项目经验先进入候选池，不能直接形成全局规则。
- 不在 Skill 或测试材料中写入本机绝对路径；计划中的本机命令仅用于本次开发验证。
- 创建用户级 Skill junction 属于仓库外写入，执行前必须获得用户明确授权，并先检查目标是否已存在。
- 未获得用户明确授权前不得执行 `git commit`；各任务中的提交步骤只是授权后的检查点。

## Review Focus

- 用户给出文件路径而不是目录时，应识别冲突并确认，不能把路径当目录创建。
- 输出目录存在但目标文件同名时，应暂停确认或选用明确的新文件名，不能覆盖。
- 一个请求同时涉及 Application 与 Bootloader 时，应分别选择复杂度，不能使用单一全局等级。
- 已有工程资料与通用参考结论冲突时，应采用可验证的项目事实并记录冲突。
- 候选工程经验与稳定框架冲突时，只能列为参考或待验证项，不能覆盖稳定规则。

---

## File Map

### Create

- `skills/embedded-design-decision/SKILL.md`：触发边界、前置条件、决策工作流、资料路由、设计文档合同和停止条件。
- `skills/embedded-design-decision/agents/openai.yaml`：UI 名称、短描述和默认调用提示。
- `skills/embedded-design-decision/references/decision-framework.md`：范围分类、架构驱动、复杂度等级、边界、运行模型、抽象证据、过度设计检查和路线排序规则。
- `skills/embedded-design-decision/references/engineering-experience.md`：工程经验的准入字段、状态和升级/废弃机制。
- `tests/embedded-design-decision/baseline.md`：无目标 Skill 时六个场景的实际结果和观察到的缺口。
- `tests/embedded-design-decision/regression-cases.md`：显式、隐式、误触发和六个核心行为场景的输入与断言。

### Preserve Unchanged

- `skills/embedded-design-decision/references/设计模式_GoF_嵌入式软件蒸馏.md`
- `skills/embedded-design-decision/references/bootloader-ota-deep-research-report.md`
- `skills/embedded-design-decision/references/freertos-deep-research-report.md`

### Development-only external state

- `%USERPROFILE%\.agents\skills\embedded-design-decision`：经用户批准后创建、指向仓库 Skill 源码的 junction，用于真实显式/隐式触发测试。
- `%TEMP%\embedded-design-decision-<GUID>`：每轮行为测试的隔离工程和输出目录，不进入 Git。

---

### Task 1: 建立无 Skill 失败基线和回归合同

**Files:**
- Create: `tests/embedded-design-decision/baseline.md`
- Create: `tests/embedded-design-decision/regression-cases.md`
- Read: `docs/superpowers/specs/2026-09-22-embedded-design-decision-design.md`

**Interfaces:**
- Consumes: 规格中的六个核心场景、触发边界和设计文档合同。
- Produces: Task 2 必须满足的行为断言，以及 Task 3 使用的完整测试提示词。

- [ ] **Step 1: 确认基线环境中尚无可发现的目标 Skill**

运行：

```powershell
$candidatePaths = @(
  (Join-Path $env:USERPROFILE '.agents\skills\embedded-design-decision'),
  (Join-Path $env:USERPROFILE '.codex\skills\embedded-design-decision')
)
$candidatePaths | ForEach-Object {
  [pscustomobject]@{ Path = $_; Exists = Test-Path -LiteralPath $_ }
}
```

预期：两个位置都不存在。如果任一位置存在，停止基线任务，报告实际路径；不能删除、移动或暂时隐藏现有 Skill。

- [ ] **Step 2: 创建隔离的基线工程目录**

运行：

```powershell
$baselineRoot = Join-Path ([System.IO.Path]::GetTempPath()) ('embedded-design-decision-baseline-' + [guid]::NewGuid())
New-Item -ItemType Directory -Path $baselineRoot | Out-Null
$baselineRoot
```

预期：输出一个新的绝对目录；目录名包含随机 GUID，不影响已有文件。

- [ ] **Step 3: 在 `regression-cases.md` 写入六个核心场景的输入和稳定断言**

文件必须使用以下结构和断言，不把固定标题或措辞当作测试条件：

```markdown
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
```

- [ ] **Step 4: 使用未安装目标 Skill 的新 Codex 会话运行六个基线场景**

运行以下完整命令：

```powershell
$baselinePrompts = @(
  '为资源受限 MCU 上的一个按键控制 LED 功能做编码前设计。',
  '设计 UART DMA 接收、RingBuffer 和 FreeRTOS 消费任务。',
  '设计只负责校验并跳转应用的 STM32F4 Bootloader。',
  '设计带 OTA、A/B 分区和回滚的产品升级方案。',
  '现有产品从 F411/W25Q64 迁移到 F407/W25Q128，要求尽量不重写 OTA 服务，请做编码前设计。',
  '公司已有架构，现在只新增 Sensor Module，请做模块编码前设计。'
)
for ($i = 0; $i -lt $baselinePrompts.Count; $i++) {
  $caseNo = '{0:D2}' -f ($i + 1)
  $caseRoot = Join-Path $baselineRoot "case-$caseNo"
  New-Item -ItemType Directory -Path (Join-Path $caseRoot 'output') -Force | Out-Null
  $documentPath = Join-Path $caseRoot "output\case-$caseNo.md"
  $finalPath = Join-Path $caseRoot "case-$caseNo-final.txt"
  $prompt = $baselinePrompts[$i] + " 设计文档必须写入：$documentPath"
  codex exec --ephemeral --skip-git-repo-check -s workspace-write -C $caseRoot -o $finalPath $prompt
}
```

预期：六条命令成功返回，且每个会话只能看到自己的空白隔离工程；结果可能满足部分要求，也可能暴露过度设计、范围越权、所有权不清或未落盘等缺口。不得为了制造 RED 而篡改或误述实际结果。

- [ ] **Step 5: 将实际基线写入 `baseline.md`**

使用以下固定结构，每个场景填写 Codex 的实际行为、已满足断言、未满足断言、临时证据文件名和能够支持判断的简短摘录；不得保留空字段，不把临时目录的本机绝对路径写入仓库：

```markdown
# Embedded Design Decision Baseline

## 环境

- Codex CLI：运行 `codex --version` 得到的实际版本
- 目标 Skill：未安装且仓库内尚无 `SKILL.md`
- 执行方式：独立临时工程、ephemeral session

## 汇总

| 场景 | 文档落盘 | 关键决策满足情况 | 观察到的缺口 |
| --- | --- | --- | --- |

## CASE-01 至 CASE-06

每节记录：输入、实际行为、满足项、缺口、临时证据文件名和关键证据摘录。

## 基线结论

仅总结实际重复出现或高风险的决策缺口，不预设 Skill 一定优于基线。
```

- [ ] **Step 6: 核验基线材料没有伪造结论或未完成占位符**

运行：

```powershell
Select-String -LiteralPath '.\tests\embedded-design-decision\baseline.md','.\tests\embedded-design-decision\regression-cases.md' -Pattern 'TBD|TODO|待补充'
```

预期：无输出。

- [ ] **Step 7: 授权后提交基线检查点**

仅在用户明确授权提交后运行：

```powershell
git add -- 'tests/embedded-design-decision/baseline.md' 'tests/embedded-design-decision/regression-cases.md'
git commit -m 'test: establish embedded design decision baseline'
```

否则记录“未提交：没有 Git 提交授权”，继续下一任务。

---

### Task 2: 实现最小 Skill 决策内核

**Files:**
- Create: `skills/embedded-design-decision/SKILL.md`
- Create: `skills/embedded-design-decision/agents/openai.yaml`
- Create: `skills/embedded-design-decision/references/decision-framework.md`
- Create: `skills/embedded-design-decision/references/engineering-experience.md`
- Preserve: `skills/embedded-design-decision/references/设计模式_GoF_嵌入式软件蒸馏.md`
- Preserve: `skills/embedded-design-decision/references/bootloader-ota-deep-research-report.md`
- Preserve: `skills/embedded-design-decision/references/freertos-deep-research-report.md`
- Test: `tests/embedded-design-decision/regression-cases.md`

**Interfaces:**
- Consumes: Task 1 的稳定断言和规格中的设计合同。
- Produces: 可被 Codex 自动发现的 Skill、稳定决策框架和经验准入机制。

- [ ] **Step 1: 写入能够区分触发与误触发的 `SKILL.md` frontmatter**

frontmatter 使用以下内容：

```yaml
---
name: embedded-design-decision
description: 在嵌入式产品或模块编码前，根据项目范围、现有工程、硬件资源、实时性、可靠性和演化约束生成工程设计决策文档。用于架构复杂度、模块边界、依赖方向、运行模型、资源所有权、关键接口和实施路线设计；不用于直接编码、构建、烧录、调试、测试执行或纯概念解释。
---
```

预期：名称与目录一致；描述同时覆盖正向场景和主要误触发边界。

- [ ] **Step 2: 写入 `SKILL.md` 的最小执行流程**

正文必须按以下顺序表达行为，不复制三个长报告：

```markdown
# Embedded Design Decision

## 前置条件

要求用户指定设计文档输出目录。未指定时只询问目录；目录与文件路径不清时先澄清。检查目标目录的现有文档和命名规范，同名文件不得直接覆盖。

## 工作流

1. 读取需求、现有工程、架构文档和项目规则。
2. 将输入区分为已验证事实、假设和未知项；会改变架构的未知项先询问。
3. 读取 `references/decision-framework.md`，分类 Enterprise Product、Personal Project 或 Existing Project Module。
4. 识别架构驱动因素，选择满足已知约束的最低复杂度。
5. 设计模块边界、依赖方向、运行模型、资源所有权和关键接口。
6. 检查过度设计与设计不足，明确不采用的方案及理由。
7. 按依赖关系生成分阶段实施路线。
8. 把完整 Markdown 设计文档写入用户指定目录并回读核验。

## 参考资料路由

- 每次读取 `references/decision-framework.md`。
- 涉及抽象、变化隔离、接口或设计模式时读取 GoF 蒸馏。
- 涉及 Task、ISR、IPC、实时性、内存或并发时读取 FreeRTOS 报告。
- 涉及启动、升级、分区、回滚或镜像安全时读取 Bootloader/OTA 报告。
- 存在相似工程场景时读取 `references/engineering-experience.md`；候选经验不能覆盖稳定规则。
- 不无条件加载全部参考文件。项目可验证事实优先于通用资料。

## 文档合同

设计文档按任务规模调整深度，但覆盖背景与目标、任务范围、事实/假设/未知项、关键约束、架构驱动、复杂度及依据、模块与依赖、运行模型与所有权、关键接口、资源规划、可靠性、拒绝方案、风险与验证项、实施路线。

## 停止边界

文档写入并核验后停止。不得继续修改业务代码、构建配置或工程结构，不执行构建、烧录、调试或测试。
```

- [ ] **Step 3: 写入 `decision-framework.md`**

文件应使用以下一级结构，并把每项写成决策规则而非背景教程：

```markdown
# Embedded Design Decision Framework

## 证据顺序
项目事实 > 明确需求 > 已验证稳定规则 > 候选工程经验 > 通用示例。

## 范围分类
- Enterprise Product：完整产品、长期维护、多型号或多人协作。
- Personal Project：个人完整项目，保留必要边界但不照搬企业治理。
- Existing Project Module：服从现有架构，只设计模块及接入边界。

## 架构驱动因素
只保留会改变设计的功能边界、现有架构、资源预算、实时与并发、可靠性、移植复用、预期变化、规模和生命周期。

## 复杂度等级
- L0 直接实现
- L1 模块化
- L2 分层与服务化
- L3 产品平台化

说明每级的进入证据、典型边界和常见过度设计信号。强调复杂度按子系统判断，Application 与 Bootloader 可以不同。

## 模块和依赖
每个模块回答职责、输入输出、依赖、状态、生命周期和失败传播；依赖指向稳定策略，不为形式增加层。

## 运行模型和所有权
先判断独立 deadline、阻塞域、ISR 边界和共享资源，再决定主循环、状态机、DMA、Task、队列或锁。每个外设和可变状态都要有明确所有者。

## 抽象证据
引入抽象需要真实变化、传播成本和收益大于代价；证据不足时保持简单。

## 过度设计与设计不足
分别列出可观察信号，并要求对每项新增机制说明它解决的当前约束。

## 实施路线
按事实确认、工程基线、诊断能力、平台边界、核心服务、业务功能、故障路径和系统验证的依赖顺序裁剪，不能把固定阶段机械套给所有项目。

## 文档自检
检查事实与假设、局部复杂度、所有权、失败路径、拒绝方案、验证项和停止边界。
```

不得把 FreeRTOS API、23 个 GoF 模式或 OTA 细节重复进该文件；只保留能够改变选择的判断规则。

- [ ] **Step 4: 写入 `engineering-experience.md`**

文件必须定义候选经验的字段和状态，不虚构首条工程经验：

```markdown
# Engineering Experience

本文件保存尚未升级为稳定规则的工程经验。经验只在上下文相似时作为参考，不能覆盖项目事实或 `decision-framework.md`。

## 记录字段

- ID：`EXP-NNN`
- 标题
- 来源场景
- 观察到的问题
- 采用的决策
- 实际结果
- 适用条件
- 不适用条件
- 证据
- 状态：候选、已验证、已废弃

## 状态转换

- 新经验进入“候选”。
- 能形成可复现场景时加入回归案例。
- 多次验证且适用边界明确后，提炼到 `decision-framework.md` 并标为“已验证”。
- 被反例推翻或由新事实取代时标为“已废弃”，保留原因。

## 当前记录

当前没有已录入的工程经验。
```

- [ ] **Step 5: 写入 `agents/openai.yaml`**

使用以下完整内容：

```yaml
interface:
  display_name: "Embedded Design Decision"
  short_description: "为嵌入式产品或模块生成编码前工程设计决策文档并规划实施路线"
  default_prompt: "使用 $embedded-design-decision 分析这个嵌入式设计任务，并将设计文档写入我指定的目录。"
```

不增加图标、品牌色、MCP 依赖或显式调用限制。

- [ ] **Step 6: 运行结构验证并确认预期 RED/GREEN 边界**

运行：

```powershell
$validator = Join-Path $skillCreatorRoot 'scripts\quick_validate.py'
python -X utf8 $validator '.\skills\embedded-design-decision'
```

预期：`Skill is valid!`

然后运行：

```powershell
Get-ChildItem -LiteralPath '.\skills\embedded-design-decision' -Recurse -File | Select-Object FullName,Length
Select-String -LiteralPath '.\skills\embedded-design-decision\SKILL.md','.\skills\embedded-design-decision\references\decision-framework.md','.\skills\embedded-design-decision\references\engineering-experience.md' -Pattern 'TBD|TODO|待补充'
```

预期：出现四个新文件和三个保留参考文件；占位符搜索无输出。

- [ ] **Step 7: 确认三个现有参考文件未被修改**

运行：

```powershell
git diff --numstat -- `
  'skills/embedded-design-decision/references/设计模式_GoF_嵌入式软件蒸馏.md' `
  'skills/embedded-design-decision/references/bootloader-ota-deep-research-report.md' `
  'skills/embedded-design-decision/references/freertos-deep-research-report.md'
```

预期：无输出。

- [ ] **Step 8: 授权后提交最小 Skill 检查点**

仅在用户明确授权提交后运行：

```powershell
git add -- 'skills/embedded-design-decision/SKILL.md' 'skills/embedded-design-decision/agents/openai.yaml' 'skills/embedded-design-decision/references/decision-framework.md' 'skills/embedded-design-decision/references/engineering-experience.md'
git commit -m 'feat: add embedded design decision skill'
```

否则记录“未提交：没有 Git 提交授权”，继续下一任务。

---

### Task 3: 安装开发链接并执行真实行为回归

**Files:**
- Read: `skills/embedded-design-decision/SKILL.md`
- Read: `tests/embedded-design-decision/regression-cases.md`
- Modify: `tests/embedded-design-decision/regression-cases.md`
- External create after approval: `%USERPROFILE%\.agents\skills\embedded-design-decision` junction

**Interfaces:**
- Consumes: Task 2 的可验证 Skill 和 Task 1 的场景合同。
- Produces: 显式触发、隐式触发、误触发、输出落盘和核心决策行为的实际证据。

- [ ] **Step 1: 请求并获得创建用户级 junction 的明确授权**

授权请求应明确说明：junction 指向仓库内 `skills/embedded-design-decision`，不会复制源码；目标位于用户级 `.agents\skills`。没有授权则停止 Task 3，并把显式/隐式触发标记为“未验证”，不能声称 Skill 已通过行为验收。

- [ ] **Step 2: 检查并创建 junction**

授权后运行：

```powershell
$skillSource = (Resolve-Path '.\skills\embedded-design-decision').Path
$skillTarget = Join-Path $env:USERPROFILE '.agents\skills\embedded-design-decision'
if (Test-Path -LiteralPath $skillTarget) {
  throw "目标已存在，停止创建 junction：$skillTarget"
}
New-Item -ItemType Directory -Path (Split-Path -Parent $skillTarget) -Force | Out-Null
New-Item -ItemType Junction -Path $skillTarget -Target $skillSource | Out-Null
Get-Item -LiteralPath $skillTarget | Select-Object FullName,LinkType,Target
```

预期：`LinkType` 为 `Junction`，`Target` 精确指向仓库 Skill 目录。不得删除或替换已存在目标。

- [ ] **Step 3: 创建隔离的回归工程和输出目录**

运行：

```powershell
$regressionRoot = Join-Path ([System.IO.Path]::GetTempPath()) ('embedded-design-decision-regression-' + [guid]::NewGuid())
New-Item -ItemType Directory -Path $regressionRoot | Out-Null
New-Item -ItemType Directory -Path (Join-Path $regressionRoot 'output') | Out-Null
$regressionRoot
```

预期：得到新的隔离目录。

- [ ] **Step 4: 运行显式、隐式和缺目录触发测试**

显式调用：

```powershell
$explicitOut = Join-Path $regressionRoot 'output\explicit.md'
$explicitPrompt = "使用 `$embedded-design-decision 为一个 STM32F4 UART DMA + FreeRTOS 接收模块做编码前设计。设计文档必须写入：$explicitOut"
codex exec --ephemeral --skip-git-repo-check -s workspace-write -C $regressionRoot -o (Join-Path $regressionRoot 'explicit-final.txt') $explicitPrompt
Test-Path -LiteralPath $explicitOut
```

预期：返回 `True`；文档说明 ISR 到 Task 数据流和 UART 所有权。

隐式调用：

```powershell
$implicitOut = Join-Path $regressionRoot 'output\implicit.md'
$implicitPrompt = "请为带 A/B 分区和回滚的嵌入式 OTA 产品做编码前架构设计。设计文档必须写入：$implicitOut"
codex exec --ephemeral --skip-git-repo-check -s workspace-write -C $regressionRoot -o (Join-Path $regressionRoot 'implicit-final.txt') $implicitPrompt
Test-Path -LiteralPath $implicitOut
```

预期：返回 `True`；文档分别处理 Application 和 Bootloader。

缺少目录：

```powershell
$missingPrompt = '请为一个新的 STM32 传感器产品做编码前架构设计。'
codex exec --ephemeral --skip-git-repo-check -s workspace-write -C $regressionRoot -o (Join-Path $regressionRoot 'missing-dir-final.txt') $missingPrompt
Get-ChildItem -LiteralPath (Join-Path $regressionRoot 'output') -File | Select-Object Name
```

预期：最终回复询问输出目录；除 `explicit.md` 和 `implicit.md` 外没有新设计文档。

- [ ] **Step 5: 运行三个误触发边界测试**

运行以下完整命令，使用只读沙箱：

```powershell
$nontriggerPrompts = @(
  '请直接实现一个 C99 的 CRC16 函数。',
  '请编译并烧录当前已有的 STM32 工程。',
  'Strategy 设计模式是什么意思？'
)
for ($i = 0; $i -lt $nontriggerPrompts.Count; $i++) {
  $caseNo = '{0:D2}' -f ($i + 1)
  $finalPath = Join-Path $regressionRoot "nontrigger-$caseNo-final.txt"
  codex exec --ephemeral --skip-git-repo-check -s read-only -C $regressionRoot -o $finalPath $nontriggerPrompts[$i]
}
```

预期：不会要求提供设计文档输出目录，不会声称正在生成编码前架构文档。读只沙箱确保误触发测试不改变工程。

- [ ] **Step 6: 运行六个核心行为场景**

运行以下完整命令：

```powershell
$regressionPrompts = @(
  '为资源受限 MCU 上的一个按键控制 LED 功能做编码前设计。',
  '设计 UART DMA 接收、RingBuffer 和 FreeRTOS 消费任务。',
  '设计只负责校验并跳转应用的 STM32F4 Bootloader。',
  '设计带 OTA、A/B 分区和回滚的产品升级方案。',
  '现有产品从 F411/W25Q64 迁移到 F407/W25Q128，要求尽量不重写 OTA 服务，请做编码前设计。',
  '公司已有架构，现在只新增 Sensor Module，请做模块编码前设计。'
)
for ($i = 0; $i -lt $regressionPrompts.Count; $i++) {
  $caseNo = '{0:D2}' -f ($i + 1)
  $documentPath = Join-Path $regressionRoot "output\case-$caseNo.md"
  $finalPath = Join-Path $regressionRoot "case-$caseNo-final.txt"
  $prompt = $regressionPrompts[$i] + " 设计文档必须写入：$documentPath"
  codex exec --ephemeral --skip-git-repo-check -s workspace-write -C $regressionRoot -o $finalPath $prompt
}
```

每个场景完成后，对照“必须观察到/不得观察到”人工检查实际文档。不得仅凭关键词出现判定通过；检查建议是否与输入约束一致。

- [ ] **Step 7: 覆盖 Review Focus 的五个高风险条件**

在 `regression-cases.md` 追加以下五个结果记录，不新增产品功能：

1. 给出以 `.md` 结尾但声称为“输出目录”的路径，预期 Skill 先确认目录/文件含义。
2. 预先创建同名目标文件，预期 Skill 不覆盖。
3. 同时包含 Application 与 Bootloader，预期分别判断复杂度。
4. 临时工程文档明确指定与通用报告不同的约束，预期采用项目事实并说明冲突。
5. 候选经验与稳定框架冲突，预期候选经验不能覆盖稳定规则。

每项记录实际输入、实际行为、PASS/FAIL 和证据文件。任何 FAIL 都进入窄修复循环：先把失败断言写清，再只修改拥有该行为的 `SKILL.md`、`decision-framework.md` 或 `engineering-experience.md`，重新运行该场景和六个核心场景中受影响的场景。

- [ ] **Step 8: 将实际结果回写 `regression-cases.md`**

为每个场景追加：

```markdown
### 验证结果

- 日期：2026-09-22
- 结果：PASS 或 FAIL
- 实际行为：对行为的事实描述
- 证据：临时输出文件名和支持结论的简短摘录
- 偏差：无，或精确描述未满足的断言
```

最终文件不得保留未执行却标为 PASS 的场景。临时目录绝对路径不进入 Skill、测试材料或通用规则；测试材料只记录证据文件名和支持结论的简短摘录。

- [ ] **Step 9: 授权后提交行为回归检查点**

仅在用户明确授权提交后运行：

```powershell
git add -- 'skills/embedded-design-decision' 'tests/embedded-design-decision/regression-cases.md'
git commit -m 'test: verify embedded design decision behavior'
```

否则记录“未提交：没有 Git 提交授权”，继续最终验收。

---

### Task 4: 最终验证和交付检查

**Files:**
- Read: `docs/superpowers/specs/2026-09-22-embedded-design-decision-design.md`
- Read: `docs/superpowers/plans/2026-09-22-embedded-design-decision.md`
- Read: `skills/embedded-design-decision/**`
- Read: `tests/embedded-design-decision/**`

**Interfaces:**
- Consumes: 前三个任务的 Skill、基线和行为证据。
- Produces: 可交付状态、未验证项和精确 Git 差异摘要。

- [ ] **Step 1: 运行 Skill 结构验证**

运行：

```powershell
$validator = Join-Path $skillCreatorRoot 'scripts\quick_validate.py'
python -X utf8 $validator '.\skills\embedded-design-decision'
```

预期：`Skill is valid!`

- [ ] **Step 2: 检查链接、编码和引用存在性**

运行：

```powershell
$skillRoot = Resolve-Path '.\skills\embedded-design-decision'
$required = @(
  'SKILL.md',
  'agents\openai.yaml',
  'references\decision-framework.md',
  'references\engineering-experience.md',
  'references\设计模式_GoF_嵌入式软件蒸馏.md',
  'references\bootloader-ota-deep-research-report.md',
  'references\freertos-deep-research-report.md'
)
$required | ForEach-Object {
  $path = Join-Path $skillRoot $_
  [pscustomobject]@{ RelativePath = $_; Exists = Test-Path -LiteralPath $path }
}
Get-Content -LiteralPath '.\skills\embedded-design-decision\SKILL.md' -Raw -Encoding UTF8 | Out-Null
```

预期：所有 `Exists` 均为 `True`，UTF-8 回读无异常。

- [ ] **Step 3: 检查规格覆盖和禁止项**

运行：

```powershell
Select-String -LiteralPath '.\skills\embedded-design-decision\SKILL.md' -Pattern '输出目录|decision-framework|engineering-experience|停止'
Select-String -LiteralPath '.\skills\embedded-design-decision\SKILL.md','.\skills\embedded-design-decision\references\decision-framework.md','.\skills\embedded-design-decision\references\engineering-experience.md' -Pattern 'TBD|TODO|待补充|直接编写业务代码|自动烧录'
```

预期：第一条搜索覆盖四类核心合同；第二条没有占位符，也没有把编码或烧录变成 Skill 行为。若“直接编写业务代码”出现在明确禁止语境中，人工确认后记录为允许。

- [ ] **Step 4: 核对测试结果完整性**

运行：

```powershell
Select-String -LiteralPath '.\tests\embedded-design-decision\regression-cases.md' -Pattern '结果：FAIL|结果：PASS'
Select-String -LiteralPath '.\tests\embedded-design-decision\baseline.md','.\tests\embedded-design-decision\regression-cases.md' -Pattern 'TBD|TODO|待补充|未执行却标为 PASS'
```

预期：所有实际执行场景都有结果；不存在占位符或虚假通过。存在 FAIL 时不得宣称行为验收通过，应修复或列为未完成项。

- [ ] **Step 5: 审查最终 Git 差异和未跟踪文件**

运行：

```powershell
git status --short
git diff --check
git diff --stat
```

预期：只有本计划列出的设计文档、计划、Skill 和测试材料发生变化；`git diff --check` 无输出。三个原始参考文件仍未被修改。

- [ ] **Step 6: 形成最终交付摘要**

摘要必须包含：

- 实际创建的文件。
- `quick_validate.py` 的结果。
- 六个核心场景、触发和误触发测试的通过/失败数量。
- 用户级 junction 是否创建及其目标。
- 未验证项或失败场景。
- 是否提交 Git；没有授权时明确写“未提交”。

- [ ] **Step 7: 授权后执行最终提交**

只有用户明确授权提交时，先展示 `git status --short` 和拟提交文件，再运行：

```powershell
git add -- 'docs/superpowers/specs/2026-09-22-embedded-design-decision-design.md' 'docs/superpowers/plans/2026-09-22-embedded-design-decision.md' 'skills/embedded-design-decision' 'tests/embedded-design-decision'
git commit -m 'feat: add embedded design decision skill'
```

未授权时不运行该步骤。
