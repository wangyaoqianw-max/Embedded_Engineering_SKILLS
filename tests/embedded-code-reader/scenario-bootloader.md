# Embedded Code Reader Bootloader Scenario

阅读 `fixtures/bootloader-state/` 中的 Bootloader OTA 代码，说明 `PENDING` 怎样安装并进入 `TRIAL`，Application 如何确认，以及未确认或回滚中复位时 Bootloader 如何选择启动路径。重点追踪状态提交顺序、擦除内部 APP 前的校验和失败/重试分支。生成带源码行号的 Markdown 阅读笔记。
