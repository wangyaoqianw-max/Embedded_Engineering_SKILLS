# Bootloader OTA 源码阅读笔记

## 状态与启动入口

元数据记录序号、当前状态、已确认槽和待安装槽；状态包括 `NONE`、`PENDING`、`TRIAL`、`ROLLBACK`。`boot_main_run()` 先读取并记录复位原因，再加载最新有效元数据；读取失败就停机。随后按状态处理，处理完成后跳转到 APP。[`boot_metadata.h:7-25`](fixtures/bootloader-state/include/boot_metadata.h) [`boot_main.c:10-49`](fixtures/bootloader-state/src/boot_main.c)

| 当前状态 | Bootloader 行为 |
| --- | --- |
| `PENDING` | 校验候选镜像、安装到内部 APP、提交 `TRIAL`；任一步失败则停机。 |
| `TRIAL` | 校验已确认槽中的镜像，进入回滚并恢复它；失败则停机。 |
| `ROLLBACK` | 再次从已确认槽恢复，完成后提交 `NONE`；失败则停机。 |
| `NONE` | 不做额外处理，跳转 APP。 |

## PENDING 安装顺序

`install_candidate()` 先调用 `candidate_prevalidate()`，成功后才调用安装函数。[`boot_main.c:51-57`](fixtures/bootloader-state/src/boot_main.c)

候选镜像必须来自非空、且不同于已确认槽的待安装槽，元数据状态也必须是 `PENDING`。随后检查外部镜像头 CRC、载荷 CRC 和向量表有效性。[`installer.c:3-16`](fixtures/bootloader-state/src/installer.c)

安装函数会再次做候选镜像预校验，然后才擦除内部 APP。擦除成功后复制载荷并检查回读，再检查内部 APP 的 CRC 和向量表。[`installer.c:34-46`](fixtures/bootloader-state/src/installer.c)

安装完全成功后，Bootloader 才调用 `metadata_commit_trial()`。它确认最新元数据仍是 `PENDING` 且槽号匹配，然后把状态改为 `TRIAL`，保留原已确认槽和待安装槽。[`boot_main.c:20-28`](fixtures/bootloader-state/src/boot_main.c) [`metadata.c:53-65`](fixtures/bootloader-state/src/metadata.c)

元数据转换会递增序号并写入非活动副本；注释说明写入顺序是先使该副本无效，再写主体和 CRC、验证，最后写提交标记。[`metadata.c:32-50`](fixtures/bootloader-state/src/metadata.c)

如果外部预校验、擦除、复制回读、内部 CRC 或向量表检查失败，安装返回失败且本次启动停机，`TRIAL` 不会在后续步骤提交。如果安装成功但 `TRIAL` 提交失败，本次也会停机。[`boot_main.c:21-27`](fixtures/bootloader-state/src/boot_main.c) [`installer.c:37-45`](fixtures/bootloader-state/src/installer.c)

## Application 确认与回滚

Application 可调用 `metadata_confirm_application()` 确认试运行版本。此接口只接受 `TRIAL` 且待安装槽非空的元数据；成功时把待安装槽设为已确认槽、清空待安装槽，并将状态改为 `NONE`。[`boot_metadata.h:37`](fixtures/bootloader-state/include/boot_metadata.h) [`metadata.c:90-101`](fixtures/bootloader-state/src/metadata.c)

若设备在确认前复位，元数据仍为 `TRIAL`，Bootloader 会走 `rollback_confirmed()`：先验证已确认槽中的外部镜像，再提交 `ROLLBACK`，之后擦除内部 APP 并恢复该镜像。恢复成功后提交回滚完成状态 `NONE`，清空待安装槽。[`boot_main.c:59-74`](fixtures/bootloader-state/src/boot_main.c) [`metadata.c:67-88`](fixtures/bootloader-state/src/metadata.c)

若恢复过程中复位，启动时读到 `ROLLBACK`，会再次校验已确认槽并重新执行恢复；该分支不会重复提交 `ROLLBACK`。恢复任何阶段失败都会停机，之后再次复位时可从保留的状态重新进入恢复路径。[`boot_main.c:36-41`](fixtures/bootloader-state/src/boot_main.c) [`installer.c:48-60`](fixtures/bootloader-state/src/installer.c)

恢复路径也在擦除之前验证外部已确认镜像的头 CRC、载荷 CRC 和向量表。之后执行擦除、复制回读、内部 CRC 和向量表检查。[`installer.c:18-32`](fixtures/bootloader-state/src/installer.c) [`installer.c:48-60`](fixtures/bootloader-state/src/installer.c)

## 阅读边界

代码读取并记录复位原因，但状态分支没有使用该值；选择启动路径看的是元数据状态。[`boot_main.c:13-20`](fixtures/bootloader-state/src/boot_main.c)

本 fixture 展示了 Application 确认接口，但没有展示 Application 何时调用它。平台擦除、复制、CRC 与向量表检查函数也只有声明，具体实现和失败原因无法从这些文件确认。[`metadata.c:90-101`](fixtures/bootloader-state/src/metadata.c) [`installer.c:62-69`](fixtures/bootloader-state/src/installer.c)
