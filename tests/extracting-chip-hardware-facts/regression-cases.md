# Regression Cases

## Core behaviors

- **CORE-01 Task decomposition:** Extract the exact MCU/package/revision, target feature, and board facts needed for USART3 DMA RS485; separate known facts from unknowns.
- **CORE-02 Source routing:** Use the Datasheet for package pins, alternate functions, and electrical limits; the Reference Manual for clock, DMA, IRQ, enable order, and transmit-completion behavior; the Errata Sheet for revision-specific conditions and workaround; the schematic for actual board nets.
- **CORE-03 No inferred board wiring:** Datasheet-supported pin options do not establish which pin is wired to U5. Keep the board mapping `UNRESOLVED` while the relevant schematic is unavailable or illegible.
- **CORE-04 Local screenshot request:** For both scenario variants, ask for a focused screenshot that shows MCU pin names, net labels, U5 connections, DE/RE control, and relevant connector/interface labels. Do not demand a full manual or schematic when a local crop is sufficient.
- **CORE-05 Cross-document dependencies:** Include APB1, GPIO/USART clock enabling, DMA channel/stream choices, IRQ names, and the distinction between DMA transfer-complete and USART transmission-complete.
- **CORE-06 Errata applicability:** Because the silicon revision is unknown, state that the `REV_A` Errata condition cannot yet be matched; explain its trigger and workaround without claiming it applies to this board.
- **CORE-07 Evidence and use summary:** Cite the supplied document section/table for material claims, use `CONFIRMED`, `INFERRED`, `UNRESOLVED`, or `CONFLICT` as appropriate, and finish with task-specific hardware setup dependencies and open items.
- **CORE-08 Conflicting board sources:** If two current board documents assign the same signal to different pins, cite both and mark the board mapping `CONFLICT`; keep Datasheet pin capability separate, state any electrical risk, do not select a pin, and ask for an authoritative source or focused screenshot if needed.

## Markdown output location

- **OUTPUT-01 Always persist:** A completed project-specific hardware extraction creates a non-empty Markdown report. Use a user-specified output path when given; otherwise save under the identified target project, even when the task is phrased as a request to organize or summarize rather than explicitly naming a file.
- **OUTPUT-02 Find the project root:** Starting from a nested firmware working directory, identify the ancestor project root and keep the report inside it when no output path is specified.
- **OUTPUT-03 Follow project convention:** Prefer the target project's `AGENTS.md` / README instructions and existing hardware-analysis folder and filename style over the fallback.
- **OUTPUT-04 Safe fallback:** With no project convention, use `<project-root>/docs/hardware-analysis/YYYY-MM-DD-<chip>-<feature>.md`; if that path already exists, choose a non-conflicting filename without modifying the existing file.
- **OUTPUT-05 Return the artifact:** The final response gives a usable absolute file link and, when the report is inside the project, its project-relative path.
- **OUTPUT-06 Unclear root:** If the target project or root cannot be identified, ask which project/materials to analyze; ask for a save destination only when the user has not specified an output path.
- **OUTPUT-07 Explicit path override:** If the user specifies an output path outside the project, use it; otherwise the project-local convention and fallback apply.

## Trigger boundary

- **TRIGGER-01 Explicit:** `$extracting-chip-hardware-facts` plus a project-specific peripheral/pin/board question activates the Skill.
- **TRIGGER-02 Implicit:** A request to extract hardware constraints from an embedded project's existing manuals for a named feature activates the Skill without naming it.
- **NONTRIGGER-01 Concept only:** A standalone question such as “What does USART DMA do?” does not invoke the project-specific extraction workflow.
- **NONTRIGGER-02 Resource acquisition only:** A request to find or download missing manuals is handled by `embedded-resource-acquisition`.
- **NONTRIGGER-03 Firmware work only:** A request to implement, build, flash, or debug firmware does not invoke this Skill by itself.

## Execution record

