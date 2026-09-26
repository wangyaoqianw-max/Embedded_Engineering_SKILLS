# Trigger-boundary response probes

## Procedure

Each prompt was sent in a separate fresh context. The evaluator did not inspect a project or use the network. The resource-acquisition and firmware probes were routing-only: they did not download files, modify firmware, or build. These are model response probes; native Codex automatic dispatch remains unverified because the local `codex exec` process failed to initialize its app-server client.

## NONTRIGGER-01: Concept-only request

**Prompt:** “What does UART DMA mean conceptually, and why would a UART use it?”

**Response:**

> UART DMA lets the UART peripheral transfer received or transmitted bytes directly between its data register and memory, with little CPU involvement.
>
> It’s useful for moving larger or continuous data streams efficiently: the CPU doesn’t have to handle every byte, can work on other tasks, and is less likely to miss incoming data. The UART still sends and receives the bits; DMA takes care of moving the bytes to or from memory.

**Result:** Routed as a direct concept explanation; no project-specific document workflow was used.

## NONTRIGGER-02: Resource acquisition only

**Prompt:** “Find and download the missing manufacturer's datasheet and archive it for the project.”

**Response:**

> 我会按 `embedded-resource-acquisition` 流程处理。请提供目标工程目录，或芯片/器件型号和资料归档目录。我会先确认缺失的数据手册及官方来源，再列出版本和目标路径供你确认后下载。

**Result:** Routed to `embedded-resource-acquisition`; no hardware extraction, download, or archive write was performed.

## NONTRIGGER-03: Firmware implementation and build

**Prompt:** “Implement USART3 DMA RS485 in the existing firmware and build it.”

**Response:**

> 请提供现有固件工程目录，或切换到包含工程源码的工作区。拿到工程后，我会按 `embedded-code-development` 流程实施，再根据工程配置选择 EIDE、Keil 或 GCC 构建。
>
> 使用路径：`embedded-code-development`；构建时按项目类型选择 `eide`、`keil` 或 `gcc`。

**Result:** Routed to firmware implementation/build workflows; hardware extraction may support the work if project-specific hardware facts are needed, but does not take over implementation or building.
