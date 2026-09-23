# Embedded Code Development Baseline

## Environment

- Date: 2026-09-23
- Target Skill: absent from both the repository Skill list and the active user-level Skill list before implementation; the repository entrypoint was created after baseline capture.
- Fixture: fixtures/cubemx-keil/
- Evaluator: an isolated sub-agent context with no embedded-code-development Skill loaded; the repository itself was not changed during the evaluation.
- Codex CLI: 0.156.0

## Result

The main-context fresh Codex CLI attempt failed before accepting a scenario prompt because its app-server could not initialize against a read-only Codex state database. A separate no-Skill sub-agent evaluation did run the bounded CORE-01 request against an isolated fixture copy. It added the counter source/header, called it from `main.c`, and registered both files in each Target's Application group. The project XML parsed and both Target definitions and output paths remained intact. The evaluator could not run a Keil build in its context, so compilation and before/after build comparison remain unverified. Exact observations are recorded in baseline-output.md.

The Skill inventory is an observable routing baseline: the repository contained embedded-code-reader and embedded-design-decision, but no Skill entrypoint for implementing, modifying, or porting firmware code. Explicit invocation of embedded-code-development was therefore unavailable. The generic model completed the narrow CORE-01 source/project synchronization request without the target Skill; this single result does not establish general implicit routing behavior.

The fixture itself was checked independently of the no-Skill evaluator. An earlier fixture draft produced only a target banner and no source compilation or output artifact; that was an invalid empty build and was not counted. After correcting the synthetic project structure, the Keil project parser enumerated Debug and Release. On fresh copies, each Target was Cleaned and Built before any feature source was added; the raw logs showed `main.c` compilation, `startup.s` assembly, link output, and zero errors and warnings.

## Baseline conclusion

The missing target Skill establishes a gap in explicit routing and repeatable implementation guidance. The observed CORE-01 response handled the requested source/project synchronization and preserved distinct Target paths, but did not produce build evidence. This run does not establish how the model handles fresh-project preflight, port licensing, historical build errors, or sub-agent boundaries without the Skill. Those behavior claims remain unverified.

The baseline gate is partially satisfied: target absence and the clean fixture build are verified, and one bounded no-Skill implementation response was observed. General no-Skill behavior across the remaining scenarios is unverified. Do not describe the CLI launch failure as a model or Skill behavior failure.
