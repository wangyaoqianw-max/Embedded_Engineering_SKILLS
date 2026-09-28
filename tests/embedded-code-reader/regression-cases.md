# Embedded Code Reader Regression Cases

## General constraints

- Read source and configuration only; modify only the requested source-reading Markdown document.
- Keep the reading scope centered on the user's target. Do not recursively read unrelated modules.
- Anchor key claims to repository-relative file paths and line numbers.
- Separate visible code facts from behavior that cannot be confirmed from the available sources.
- Include an ordered reading path through the relevant files and symbols.

## CORE-01 UART DMA receive flow

Use `scenario.md` with `fixtures/uart-dma/` and write the requested note.

Must cover:

- `main()` to `uart_service_start()` and the task entry.
- The `UART_RX_BACKEND` definition and the fact that this fixture does not use it to select code.
- DMA ISR to registered callback to task notification and wait.
- Buffer use, state transitions, and the error path.
- Unknown hardware, scheduler, and protocol behavior.
- Evidence references and a recommended reading order.

Must not:

- Modify fixture source or configuration.
- Claim that the fixture performs actual DMA or parses received data.
- Invent a build-selected path that is not represented in the fixture.

## CORE-02 Bootloader trial and rollback state flow

Use `scenario-bootloader.md` with `fixtures/bootloader-state/` and write a source-reading note.

Must cover:

- How the latest Metadata copy is selected using its commit marker, CRC, and sequence.
- Candidate validation and installation before `PENDING -> TRIAL` and application jump.
- How Application confirmation promotes the pending slot to confirmed and clears pending state.
- Why an unconfirmed `TRIAL` enters rollback on the next Bootloader entry, regardless of the logged reset-cause value.
- Confirmed-image validation and `TRIAL -> ROLLBACK` before destructive restore.
- How an interrupted pending install remains on the pending path at the next boot, and how an interrupted rollback retries restoration.
- Failure branches that halt rather than claiming installation or rollback succeeded.
- Checked source paths and line numbers; platform power-loss behavior remains unverified.

Must not:

- Claim the fixture performs actual EEPROM/Flash operations or proves hardware power-fail durability.
- Treat reset-cause logging as a state-selection condition.
- Modify fixture sources.

Baseline execution:

- 2026-09-23: CORE-02 run without the Skill; see `baseline-bootloader-output.md`.
- Observed omissions: Metadata double-copy selection and retrying installation after a reset before the `TRIAL` commit.
- 2026-09-23: CORE-02 run with the updated Skill; see `after-bootloader-output.md`.
- Forward review covered all required state-selection, commit-order, retry, failure, evidence, and hardware-boundary criteria; all 31 source references resolve within the fixture. The two baseline omissions are covered.

## CODEGRAPH-01 Indexed project

In a project that already has `.codegraph/`, ask for a source-reading note about a feature with a multi-file call or data flow.

Must:

- Query `codegraph_explore` before source search or reading with the target project path in `projectPath` when the MCP tool is available; otherwise run `codegraph explore` from the target project directory.
- Use the result to locate relevant symbols and candidate relationships, then verify key claims against source and active project configuration.
- Preserve the existing source-reference, runtime-registration, and unknown-behavior requirements.
- Leave the existing index and project source unchanged.
- Keep explicit/implicit source-reading triggers and non-trigger boundaries unchanged.

Must not:

- Treat an indexed edge as proof that a callback, ISR, task, or conditional branch executes at runtime.
- Run `codegraph init` or create an index automatically.

## CODEGRAPH-02 Project without an index

Use an existing fixture or project without `.codegraph/` for a normal source-reading request.

Must:

- Skip CodeGraph and follow the existing targeted source-reading workflow.
- Leave `.codegraph/` absent and avoid unrelated repository-wide searches.
- Keep explicit/implicit source-reading triggers and non-trigger boundaries unchanged.

Baseline check before change:

- 2026-09-24: No CodeGraph guidance or regression case existed. This Skills repository has no `.codegraph/`, so the indexed-project path was unavailable for live exercise; no index was created.

## STRUCTURE-01 C 类型与模块静态结构图

使用 `scenario-bootloader.md` 和 `fixtures/bootloader-state/` 生成源码阅读笔记。

必须：

