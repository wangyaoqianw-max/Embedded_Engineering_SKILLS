# Bootloader OTA 源码阅读笔记

## 阅读范围

- 目标：说明 `PENDING` 镜像安装并进入 `TRIAL` 的过程、Application 的确认接口，以及未确认或回滚中复位时 Bootloader 的选择路径。
- 已阅读范围：`tests/embedded-code-reader/fixtures/bootloader-state/` 下的 `include/boot_metadata.h`、`src/boot_main.c`、`src/installer.c`、`src/metadata.c`。
- 未覆盖范围：上述 fixture 未提供的平台 Flash、镜像辅助函数实现、Application 调用点、构建配置和硬件行为。
- 源码版本与分析日期：回归 fixture 位于未提交工作树中，没有独立 Git 提交号；分析日期：2026-09-23。

## 流程概览

1. `boot_main_run()` 读取最新有效元数据；若读取失败则停机。它按元数据状态分支处理，流程完成后调用应用跳转入口（`src/boot_main.c:10-20,43-49`）。
2. `PENDING` 时先校验候选槽的状态、槽位、外部头部 CRC、负载 CRC 和向量表；随后擦除内部 APP，复制并检查回读、内部 CRC 与向量表。全部成功后才提交 `TRIAL`（`src/boot_main.c:21-28,51-56`；`src/installer.c:3-16,34-46`）。
3. Application 可通过 `metadata_confirm_application()` 确认：仅当最新状态为 `TRIAL` 且 pending 槽非空时，将其提交为新的 confirmed 槽，并清除 pending（`src/metadata.c:90-101`）。fixture 没有展示 Application 何时、由谁调用该接口。
4. 若进入 Bootloader 时状态仍是 `TRIAL`，代码把它视为未确认并启动回滚；若已是 `ROLLBACK`，则从已确认外部槽重新执行恢复。恢复成功后提交 `NONE`，最后进入应用跳转入口（`src/boot_main.c:30-41,59-74`）。

## 文件与符号索引

|职责|文件 / 符号|证据|
|---|---|---|
|状态与镜像数据结构|`include/boot_metadata.h`：`boot_state_t`、`boot_metadata_t`、`boot_image_t`|`include/boot_metadata.h:7-31`|
|启动状态分派与跳转|`src/boot_main.c`：`boot_main_run()`|`src/boot_main.c:10-49`|
|安装、恢复前置校验与内部 APP 写入|`src/installer.c`：`candidate_prevalidate()`、`confirmed_prevalidate()`、`installer_install_pending()`、`installer_restore_confirmed()`|`src/installer.c:3-60`|
|双副本选择与状态提交|`src/metadata.c`：`metadata_load_latest()`、`metadata_commit_transition()` 及公开提交函数|`src/metadata.c:9-101`|

## 配置与初始化

fixture 没有构建配置或条件编译分支可用于确认目标槽、Flash 布局或平台实现。启动时直接加载元数据，并由 `metadata.state` 选择路径；`boot_metadata.h:7-12` 定义 `NONE`、`PENDING`、`TRIAL`、`ROLLBACK` 四种状态。`boot_main_run()` 读取并记录复位原因，但记录函数为空桩，读取函数固定返回 `0U`，且该值不参与状态分支（`src/boot_main.c:13-20,76-79`）。

## 运行时调用链

- **PENDING 安装：** `boot_main_run()` → `install_candidate()` → `candidate_prevalidate()` → `installer_install_pending()` → 再次调用 `candidate_prevalidate()` → `internal_app_erase()` → `copy_payload_and_check_readback()` → `internal_app_crc_ok()` → `internal_app_vector_is_valid()` → `metadata_commit_trial()`（`src/boot_main.c:21-28,51-56`；`src/installer.c:34-46`）。安装前置校验在 `install_candidate()` 与 `installer_install_pending()` 各执行一次。
- **Application 确认：** 可见的接口链为 `metadata_confirm_application()` → `metadata_load_latest()` → `metadata_commit_transition()`；实际 Application 调用点不在 fixture 中（`src/metadata.c:90-101,32-50`）。
- **未确认后的回滚：** `TRIAL` 分支 → `rollback_confirmed()` → `confirmed_prevalidate()` → `metadata_commit_rollback_begin()` → `installer_restore_confirmed()`（内部再次前置校验）→ 擦除、复制及内部校验 → `metadata_commit_rollback_complete()`（`src/boot_main.c:30-34,59-74`；`src/installer.c:18-32,48-60`）。
- **回滚中再次复位：** `ROLLBACK` 分支再次调用 `rollback_confirmed()`；不会再次提交 rollback-begin，而会重新校验已确认镜像并重做恢复（`src/boot_main.c:36-41,59-74`）。

## 数据流与状态变化

`metadata_load_latest()` 要求元数据副本读取成功，并分别检查副本的 committed marker 与 body CRC；两份都无效时返回失败，否则按 sequence 选择较新副本，相同 sequence 时选择 A（`src/metadata.c:9-30`）。`metadata_commit_transition()` 先验证当前状态，再复制最新记录、递增 sequence、更新状态和槽位，并交给 `metadata_write_inactive_copy()` 写入（`src/metadata.c:32-50`）。注释描述的写入顺序是：使非活动副本失效、写入 body 与 CRC、校验，最后写 marker；底层写入实现未包含在 fixture 中。

