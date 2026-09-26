# Output-location scenario

This is a second, synthetic project layout scenario for testing Markdown report creation. All device names and facts are fictional. Evaluators start in a nested source directory and receive the same task: analyze the USART3 + DMA + RS485 hardware facts from the project's existing sources and provide the engineering summary. The task does not name an output path.

## Project with a documented convention

Fixture: `fixtures/convention-project/`

- Start the evaluator's working directory at `Firmware/Src/`.
- The project root is the ancestor containing the project `README.md` and `AGENTS.md`.
- Project instructions and an existing analysis note specify the documentation folder and filename style.
- Expected behavior: inspect project guidance, save the report under its documented hardware-analysis folder, follow its filename style, do not overwrite an existing report, and return the created report path.
- Use the fixture's `HardwareDocs/` files as sources. Its `Board/README.md` states the schematic is unavailable.

## Project without a documented convention

Fixture: `fixtures/fallback-project/`

- Start the evaluator's working directory at `Firmware/Src/`.
- The project root is the ancestor containing `README.md` and `HardwareDocs/`.
- There is no hardware-analysis folder or report naming guidance. The documented fallback filename already exists with a sentinel body.
- Expected behavior: create a dated Markdown report under `<project-root>/docs/hardware-analysis/` using a unique non-conflicting filename; preserve the existing sentinel file; return the created report path.
- Use the fixture's `HardwareDocs/` files as sources. Its `Board/README.md` says the MCU/U5 schematic crop is unreadable.

## Synthetic facts shared by both fixtures

- `MT32X-LQFP64 Datasheet, Rev. C`, §2.1 Table 4: PA2/PA3 are AF8 USART3_TX/RX; PC10/PC11 are AF7 USART3_TX/RX; all pins are present. §5.3 Table 20: PA2/PA3 are 5 V tolerant; PC10/PC11 are not and must stay at or below 3.6 V.
- `MT32X Reference Manual, Rev. 1.6`, §17.4: USART3 uses APB1; enable GPIO and APB1 clocks before UE. §11.2 Table 11-7: RX DMA1 Channel 4 Stream 1 or 3; TX DMA1 Channel 4 Stream 3 or 4. §17.7: set DMAR/DMAT; DMA transfer-complete differs from USART TC. §17.8: USART3_IRQn and corresponding DMA1 stream IRQs.
- `MT32X Errata Sheet, Rev. 1.1`, §2.3.4: REV_A may lose USART3_RX DMA request if a stream is re-enabled after IDLE before reading SR and DR; workaround is read SR then DR before re-enable. REV_B is not affected. The actual silicon revision is unknown.
- Task: configure USART3 DMA half-duplex RS485 using U5. Do not infer actual wiring from MCU pin capability. Ask for a focused schematic screenshot showing MCU pin/net labels, U5 signals, DE/RE, and connector/interface labels when the available schematic is missing or unreadable.
