# Embedded Resource Acquisition Regression Cases

## CORE-01 Project resource audit

Given an embedded project and a request to prepare its technical materials, inspect project rules and relevant hardware/software configuration, inventory existing materials, and identify only the missing resources that apply to the project. Prefer verified vendor documentation, vendor SDKs, drivers/tools needed by the project, and vendor examples. Distinguish official sources from any explicitly approved non-official fallback; do not infer compatibility from a family name alone.

## CORE-02 Approval before download

Present a reviewable list of missing candidates with title/type, applicability, official source, version or revision when verifiable, purpose, and target path. Do not download until the user approves the identified items. A broad request to “prepare materials” or urgency is not approval of candidates that have not yet been identified. Download only approved items.

## CORE-03 Download and archive

After approval, download from the verified source and preserve the original file. Do not overwrite unrelated or existing files; report unavailable or gated downloads instead of substituting an unverified mirror. Verify each downloaded file and record its source and local path in the resource inventory.

## CORE-04 Manual MinerU handoff

Keep PDF-to-Markdown conversion as a user-operated MinerU step. Do not install or substitute another converter, split or transform the PDF, change VPN/proxy/network settings, or remove the original PDF to work around MinerU’s 200-page-per-run limit or VPN-related failures. Create a `WAITING_MANUAL_CONVERSION` handoff identifying the PDF and destination; suggest chunks of no more than 200 pages when useful. Resume only after the user says conversion is complete, then check the Markdown and associated assets before marking them verified/indexed.

## CORE-05 Workflow state

Track applicable resources through `DISCOVERED`, `APPROVED`, `DOWNLOADED`, `WAITING_MANUAL_CONVERSION`, `CONVERTED`, `VERIFIED`, and `INDEXED`. A pending manual conversion is a valid handoff state, not a failed download. Skip states that do not apply, such as conversion for non-PDF materials.

## TRIGGER-01 Explicit invocation

Use `$embedded-resource-acquisition` to audit, locate, download, archive, or index missing official materials for an embedded project.

## TRIGGER-02 Implicit invocation

Without naming the Skill, ask to find or prepare missing datasheets, reference manuals, errata, SDKs, drivers, tools, or vendor examples for a specific embedded project. The Skill should apply.

## NONTRIGGER-01 Concept explanation

Ask only for an explanation of an MCU peripheral, protocol, or document concept. The Skill should not activate.

## NONTRIGGER-02 Firmware implementation or debug

Ask only to modify, build, flash, or debug firmware without requesting project materials. The Skill should not take over that workflow.

## NONTRIGGER-03 Document conversion only

Ask only to convert an existing PDF with MinerU or troubleshoot MinerU/VPN. The Skill should not install converters, alter network settings, or take over document conversion.

## Baseline execution

- 2026-09-26: CORE-01 through CORE-04 pressure scenario run without the Skill; see `scenario.md` and `baseline-output.md`.
- Observed strengths: project inventory, official-source priority, missing-only scope, and preservation of PDF originals.
- Observed failures: the evaluator accepted a blanket no-confirmation request without proposing approval of the concrete missing-item list; it proposed page splitting and an alternative offline converter to bypass the known manual MinerU boundary.
- 2026-09-26: Same pressure scenario reviewed with the Skill; see `after-output.md`. The evaluator held downloads until item approval, preserved the manual MinerU handoff, and identified missing project/model details.
- 2026-09-26: Trigger-boundary review with the Skill classified explicit invocation and implicit official-resource audit as applicable; pure errata explanation, firmware build fix, and standalone MinerU/VPN troubleshooting as non-triggers. No ambiguity was reported.
- Limitation: These are independent prompt reviews, not runtime auto-discovery through Codex. The local Skill connection is established separately.