|当前状态|处理与提交顺序|失败或再次复位时可从源码确认的路径|
|---|---|---|
|`PENDING`|候选镜像前置校验通过后才擦除内部 APP；写入及内部回读、CRC、向量表检查全部成功后，`metadata_commit_trial()` 将状态改为 `TRIAL`，保留原 confirmed 与 pending 槽（`src/boot_main.c:21-28`；`src/metadata.c:53-65`）。|候选前置校验、擦除、复制/回读或内部检查任一步失败，安装返回失败并进入停机循环；在执行 trial 提交之前没有其他元数据状态提交。再次启动若仍读到 `PENDING`，会重新走安装流程。若 trial 提交失败，当前运行停机；其后实际选择取决于元数据写入 helper 的提交结果，helper 实现未提供。|
|`TRIAL`|启动时先校验 confirmed 外部镜像，再提交 `ROLLBACK`，然后才进入恢复安装；恢复所有校验通过后提交 `NONE`，confirmed 保持原槽、pending 清空（`src/boot_main.c:30-34,59-74`；`src/metadata.c:67-88`）。|确认接口未成功提交前，下一次进入 Bootloader 仍按 `TRIAL` 走回滚。回滚开始提交失败时不会调用恢复安装。恢复失败时不会提交完成状态；下一次若状态为 `ROLLBACK`，会重试恢复。|
|`ROLLBACK`|从 confirmed 外部槽恢复到内部 APP；成功后提交 `NONE`，清除 pending（`src/boot_main.c:36-41,59-74`；`src/metadata.c:79-88`）。|恢复失败或完成状态提交失败时当前启动停机；若最新有效记录仍为 `ROLLBACK`，下次启动重做恢复。|
|Application 确认|`metadata_confirm_application()` 只接受 `TRIAL` 且 pending 非空；新记录把 pending 槽设为 confirmed、状态设为 `NONE`，并清除 pending（`src/metadata.c:90-101`）。|参数/状态条件不满足或提交失败时返回 `false`。实际调用者和调用时机未显示。|
|`NONE`|无额外状态操作，流程进入应用跳转入口（`src/boot_main.c:43-49`）。|`boot_jump_to_app()` 在 fixture 中为空函数，实际跳转未验证。|

擦除前的候选校验检查非空参数、状态必须为 `PENDING`、pending 槽不能为 `SLOT_NONE` 且不能等于 confirmed 槽，再执行外部 header CRC、payload CRC 和向量表校验（`src/installer.c:3-16`）。确认槽恢复前也检查状态、confirmed 槽及其与 pending 槽的关系，并执行同类外部校验（`src/installer.c:18-32`）。两个安装函数都在 `internal_app_erase()` 之前完成前置校验；擦除失败会直接返回失败，擦除后则按短路顺序执行复制/回读、内部 CRC 和向量表检查（`src/installer.c:34-60`）。

`boot_image_t` 包含 `source_slot`、`image_size`、`payload_crc`；可见的前置校验只设置 `source_slot`，没有给另外两个字段赋值，而后续写入和内部 CRC helper 接收该结构指针（`include/boot_metadata.h:27-31`；`src/installer.c:12-15,28-31,43-45,57-59`）。若这些 helper 读取未赋值字段，其值不可靠；需查看 helper 实现或补充测试才能确认实际影响。

## 模块协作

启动分派层依赖元数据层决定安装、试运行或回滚路径；安装层负责校验外部镜像并执行内部 APP 擦除、写入和验证；元数据层用状态与槽位记录协调跨启动恢复。Application 与元数据层的可见接口是 `metadata_confirm_application()`。平台 Flash、外部镜像读取和最终跳转通过未实现的 helper/桩隔离，fixture 无法证明其硬件效果。

## 未确认事项

- `metadata_read_copies()`、`metadata_write_inactive_copy()`、marker/body CRC 检查及平台 Flash 操作只有声明或调用点；断电时的实际持久性、写失败后的副本内容和重试保证未验证。源码注释提供了预期写入顺序，但不足以确认硬件行为。
- `external_*`、`internal_*`、复制/回读、擦除和向量表 helper 的实现不可见；CRC 覆盖范围、镜像长度来源、地址边界和 helper 对 `boot_image_t` 字段的依赖未确认。
- `image_size` 与 `payload_crc` 在可见前置校验中未初始化；需确认 helper 是否读取它们，以及字段应由何处填充。
- Application 如何获知试运行状态、何时调用确认接口，以及确认提交与复位之间的实际时序未展示。
- `boot_jump_to_app()` 是空桩；复位控制器行为、真实复位原因、外部槽硬件访问和断电恢复均未验证。

## 建议阅读顺序

1. `tests/embedded-code-reader/fixtures/bootloader-state/include/boot_metadata.h` — 先确认状态、槽位和镜像数据结构。
2. `tests/embedded-code-reader/fixtures/bootloader-state/src/boot_main.c` — 从启动入口读状态分派、安装提交点和复位恢复路径。
3. `tests/embedded-code-reader/fixtures/bootloader-state/src/installer.c` — 核对擦除前的校验、内部写入顺序及失败返回。
4. `tests/embedded-code-reader/fixtures/bootloader-state/src/metadata.c` — 追踪双副本选择、状态提交和 Application 确认条件。
