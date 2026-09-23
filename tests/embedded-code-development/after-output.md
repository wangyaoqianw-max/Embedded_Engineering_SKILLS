# Post-Skill Verification Record

## Evaluation method

- Date: 2026-09-23
- Target: `skills/embedded-code-development/`
- Core scenario: CORE-01, run by the main agent as a manual workflow check against an isolated copy of `fixtures/cubemx-keil/` while following the Skill and its project-build reference.
- This was not a native Codex skill-router invocation. The available fresh-evaluator attempt had failed during baseline capture because its app-server could not initialize against a read-only state database; no working native evaluator was available for post-Skill routing, so explicit invocation, implicit routing, and non-trigger routing were not observed in Codex itself.
- The repository fixture and the user's real firmware project were not modified or built during this check.
- The single Skill directory is linked into the user-level `.agents/skills/embedded-code-development` discovery location. The junction target and matching entrypoint hash were verified; this filesystem check does not establish native trigger behavior.
- Skill Creator `quick_validate.py` result: `Skill is valid!`.

## CORE-01 and CubeMX/Keil build check

The isolated fixture was first Cleaned and Built before application edits. Debug and Release each showed `compiling main.c`, `assembling startup.s`, `linking`, and an AXF artifact, with 0 errors and 0 warnings.

The temporary implementation then:

- Added `Core/Inc/status_counter.h` and `Core/Src/status_counter.c`.
- Updated `Core/Src/main.c` to increment and read the counter.
- Registered both files in the Application group for Debug and Release in `MDK-ARM/Board.uvprojx`.

After the change, both Targets rebuilt successfully. Their raw logs contained compilation records for `status_counter.c` and `main.c`, assembly of `startup.s`, and linking. Each reported 0 errors and 0 warnings, and its configured AXF artifact existed. The Keil wrapper reported 376 flash bytes and 104 RAM bytes for each Target; the pre-edit baseline reported 348 flash bytes and 100 RAM bytes.

The modified files existed only in a uniquely named temporary fixture copy. It was removed after collecting the sanitized build evidence; no generated output or tool log was added to the repository.

### Test harness note

Early temporary harness attempts omitted required file metadata or reserialized empty XML elements, and UV4 refused those test copies. Those harness results were discarded. The final run inserted complete file entries using the fixture's existing XML formatting and parsed both Target groups before building.

The independent reviewer also rebuilt an equivalent temporary implementation. Its scratch `status_counter.h`, `status_counter.c`, and modified `main.c` lacked final newlines, producing four compiler `#1-D` warnings per Target. The primary run recorded above wrote newline-terminated files and produced zero warnings. This separate result is retained to show that the warning count depends on the exact temporary source bytes.

## Scenario status

| Scenario | Result | Evidence or limitation |
|---|---|---|
| CORE-01: add a module and synchronize project inputs | Workflow check passed; native Skill activation unverified | Temporary implementation and successful Debug/Release builds above. |
| CORE-02: fresh CubeMX/Keil preflight and before/after comparison | Workflow check passed | Pre-edit and post-edit builds both compiled sources, linked, and produced AXF artifacts for both Targets. |
| CORE-03: third-party porting boundary | Unverified as behavior | Reference content reviewed; no porting prompt was executed. |
| CORE-04: compiler diagnostic vs output I/O failure | Unverified as behavior | Classification guidance reviewed; supplied historical logs were not presented to a post-Skill evaluator. |
| CORE-05: sub-agent ownership | Unverified as behavior | Policy is present in SKILL.md; no routing scenario was executed. |
| TRIGGER-01: explicit invocation | Unverified | Native Codex router was unavailable in the evaluator environment. |
| TRIGGER-02: implicit invocation | Unverified | Native Codex router was unavailable in the evaluator environment. |
| NONTRIGGER-01 through NONTRIGGER-06 | Unverified | No native routing evaluator ran these requests. |

The manual workflow check validates the project inspection, file synchronization, and build-evidence steps only. It does not establish that Codex will activate the Skill for explicit or implicit requests, nor that unrelated requests will stay outside it.