- 当多个相关结构体、枚举或模块接口有助于理解目标的静态结构时，在报告中加入 Mermaid `classDiagram`。
- 将源码中实际存在的 `struct`、`enum`、`typedef` 和模块接口作为图中元素；模块节点标为 `<<module>>`，类型节点标明其 C 类型类别。
- 展示模块函数前核对其声明、链接属性和实际调用；不使用 Mermaid 的 `+`、`-`、`#` 为 C 函数推断访问级别，也不把文件内 `static` 函数标成模块外部接口。
- 模块节点使用相关 `.c` 实现文件；头文件只作为声明和类型的证据，不单独建模为模块，也不把 `#include` 画成模块协作边。枚举等标量字段保留为类型字段，不另画关系线。
- 只按可见声明表达字段和关系：按值嵌入的结构体可表示为包含关系；指针成员只表示引用，不推断所有权或生命周期。
- 回调关系只有在源码同时显示注册和分发路径时才能连线。
- 使用 `scenario.md` 和 `fixtures/uart-dma/` 核对回调边：证据要覆盖 `uart_service.c` 中的注册以及 `dma.c` 中的实际间接调用；`dma.h` 中的 typedef 声明本身不足以证明运行时关系。
- 为图中的节点和关系提供相邻的证据表，标出仓库相对路径和核对过的行号。
- 保持调用顺序、运行时数据流和状态转换在原有章节说明；没有有用静态关系时省略类图。

不得：

- 根据符号名称或设计习惯虚构类、字段、方法、所有权或调用关系。
- 用类图替代运行流程、数据流或状态流分析。
- 为只有单一类型且没有有意义关系的目标强行绘图。

基线（2026-09-28）：未加载目标 Skill 的隔离评估者按 Bootloader 阅读场景生成了文字流程和源码引用，没有输出静态结构图。更新后用同一场景检查是否能在证据支持时生成结构图。

更新后核验（2026-09-28）：`after-bootloader-output.md` 和 `after-output.md` 保存了 Bootloader 类型/模块图与 UART DMA 回调图。独立评估者按最新 Skill 对 Bootloader 场景生成了图，类型、模块边和行号证据均能对应源码；枚举成员只列为字段，没有额外关系线。UART DMA 图的回调边同时引用注册、保存和 ISR 分发证据。首轮评估暴露的 C 可见性标记、头文件模块节点和枚举关系线问题已写入规则并修正。

触发边界复核（2026-09-28）：本次未修改 Skill 的 frontmatter、适用范围或非目标范围；显式/隐式嵌入式源码阅读仍匹配，单独的编码、构建/烧录、调试、Code Review 和概念解释仍不匹配。

## TRIGGER-01 Explicit invocation

Request `$embedded-code-reader` and ask it to explain an existing embedded module's configuration, initialization, runtime flow, data/state changes, or source-reading order. It should apply the Skill and create/update only the requested Markdown note.

## TRIGGER-02 Implicit invocation

Without naming the Skill, ask to understand an existing embedded firmware feature from source and produce a Markdown source-reading note. The Skill should apply.

## TRIGGER-03 Missing target

Ask for a source-reading note without naming a module or feature. It should ask which target to analyze and not start a repository-wide scan.

## NONTRIGGER-01 Direct code change

Ask only to implement or modify a CRC function. The Skill should not expand the task into a source-reading document.

## NONTRIGGER-02 Build or flash

Ask only to build or flash an existing embedded project. The Skill should not apply.

## NONTRIGGER-03 Debug or code review

Ask only to locate/fix a runtime fault or review code quality. The Skill should not take over the Debug or Code Review workflow.

## NONTRIGGER-04 Concept explanation

Ask what UART DMA or a callback means without asking to analyze an existing project. The Skill should not apply.

## Baseline execution

- 2026-09-23: CORE-01 run without the Skill; see `baseline-output.md`.
- Observed gaps against the supplied design: no line-level evidence and no recommended source-reading order.

## Skill and trigger review

- 2026-09-23: CORE-01 run with the Skill by an isolated evaluator; see `after-output.md`. The note includes line references, an ordered reading path, configuration/callback/state tracing, and explicit unknowns. The evaluator wrote only the requested note; fixture sources remained unchanged.
- 2026-09-23: Explicit invocation was exercised by directing an isolated evaluator to load this Skill and apply the scenario. The requested note was produced.
- 2026-09-23: Trigger-boundary review used the frontmatter description only. The concrete implicit source-reading request was classified as applicable; direct coding, GCC build, HardFault diagnosis, concept explanation, and bounds/race review were classified as non-triggers. A request for a note with no target is applicable when the current project is known to be embedded; the Skill asks for the missing target.
- 2026-09-23: Rechecked trigger cases after CORE-02 changes. Explicit and implicit source-reading requests remain applicable; a missing target prompts a question when embedded-project context is established. Direct code changes, build/flash, debugging, code review, and concept-only requests remain non-triggers. The frontmatter trigger description was unchanged in this change set.
- Limitation: these are isolated prompt reviews, not runtime auto-discovery through a Codex-installed Skill. The repository source was not linked into a user-level Skill directory, so platform-level explicit/implicit discovery remains unverified.