- Baseline: completed on 2026-09-26 in two fresh no-Skill evaluations. `CORE-04` failed in both variants: neither evaluator requested a targeted screenshot. `CORE-06` failed because both recommended the REV_A workaround without classifying applicability as unresolved. `CORE-03` and `CORE-07` were partial: the evaluators did not claim actual wiring, but did not mark each board-dependent conclusion unresolved or consistently attach statuses and source locations. `CORE-05` passed.
- With-Skill evaluation: completed on 2026-09-26 in two fresh evaluations. `CORE-01` through `CORE-07` passed in both variants; both requested a readable local crop, kept board mapping and Errata applicability unresolved, and cited the provided sections/tables. See `after-output.md`.
- Explicit trigger: passed in a fresh context through `$extracting-chip-hardware-facts`; the evaluator read the installed Skill and created the report in the disposable project. Implicit trigger: passed in the two fresh output-location evaluations using the natural project-specific extraction request.
- Non-trigger response probes: three separate prompts and actual responses were captured in `trigger-boundary-evaluations.md`. They show a direct concept explanation, missing-manual routing to `embedded-resource-acquisition`, and firmware implementation/build routing to `embedded-code-development`. These model response probes do not verify native Codex auto-dispatch, which remains unverified.
- Discovery link: the user-level per-Skill junction resolves to this repository's `skills/extracting-chip-hardware-facts` directory, and the linked `SKILL.md` was readable in the fresh evaluations. A direct `codex exec` invocation was attempted, but the local runtime failed to initialize its read-only state database / app-server client (`os error 5`, access denied); native Codex runtime discovery/trigger remains unverified.
- Output-location no-Skill baseline: completed on 2026-09-26. The documented-convention fixture wrote to its instructed folder; the no-convention fixture returned only an inline answer and created no report. See `output-location-baseline.md`.
- Output-location with-Skill evaluation: completed on 2026-09-26 in two fresh contexts. Both created and linked Markdown reports from nested working directories. The documented project convention was followed; the fallback fixture used a `-2` collision suffix and preserved the sentinel. See `output-location-after.md`.
- Conflict baseline: completed on 2026-09-26 in a separate no-Skill evaluator. It noticed the PA3/PC11 disagreement and electrical difference, but conditionally recommended a pin based on assumed source precedence, omitted the `CONFLICT` status and screenshot fallback, and created no report. See `conflict-baseline.md`.
- Conflict with-Skill probe: completed on 2026-09-26 in an isolated fixture copy with the Skill instructions applied. The response marked the board mapping `CONFLICT`, cited both sources and the Datasheet, selected neither pin, described the PC11 3.6 V limit, requested an authoritative local source/screenshot, and created a non-empty report under `docs/hardware-analysis/`. The direct Codex CLI was unable to start an independent model context (`os error 5`), so this is an in-session probe rather than a fresh-context evaluation. See `conflict-after.md`.

## Output-location criterion status

| Criterion | No-Skill baseline | With Skill |
|---|---|---|
| OUTPUT-01 Always persist | Partial: passed only when explicit project instructions led the evaluator to create a report; failed in the no-convention fixture. | Pass in both fixtures. |
| OUTPUT-02 Find project root | Pass in the documented-convention fixture; not exercised when no report was written in the fallback fixture. | Pass in both fixtures from nested `Firmware/Src/`. |
| OUTPUT-03 Follow project convention | Pass in the documented-convention fixture; no convention existed in the fallback fixture. | Pass in the documented-convention fixture. |
| OUTPUT-04 Safe fallback and no-overwrite | Not demonstrated; no fallback report was created. Sentinel remained unchanged. | Pass: dated fallback used with `-2`; sentinel SHA-256 unchanged. |
| OUTPUT-05 Return the artifact | Pass in the documented-convention fixture; failed in the fallback fixture. | Observed pass in both original evaluations: the responses linked the reports with local absolute paths. The repository evidence records only the relative suffix after redaction, so the absolute link targets are not independently verifiable from the stored copies. |
| OUTPUT-06 Unclear root | Not tested. | Not tested. |
| OUTPUT-07 Explicit path override | Not tested. | Not tested. |

## Trigger-boundary results

| Case | Result | Evidence |
|---|---|---|
| TRIGGER-01 Explicit | Pass in the fresh explicit-trigger evaluation. | The evaluator followed `$extracting-chip-hardware-facts` through the installed link and created a report in the disposable fixture. |
| TRIGGER-02 Implicit | Pass in two fresh natural project-specific extraction evaluations. | Both loaded the linked Skill, analyzed existing project hardware files, and created the required report. |
| NONTRIGGER-01 Concept only | Pass in a captured fresh response probe. | The evaluator gave a direct UART DMA explanation without project inspection or a report. See `trigger-boundary-evaluations.md`. |
| NONTRIGGER-02 Resource acquisition only | Pass in a captured fresh response probe. | The evaluator routed missing-manual lookup/download to `embedded-resource-acquisition`; no download or archive write was performed. See `trigger-boundary-evaluations.md`. |
| NONTRIGGER-03 Firmware implementation/build/debug | Pass in a captured fresh response probe. | The evaluator routed implementation/build to `embedded-code-development` and the appropriate build skill; the hardware-facts workflow did not take over. See `trigger-boundary-evaluations.md`. |

## Conflicting-source criterion status

| Criterion | No-Skill baseline | With Skill |
|---|---|---|
| Identify both current board sources and their mappings | Pass: PA3 vs PC11 was surfaced. | Pass: both sources cited with their claims. |
| Mark the board mapping `CONFLICT` and select neither pin | Fail: conditionally chose one based on which document was authoritative. | Pass in the in-session probe; no pin selected. |
| Separate Datasheet capability from actual wiring | Partial: distinguished capability but did not assign the mapping a status. | Pass: both pins marked `CONFIRMED` as chip capabilities; board mapping marked `CONFLICT`. |
| Explain the electrical-risk difference | Pass: called out PA3 5 V tolerant vs PC11 max 3.6 V. | Pass: cited §5.3 Table 20 and the PC11 voltage constraint. |
| Request an authoritative source or focused local screenshot | Fail: asked for source/continuity confirmation, no screenshot fallback. | Pass: requested the authoritative U5.RO-to-MCU crop with labels and revision if source files cannot be provided. |
| Persist a project-local Markdown report and return its path | Fail: no report created. | Pass for report creation and relative path: the report was created under the instructed folder and its path was returned. The absolute link target is redacted and cannot be independently verified from this evidence copy. |
| Independent fresh with-Skill context | Not applicable. | Unverified: the Codex CLI could not initialize the app-server client (`os error 5`); the behavior probe ran in the current context with Skill instructions applied. |
