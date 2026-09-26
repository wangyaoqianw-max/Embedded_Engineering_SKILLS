# 芯片硬件事实整理 Skill 实施计划

> **执行说明：** 用户选定执行方式后，按对应流程执行：使用子代理时遵循 `superpowers:subagent-driven-development`，由当前代理执行时遵循 `superpowers:executing-plans`。按顺序完成各项任务。

**目标：** 创建一个 Codex 可发现的 Skill，从已有资料中提取与项目相关的硬件事实，交叉核对来源，并给出带出处的使用约束。

**设计：** 精简的 `SKILL.md` 负责任务拆解、资料分工、跨文档核查、原理图缺失或不可读时请求局部截图、证据状态、项目根目录识别、Markdown 报告位置和输出要求。仓库测试材料使用虚构芯片与项目布局，对比 Skill 使用前后的行为，并检查触发边界和本地发现方式。

**技术栈：** Markdown Skill 和场景文件、YAML frontmatter、PowerShell、Codex 评估会话、`quick_validate.py`、Git。

**规格：** `docs/superpowers/specs/2026-09-26-extracting-chip-hardware-facts-design.md`

## 全局约束

- 只读取和分析现有项目及硬件资料；不下载或转换文档。
- 本 Skill 不实现固件、不构建、不烧录、不调试。
- 板级证据不足时标记为 `UNRESOLVED`；不得从芯片能力推断实际连线。
- 原理图缺失或目标区域不可读时，请求能看清 MCU 引脚名、网络标号和相关器件连接的局部截图。
- 使用 `CONFIRMED`、`INFERRED`、`UNRESOLVED`、`CONFLICT` 区分证据状态。
- 每次完成整理都生成 Markdown 报告。用户指定路径时优先写到该路径；否则根据项目规则和既有约定写在识别出的项目目录内，并采用规定的回退路径。不得覆盖已有报告。
- Skill 指引中不得硬编码本机绝对路径。

## 评审重点

1. 封装或硅片修订版未知时，保留不确定性，不得在未匹配修订版时套用特定 Errata。Task 1 建立基线，Task 3 验证。
2. 原理图目标区域缺失或不可读时，请求局部截图，并在继续其他核查的同时保持板级连线未确认。Task 1 建立基线，Task 3 验证。
3. 外设依赖分散在不同资料时，继续核查相关时钟、DMA 和 IRQ，不得只查用户点名的章节。Task 1 建立基线，Task 3 验证。
4. 证据冲突或推导事实须区分直接事实、推断、冲突和未解决状态，并标注来源。原始场景覆盖推断；下文追加由审阅反馈触发的板级资料冲突场景。
5. 不接管纯概念解释、缺失资料下载/索引、固件实现或调试。Task 4 验证。

---

### Task 1：建立失败基线

**文件：**

- 创建：`tests/extracting-chip-hardware-facts/scenario.md`
- 创建：`tests/extracting-chip-hardware-facts/baseline.md`
- 创建：`tests/extracting-chip-hardware-facts/baseline-output.md`
- 创建：`tests/extracting-chip-hardware-facts/regression-cases.md`
- 创建：`tests/extracting-chip-hardware-facts/output-location-scenario.md`
- 创建：`tests/extracting-chip-hardware-facts/output-location-baseline.md`
- 在 `tests/extracting-chip-hardware-facts/fixtures/` 下创建项目布局测试夹具。

**接口：**

- 输入：已批准的规格和仓库现有测试约定。
- 输出：包含虚构 Datasheet、Reference Manual、Errata 摘录的完整场景；未加载 Skill 的记录回复；可观察的回归判据。

- [x] **步骤 1：编写 `scenario.md`**，提供共用的虚构 Datasheet、Reference Manual、Errata 摘录，以及两个独立的 USART + DMA + RS485 请求变体：一个没有原理图，一个的目标区域不可读。摘录须要求核对引脚能力、时钟/DMA 依赖和修订版相关 Errata。明确所有摘录均为测试数据，不代表真实芯片事实。
- [x] **步骤 2：编写 `regression-cases.md`**，列出可观察的资料分工、实际连线与芯片能力区分、证据位置/状态、截图请求细节、使用限制、显式/隐式触发和非触发判据。
- [x] **步骤 3：在两个不加载目标 Skill 的独立上下文中运行场景变体**，不提供预期答案或评分标准。评估期间不得检查仓库或联网。
- [x] **步骤 4：将评估回复原样保存到 `baseline-output.md`**，并在 `baseline.md` 记录做得较好的部分和具体遗漏。
- [x] **步骤 5：在创建 `SKILL.md` 前确认至少一个目标行为存在明显基线缺陷**；若场景未能暴露缺陷，则调整场景并重跑基线。
- [x] **步骤 6：创建两个嵌套工作目录的虚构项目夹具**：一个明确规定硬件报告目录和命名方式；另一个没有约定，且已有同名回退路径文件。
- [x] **步骤 7：在独立且未加载 Skill 的上下文中运行两个输出位置场景**，从嵌套项目目录开始。用自然语言请求硬件整理，不指定文件名或输出路径；观察是否创建报告及保存位置。不要提供评分标准。
- [x] **步骤 8：将无 Skill 的报告结果保存到 `output-location-baseline.md`**；保留回复和生成报告，同时将本机绝对根路径替换成 `<fixture-root>`。记录项目根目录、项目约定、回退路径和防覆盖情况；在修改 Skill 前确认具体行为缺陷。

### Task 2：编写最小 Skill

**文件：**

- 修改：`skills/extracting-chip-hardware-facts/SKILL.md`

**接口：**

- 输入：已批准的规格，以及 Task 1 记录的基线缺陷。
- 输出：Skill 入口 `extracting-chip-hardware-facts`。

