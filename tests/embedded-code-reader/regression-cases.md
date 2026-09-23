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
