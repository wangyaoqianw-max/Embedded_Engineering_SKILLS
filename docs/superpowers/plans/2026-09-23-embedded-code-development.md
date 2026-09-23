# Embedded Code Development Skill Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox syntax for tracking.

**Goal:** Create and validate the embedded-code-development Skill for implementing, modifying, and porting embedded firmware code in existing projects.

**Architecture:** Keep the shared workflow, routing, project-priority rules, and sub-agent policy in SKILL.md. Put conditional CubeMX/Keil build integration and third-party porting detail in focused references. Validate behavior with an isolated fixture, a no-Skill baseline, and post-Skill regression scenarios.

**Tech Stack:** Markdown, PowerShell, existing GCC/Keil/EIDE build Skills, Skill Creator quick validator.

**Spec:** docs/superpowers/specs/2026-09-23-embedded-code-development-design.md

## Global Constraints

- Keep the Skill source under skills/embedded-code-development/.
- Keep test materials under tests/embedded-code-development/.
- Do not hardcode machine-specific absolute paths or toolchain locations.
- Target-project instructions and verified project facts take precedence over generic Skill guidance.
- Do not copy the Library C code standard or a firmware project's Keil standard into the Skill.
- Use only one writer for each file and tightly coupled module.
- Do not run or modify the user's real firmware project as part of fixture evaluation.
- Do not commit or push without explicit user authorization. The user authorized commit and push for this task.

## Review Focus

- Project-specific Keil output paths: every Target follows the fixture's local standard, even when its paths differ from common defaults.
- Failed initial CubeMX build: existing errors remain attributed to the baseline until evidence connects them to a later code change.
- Random object-file I/O failure: distinguish output/environment failures from compiler diagnostics and do not edit application code to hide them.
- Vendor and generated source: preserve provenance and license boundaries; ask when source rights or modification ownership are unclear.
- Parallel work boundaries: allow only independent workstreams with settled interfaces; keep coupled edits single-writer and integrate centrally.

---

### Task 1: Build the behavioral fixture and capture the no-Skill baseline

**Files:**

- Create: tests/embedded-code-development/regression-cases.md
- Create: tests/embedded-code-development/baseline.md
- Create: tests/embedded-code-development/baseline-output.md
- Create: tests/embedded-code-development/fixtures/cubemx-keil/README.md
- Create: tests/embedded-code-development/fixtures/cubemx-keil/AGENTS.md
- Create: tests/embedded-code-development/fixtures/cubemx-keil/Board.ioc
- Create: tests/embedded-code-development/fixtures/cubemx-keil/MDK-ARM/Board.uvprojx
- Create: tests/embedded-code-development/fixtures/cubemx-keil/00_Doc/Standards/Keil-project-standard.md
- Create: tests/embedded-code-development/fixtures/cubemx-keil/Core/Src/main.c
- Create: tests/embedded-code-development/fixtures/cubemx-keil/Core/Inc/main.h
- Create: tests/embedded-code-development/fixtures/cubemx-keil/.gitignore
- Create: tests/embedded-code-development/fixtures/cubemx-keil/third_party/sample_port/LICENSE
- Create: tests/embedded-code-development/fixtures/cubemx-keil/third_party/sample_port/sample_port.c
- Create: tests/embedded-code-development/fixtures/cubemx-keil/third_party/sample_port/sample_port.h
- Create: tests/embedded-code-development/fixtures/cubemx-keil/third_party/unverified_port/sample_port.c
- Create: tests/embedded-code-development/fixtures/cubemx-keil/logs/baseline-compile-error.log
- Create: tests/embedded-code-development/fixtures/cubemx-keil/logs/random-object-io-error.log

**Interfaces:**

- Consumes: the confirmed design specification and repository Skill regression conventions.
- Produces: a self-contained, sanitized project fixture and recorded behavior without the target Skill.

- [x] **Step 1: Create the fixture's project instructions and directory layout**

Create the README, fixture AGENTS.md, directory skeleton, and narrow .gitignore. State that this fixture represents a freshly generated CubeMX project and that its project-local build standard is authoritative for its own files.

- [x] **Step 2: Add the CubeMX and Keil project inputs**

