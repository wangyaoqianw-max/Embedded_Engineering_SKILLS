# Engineering Experience

本文件保存尚未升级为稳定规则的工程经验。经验只在上下文相似时作为参考，不能覆盖项目事实或 `decision-framework.md`。

## 记录字段

- ID：`EXP-NNN`
- 标题
- 来源场景
- 观察到的问题
- 采用的决策
- 实际结果
- 适用条件
- 不适用条件
- 证据
- 状态：候选、已验证、已废弃

记录必须去除密钥、个人隐私、公司内部代码和其他保密材料。无法说明适用边界的经验不得进入稳定框架。

## 状态转换

- 新经验进入“候选”。
- 能形成可复现场景时加入回归案例。
- 多次验证且适用边界明确后，提炼到 `decision-framework.md` 并标为“已验证”。
- 被反例推翻或由新事实取代时标为“已废弃”，保留原因。

## 当前记录

## EXP-001 FreeRTOS 应用启动组合与一次性启动入口

- 来源场景：个人 STM32F407VET6 综合项目复用 `STM32F4_Bootloader_OTA_Test_Project` 的 Application 启动结构；目标工程要求保留 CubeMX 生成的 `defaultTask`，并希望由一次性启动任务组织应用初始化。
- 观察到的问题：容易把 `app_system` 模块名误当成固定的 RTOS 任务名，或把组件启动、就绪判断和运行期管理混在一个任务入口中；长期任务若未报告本地初始化结果，系统也无法形成一致的启动结论。
- 采用的决策：区分启动入口和应用组合模块。来源工程由 `StartDefaultTask` 调用 `app_system_bootstrap()`，成功后自删除；`app_system.c` 创建启动同步对象和队列、启动长期组件任务、等待带超时的组件启动屏障，并裁决 `RUNNING`、`DEGRADED` 或 `FAILED`；`app_startup.c` 保存组件初始化结果和启动决策事件。对要求保留 `defaultTask` 的新工程，可让其保持非业务等待职责，并新增一次性 `appSystemTask` 调用应用组合模块；复用启动上下文、组件就绪报告、有限等待和系统状态裁决，完成后由 `appSystemTask` 自删除。外设运行期所有权仍属于各长期任务。保留 `defaultTask` 是目标工程约束，不是所有 CubeMX 工程的通用要求。
- 实际结果：上述来源结构已通过源码核对；STM32F407VET6 目标工程目前处于设计阶段，尚无固件运行或实机验证结果。
- 适用条件：FreeRTOS 应用包含多个长期组件，启动存在依赖顺序，且需要汇总组件就绪状态后再发布系统级启动结论。
- 不适用条件：单任务或简单主循环即可完成启动的工程；目标工程明确要求 `defaultTask` 常驻承担运行期职责；静态任务在调度器启动前已统一创建、且不需要额外启动屏障的工程。
- 证据：[来源工程 freertos.c](https://github.com/wangyaoqianw-max/STM32F4_Bootloader_OTA_Test_Project/blob/main/03_Firmware/Application/OTA_APP/Core/Src/freertos.c)、[app_system.c](https://github.com/wangyaoqianw-max/STM32F4_Bootloader_OTA_Test_Project/blob/main/03_Firmware/Application/OTA_APP/01_APP/system/app_system.c)、[app_startup.c](https://github.com/wangyaoqianw-max/STM32F4_Bootloader_OTA_Test_Project/blob/main/03_Firmware/Application/OTA_APP/01_APP/system/app_startup.c)。
- 状态：候选。
