# Extracting Chip Hardware Facts Baseline

## Environment

- Date: 2026-09-26
- Target Skill: does not exist yet
- Scenarios: `scenario.md`, Variants A and B
- Execution: two fresh independent evaluators; no target Skill or rubric supplied; no repository/network access; no side effects
- Sources: fictional excerpts included in the scenario

## Results

Both evaluators correctly found the candidate MCU pins, Datasheet electrical differences, APB1 clock, DMA mappings, IRQ names, distinction between DMA completion and USART `TC`, and revision-specific Errata condition. Both avoided claiming that the excerpts prove the board's actual U5 wiring and identified additional U5 details that still need checking.

Both failed to ask the user for a targeted local screenshot: Variant A recommended checking a schematic that the user said was missing; Variant B said the blurred schematic could not confirm the mapping and recommended checking the nets, but did not ask for a readable crop. Both supplied conventional MCU-to-transceiver connections as a checklist without explicitly marking each board-dependent connection `UNRESOLVED`. Both recommended applying the REV_A workaround while the silicon revision was unknown rather than stating that applicability was unresolved. Source names and some section identifiers appeared, but the answer did not use a consistent status and evidence-location contract.

## Baseline conclusion

The model can extract most relevant engineering facts, but does not consistently close the evidence chain when board documentation is missing or unreadable. The Skill should specifically require a focused screenshot request, separate chip capability from actual board wiring, preserve revision-specific Errata uncertainty, and make conclusion states and source locations visible.

## With-Skill comparison

Both fresh with-Skill evaluations requested a focused schematic crop and named the MCU pin/net labels, U5 signal pins, bus/interface connections, and related details needed for the task. Both kept actual board mapping `UNRESOLVED`, treated the `REV_A` Errata applicability as `UNRESOLVED`, cited the supplied document sections/tables, and continued with useful pin, clock, DMA, IRQ, electrical, and transmit-completion facts.

No new failure appeared in these two scenarios. Variant B also explicitly noted that the excerpts contained no page numbers. The output-location change was added afterward and verified separately.

## Markdown output-location extension

A second baseline used nested `Firmware/Src/` working directories and identical synthetic hardware sources. In the project with an explicit `AGENTS.md` report convention, the evaluator detected the project root, created the Markdown report in the instructed folder using the existing filename style, and returned its path. In the project without a convention, it provided a useful inline answer but created no report, so it did not exercise the fallback or collision behavior. The new output contract addresses this gap by requiring a Markdown report for every completed extraction, using project conventions first and a dated `docs/hardware-analysis/` fallback with collision-safe naming.

After the update, two fresh with-Skill evaluations created reports under the expected locations. The fallback report used a `-2` suffix and the pre-existing sentinel's SHA-256 stayed unchanged. Both reports kept the board wiring unresolved and asked for a focused schematic crop. See `output-location-baseline.md` and `output-location-after.md` for the fixture details and captured report bodies.