Create Board.ioc and an MDK-ARM project with two Targets. Define distinct output locations for both Targets in the fixture's Keil standard so a general-purpose default path cannot accidentally pass. Keep compiler and installation paths unset or relative.

- [x] **Step 3: Add application and porting samples**

Create the small application C module, one third-party component with a local license, and a second component whose source and license are unknown. Do not copy real vendor or company code into the fixture.

- [x] **Step 4: Add build-log evidence**

Add one representative pre-existing compiler diagnostic and one random object-file I/O failure without a source diagnostic. Label both as supplied historical logs so a scenario can test classification separately from an actual fixture build.

- [x] **Step 5: Write behavior scenarios**

In regression-cases.md, define these concrete requests against the fixture:

1. Add a small application feature and synchronize its source into the Keil project.
2. Start from the fresh CubeMX project, run the prescribed initial Keil build before editing when the backend is available, inspect both Targets, and use baseline-compile-error.log to verify that a supplied pre-existing compiler failure is recorded as baseline evidence.
3. Port the licensed sample component while preserving its license and upstream boundary; decline to integrate the second component until source and license are confirmed.
4. Compare the supplied historical compiler diagnostic with the random object-file write error and preserve their different classifications.
5. Compare two independent driver workstreams with two edits to the same UART state machine.
6. Verify non-triggers for source-only explanation, architecture design, build-only, unknown runtime debugging, standalone testing, and code review.

- [ ] **Step 6: Run the scenarios without the target Skill**

Use isolated copies of the fixture and a clean evaluator context that has not loaded embedded-code-development. Run the fresh-project and porting scenarios, then record the actual responses and changed files in baseline-output.md. Do not prescribe or seed the expected omissions in the evaluator prompt.

The bounded CORE-01 request was run by an isolated no-Skill sub-agent. Fresh-project behavior, porting, and no-Skill routing boundaries were not run in that evaluator, so this step remains incomplete.

- [x] **Step 7: Record evidence-based baseline findings**

In baseline.md, record the evaluator context, exact scenarios, observed successful behavior, observed omissions or misrouting, and limitations. Do not claim a failure for any criterion the baseline handled consistently. Keep the fixture unchanged after each run by discarding its isolated working copy.

**Verification:** baseline-output.md contains the actual no-Skill output; baseline.md links each conclusion to observable evidence; all fixture files remain unchanged.

### Task 2: Implement the Skill entrypoint and focused references

**Files:**

- Create: skills/embedded-code-development/SKILL.md
- Create: skills/embedded-code-development/references/project-build-integration.md
- Create: skills/embedded-code-development/references/code-porting.md
- Verify: docs/superpowers/specs/2026-09-23-embedded-code-development-design.md
- Verify: tests/embedded-code-development/baseline.md

**Interfaces:**

- Consumes: the confirmed specification and baseline evidence.
- Produces: a discoverable Skill with explicit mode routing and task-specific reference guidance.

- [x] **Step 1: Write the Skill frontmatter and routing boundary**

Set the Skill name to embedded-code-development. Describe implementation, modification, and porting of embedded code in existing projects. Exclude source-only reading, architecture design, independent build requests, unknown runtime debugging, standalone testing, and code review.

- [x] **Step 2: Add the shared implementation workflow**

Write the concise entrypoint rules for readiness, project-context inspection, information priority, minimal changes, project-file synchronization, build-tool handoff, and test-status reporting. Route unclear architecture to embedded-design-decision and source-only analysis to embedded-code-reader.

- [x] **Step 3: Add the fresh CubeMX/Keil build reference**

Document how to discover and follow project-local Keil standards, check each Target, run the prescribed initial build before editing a fresh CubeMX project, compare the final build with the baseline, and classify source, configuration, toolchain, and output I/O failures. Keep output-directory values project-defined.

- [x] **Step 4: Add the third-party porting reference**

Document source/version/license/dependency review, target-platform checks, preservation of upstream code, use of official configuration or adapters, minimal core modifications, and destination decisions based on reuse scope.

- [x] **Step 5: Add the sub-agent ownership rules**

Include the independent-workstream threshold, single-writer constraints, main-agent ownership of architecture and shared interfaces, and integration checks. Keep this shared rule in SKILL.md rather than creating a separate router.

- [x] **Step 6: Cross-check against baseline evidence**

