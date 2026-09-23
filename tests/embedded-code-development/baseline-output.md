# No-Skill Baseline Evidence

## Evaluator launch

- Main-context evaluator attempt: fresh Codex CLI process, intended to work only on an isolated copy of the sanitized fixture.
- Target Skill state: embedded-code-development did not exist.
- Observed CLI result: process initialization failed before the prompt ran because its Codex state database was read-only.
- This CLI attempt produced no evaluator response and made no fixture changes.

## Isolated no-Skill request

- Evaluator: sub-agent context with no embedded-code-development Skill loaded.
- Request: “Add a small status counter module to this project, make it available to both Keil Targets, and tell me what changed.”
- Worktree: an isolated temporary copy of the sanitized fixture; the repository fixture remained unchanged.
- Files changed in the temporary copy: added `Core/Inc/status_counter.h` and `Core/Src/status_counter.c`; modified `Core/Src/main.c`; modified `MDK-ARM/Board.uvprojx` to register both files under the Application group in each Target.
- Observed checks: project XML parsed; Debug and Release remained available; their distinct Objects and Listings paths remained intact.
- Build: not run in the evaluator context, so compilation and before/after comparison are unverified.
- Limitation: this is one bounded implementation scenario, not a fresh-context or broad behavior evaluation. It does not establish how the model handles port licensing, historical build errors, or sub-agent boundaries without the Skill.

The CLI launch failure is an execution-environment failure, not evidence of model behavior. The separate sub-agent's bounded no-Skill output is recorded above; no output was produced by the failed CLI attempt.

## Skill inventory before implementation

- Repository Skills present: embedded-code-reader and embedded-design-decision.
- Repository Skill absent: embedded-code-development.
- Result for explicit invocation: no target SKILL.md was available to load.
- Result for implicit invocation: not measured in a fresh evaluator.

## Fixture and Keil tool baseline

These checks were performed by the main execution context as fixture validation; they are not evaluator responses.

- Previously working firmware projects were references only. The user authorized read-only use of their existing Keil tool and project-local standard; no build ran against those projects, no project source or configuration was changed, and no project-specific paths or source files were copied into this repository.
- The actual build check used only this synthetic CubeMX/Keil fixture.

- The Keil project parser found exactly two Targets: Debug and Release.
- Debug output paths: ../build/keil-debug/objects/ and ../build/keil-debug/listings/.
- Release output paths: ../build/keil-release/objects/ and ../build/keil-release/listings/.
- An earlier fixture draft produced an empty build with no source records or output artifact; that run was rejected as an invalid baseline. After correcting the synthetic project structure, each Target was Cleaned and Built from a fresh fixture copy before application feature edits. The raw logs included `compiling main.c`, `assembling startup.s`, and `linking`, with the expected AXF output present.
- Debug: 0 errors, 0 warnings.
- Release: 0 errors, 0 warnings.
- Toolchain recorded from the build result: µVision 5.38.0.0, ARMCC V5.06 update 7 build 960.
- The historical compiler and object-file I/O logs were not produced by these builds.

## Limitation

The baseline request demonstrated source/project synchronization and preservation of per-Target paths in one case, but no build was run by the no-Skill evaluator. The independent fixture check establishes that a valid pre-edit build can be produced from this synthetic project. Skill-specific behavior assertions outside this case remain unverified at baseline.
