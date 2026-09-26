---
name: embedded-resource-acquisition
description: Use when an embedded project needs an audit of missing vendor datasheets, reference manuals, errata, SDKs, drivers, or official examples, or their approved download and indexing; not for firmware implementation, build/debug, concept explanations, or standalone document conversion.
---

# 嵌入式项目资料准备

为具体嵌入式项目盘点现有技术资料，识别缺项，查找可信来源，并在授权后下载归档。PDF 转 Markdown 使用用户手动操作的 MinerU 流程；本 Skill 负责交接和结果核对，不负责转换。

## 适用范围

- 芯片、器件和开发板资料：datasheet、Reference Manual、Programming Manual、errata、应用笔记、用户手册、原理图、BOM、引脚和硬件设计指南。
- 项目实际使用的官方软件包、SDK、CMSIS、HAL/LL、BSP、中间件、驱动和开发工具。
- 芯片厂商或板卡厂商发布的官方示例和参考实现。

不用于知识蒸馏、工程设计、源码分析、代码修改、构建烧录调试、软件大全收集、自动 OCR 或独立的 PDF 转换与 VPN 排障。

## 工作流

1. 确认目标工程。读取适用的 `AGENTS.md`、README 和相关硬件、芯片、软件栈配置；按目标检查已有资料目录和索引，不做无目标的全盘扫描。若目标工程或资料目标目录无法确定，先询问。
2. 根据已核对的器件型号、板卡和软件栈建立资料需求清单。区分已存在、版本可能过期、缺失和当前不适用项；型号或版本不能确认时标为待确认，不按系列名猜测。
3. 搜索并核实当前来源。优先芯片/器件厂商官网，其次厂商官方 GitHub/GitLab、官方 SDK/Package 和工具站点。官方来源无法取得时，可把可信上游候选作为 `official: false` 单独列出；不得冒充官方或默认下载。
4. 下载前给用户一份可审核清单，至少列出资料名称和类型、适用理由、来源链接、可核实的版本/修订、目标路径及状态。等待用户批准具体候选后再下载。只有当用户已明确指定具体资料和目标路径并要求下载时，才视为该项已批准；“把缺失资料都下载”或时间紧不等于批准尚未识别的候选。
5. 只下载已批准的项目，优先使用官方原始文件。保留原件，不覆盖或删除已有文件；遇到登录、许可协议、来源失效或访问失败时停止该项并报告，不代替用户接受协议、不改用未经批准的镜像。检查下载文件可读取且非空，记录来源和本地路径。
6. 更新项目已有资料索引；若没有索引，在已确认的资料目录内建立简洁索引，记录名称、类型、版本、来源、相对路径和状态。不要把资料索引扩写成技术内容总结或设计文档。
7. 对下载的 PDF 建立 MinerU 人工转换清单，至少记录 PDF 路径、预期 Markdown 位置和状态 `WAITING_MANUAL_CONVERSION`。可核实时记录页数，并建议用户按每次不超过 200 页分片；页数未知时明确标未知。告知用户转换需要手动完成，然后停止该 PDF 的转换工作。
8. 用户通知 MinerU 转换完成后，检查 Markdown 和关联图片等文件是否存在、非空、路径可用，并更新索引状态。只核对转换产物，不据此声称内容识别完全准确。

## MinerU 边界

- 用户使用 MinerU 手动完成 PDF 转换；`WAITING_MANUAL_CONVERSION` 是正常交接状态，不是下载失败。
- 不得自动调用 MinerU，不得安装或替换 PDF 转换器，不得自行拆分、转换或 OCR PDF。
- 不得修改 VPN、代理或系统网络设置来绕过下载/转换问题。
- 始终保留 PDF 原件。超过 200 页时只建议由用户分片处理，不代替用户操作。

## 资料状态

按单项资料跟踪适用状态：`DISCOVERED` → `APPROVED` → `DOWNLOADED` → `WAITING_MANUAL_CONVERSION` → `CONVERTED` → `VERIFIED` → `INDEXED`。无须转换的资料可从 `DOWNLOADED` 进入 `VERIFIED`；未授权下载的候选保持 `DISCOVERED`。完成后汇报已下载、待人工转换、已核验和未能获取的项目。