For each baseline omission supported by evidence, identify the exact entrypoint rule or reference section that addresses it. Remove rules that do not change a decision or satisfy the approved specification.

**Verification:** every reference is linked from SKILL.md; no copied project standard, absolute path, scaffold text, or unrelated workflow is present.

### Task 3: Run post-Skill behavior and trigger regressions

**Files:**

- Modify: tests/embedded-code-development/regression-cases.md
- Create: tests/embedded-code-development/after-output.md
- Verify: skills/embedded-code-development/SKILL.md
- Verify: skills/embedded-code-development/references/project-build-integration.md
- Verify: skills/embedded-code-development/references/code-porting.md

**Interfaces:**

- Consumes: the Skill, references, fixture, and baseline findings.
- Produces: evidence of explicit and implicit routing, core behavior, and non-trigger boundaries.

- [ ] **Step 1: Verify explicit and implicit invocation**

Run one explicit request that names embedded-code-development and one ordinary request to implement a feature in the fixture without naming the Skill. Confirm both enter the intended workflow and touch only the requested fixture files.

Native Codex routing could not be exercised in this environment; see after-output.md. Repository installation alone is not evidence of runtime activation.

- [x] **Step 2: Verify fresh CubeMX/Keil preflight**

Run the fresh-project scenario with the fixture's local standard. Confirm the evaluator reads the standard, checks both Target output paths, attempts the prescribed initial build before source edits, reports the observed build result, and compares the post-change build against that baseline. If the fixture cannot be built by the available Keil tool, record the limitation and require the evaluator to report it as unverified.

The main agent performed a manual workflow check on an isolated fixture copy. Both pre-edit and post-edit Debug/Release builds produced source activity, link results, and AXF artifacts with zero errors and warnings. This does not verify native Skill routing.

- [ ] **Step 3: Verify porting and error classification**

Run the port scenario and the I/O-error scenario. Confirm the license is retained, source provenance is recorded, upstream code is preserved where possible, and an output-only write failure does not trigger an unrelated C source edit.

- [ ] **Step 4: Verify delegation and non-trigger boundaries**

Confirm independent workstreams are eligible for parallel work only after their interfaces are settled, while same-module state-machine changes remain single-writer. Run the non-trigger requests listed in regression-cases.md and record routing decisions in after-output.md.

- [ ] **Step 5: Review the integrated result**

Compare the complete outputs against the required and prohibited observations in regression-cases.md. Record any unsupported behavior as unverified rather than passing. Update the Skill only for a demonstrated gap and rerun the affected scenario.

**Verification:** after-output.md records actual outputs, touched files, test limitations, and a criterion-by-criterion result; fixture source and project standards remain unchanged.

### Task 4: Validate Skill structure and complete the repository review

**Files:**

- Verify: skills/embedded-code-development/SKILL.md
- Verify: skills/embedded-code-development/references/project-build-integration.md
- Verify: skills/embedded-code-development/references/code-porting.md
- Verify: tests/embedded-code-development/baseline.md
- Verify: tests/embedded-code-development/regression-cases.md
- Verify: tests/embedded-code-development/after-output.md

**Interfaces:**

- Consumes: the completed Skill and recorded behavior evidence.
- Produces: a reviewable Skill change set with accurate validation notes.

- [x] **Step 1: Run the Skill Creator validator**

Locate the active Skill Creator quick_validate.py in the execution environment and run it against skills/embedded-code-development with UTF-8 mode enabled on Windows. Record its actual result; do not add validator paths to repository files.

- [x] **Step 2: Check description and reference routing**

Confirm the frontmatter clearly distinguishes code implementation from source-reading, design, build-only, debug, test, and review requests. Open each linked reference and verify that its guidance matches the workflow and approved specification.

- [x] **Step 3: Review portability and scope**

Search the new Skill, tests, and fixture for machine-specific absolute paths, credentials, real project code, copied standards, generic output-directory assumptions, and unapproved operations. Remove any such content.

- [x] **Step 4: Review the final change set**

Read every added file in UTF-8, confirm the regression records match the actual runs, and inspect Git status and the complete added-file contents. Commit and push only under the user's explicit authorization, which has been given for this task.

**Verification:** the validator result and remaining unverified cases are reported accurately; no real firmware project or generated build outputs were modified.
