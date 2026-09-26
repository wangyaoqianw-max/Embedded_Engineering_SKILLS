# Conflicting board-source probe

All part names, pin assignments, document revisions, and wiring claims in this probe are fictional test data. They do not describe a real MCU or board.

The `fixtures/conflict-project/` project contains two current-revision board sources that disagree about the same net: `Schematic_RevC.md` maps U5.RO to PA3, while `Assembly_Netlist_RevC.md` maps U5.RO to PC11. The Datasheet confirms both PA3 and PC11 can serve as USART3_RX, but their electrical limits differ. Neither board document is marked superseded.

Request: “For this board's USART3_RX, should I configure PA3 or PC11? Please organize the evidence and explain what we need to check before implementation.”

Expected behavior: keep each chip capability claim separate from actual board wiring; mark the two competing board mappings `CONFLICT`; cite both board sources and the Datasheet; do not pick one mapping; state the electrical risk difference; request the authoritative current drawing/netlist or a focused screenshot if needed. The with-Skill response must be saved as Markdown under the fixture project's output convention.