- [x] **步骤 1：编写 YAML frontmatter**，名称为 `extracting-chip-hardware-facts`；描述以 `Use when...` 开头，并明确触发场景为项目相关硬件资料提取。
- [x] **步骤 2：补齐流程和输出要求**，包含项目根目录识别、项目规则和既有约定优先级、日期化的 `docs/hardware-analysis/` 回退路径、防覆盖命名，以及最终返回 Markdown 报告链接。
- [x] **步骤 3：运行随附的 Skill 校验器**，对 `skills/extracting-chip-hardware-facts` 执行检查；预期无脚手架占位内容并通过校验。

### Task 3：验证 Skill 行为

**文件：**

- 修改：`tests/extracting-chip-hardware-facts/after-output.md`
- 修改：`tests/extracting-chip-hardware-facts/baseline.md`
- 修改：`tests/extracting-chip-hardware-facts/regression-cases.md`
- 创建：`tests/extracting-chip-hardware-facts/output-location-after.md`
- 创建：`tests/extracting-chip-hardware-facts/conflict-scenario.md`
- 创建：`tests/extracting-chip-hardware-facts/conflict-baseline.md`
- 创建：`tests/extracting-chip-hardware-facts/conflict-after.md`
- 创建：`tests/extracting-chip-hardware-facts/trigger-boundary-evaluations.md`

**接口：**

- 输入：`scenario.md`、目标 Skill 和回归判据。
- 输出：加载 Skill 后的同场景响应，以及各核心行为的验证证据。

- [x] **步骤 1：在两个独立上下文加载 Skill，重跑 Task 1 的两个场景变体**，并将回复保存到 `after-output.md`。
- [x] **步骤 2：核实结果**保持板级连线未确认、请求局部截图、使用夹具预期报告目录、从嵌套工作目录识别项目根目录、遵循明确约定、使用回退路径且不覆盖文件，并返回报告路径/链接。
- [x] **步骤 3：记录前后对比**到 `baseline.md` 和 `output-location-baseline.md`；在 `regression-cases.md` 中将各项回归判据标为已验证或未验证。
- [x] **步骤 4：Skill 内容修正后重新运行 `quick_validate.py`**；预期校验成功。

### Task 3 扩展：验证证据冲突

**文件：**

- 创建：`tests/extracting-chip-hardware-facts/conflict-scenario.md`
- 创建：`tests/extracting-chip-hardware-facts/conflict-baseline.md`
- 创建：`tests/extracting-chip-hardware-facts/conflict-after.md`
- 在 `tests/extracting-chip-hardware-facts/fixtures/conflict-project/` 下创建板级资料冲突的合成夹具。

- [x] **步骤 1：创建合成探针**：两份均标为当前修订版的板级资料把同一 U5 信号接到不同 MCU 引脚；Datasheet 确认两个引脚都具备芯片侧能力。
- [x] **步骤 2：先运行独立的无 Skill 基线**，只问引脚选择问题，不展示评分标准。
- [ ] **步骤 3：在独立且加载 Skill 的新上下文运行相同问题**，保存生成的 Markdown 报告。本机 Codex CLI 无法初始化 app-server（`os error 5`）；已在隔离夹具副本中应用 Skill 并生成报告，但独立新上下文运行仍未验证。
- [x] **步骤 4：核实**实际板级映射标为 `CONFLICT`、引用两份冲突资料、不擅自选脚并说明电气差异。将前后结果记录在 `conflict-baseline.md`、`conflict-after.md` 和 `regression-cases.md`，并注明新上下文限制。

### Task 4：验证触发边界并接入发现路径

**文件：**

- 修改：`tests/extracting-chip-hardware-facts/regression-cases.md`
- 创建：`tests/extracting-chip-hardware-facts/trigger-boundary-evaluations.md`
- 在仓库外创建单 Skill Junction：`$env:USERPROFILE\.agents\skills\extracting-chip-hardware-facts`，目标为本仓库对应 Skill 目录。

**接口：**

- 输入：已校验的 Skill 和仓库内 Skill 源目录。
- 输出：用户级发现链接和已记录的触发边界结果。

- [x] **步骤 1：检查目标路径**，确认目标不存在或已指向本 Skill；保留其他无关路径。
- [x] **步骤 2：创建或核实单 Skill Junction**，布局沿用 `embedded-resource-acquisition`；不得重定向整个 `.agents/skills` 目录。
- [x] **步骤 3：通过安装链接验证 Skill 文件可读**，测试隐式项目硬件提取和显式 `$extracting-chip-hardware-facts` 调用。若本机 Codex 运行时无法启动，记录错误并将运行时检查标为未验证。
- [x] **步骤 4：在独立响应探针中检查非触发场景**：纯概念解释、只查找/下载/索引资料、只实现/构建/烧录/调试固件。记录提示和实际路由；运行时不可用时，保留 Codex 原生自动路由为未验证。
- [x] **步骤 5：在 `regression-cases.md` 记录发现链接和触发结果**；运行时或模型服务失败均标为未验证，不误记为 Skill 通过或失败。

### Task 5：复审、提交并推送

**文件：**

- 复审：Task 1–4 创建或修改的全部文件。

**接口：**

- 输入：已校验 Skill、测试证据、批准的设计，以及跟踪 `origin/main` 的 `main` 分支。
- 输出：一条已复审提交，推送到配置的上游。

- [ ] **步骤 1：检查** `git diff --check`、UTF-8 可读性、来源链接、输出路径和完整仓库差异；确认只包含已批准的 Skill、测试材料和设计/计划文档。
- [ ] **步骤 2：暂存列出的文件并提交**，提交说明为 `feat: add chip hardware facts skill`。
- [ ] **步骤 3：将当前 `main` 推送到 `origin`**，并核实本地分支与上游一致。
