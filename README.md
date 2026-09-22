# Embedded Engineering Skills

面向 Codex/Agent 的嵌入式工程工作流与决策 Skill 源码仓库。

## 仓库定位

- 保存可重复使用的嵌入式 AI 工作流和工程决策 Skill。
- 不保存通用嵌入式源码资产或通用工具实现。
- 以本仓库中的 `skills/` 作为正式 Skill 的唯一源码。

## 仓库边界

- `Embedded_Engineering_SKILLS`：Agent Skill 及其测试、设计和仓库级辅助材料。
- `Embedded_Engineering_Library`：可跨项目复用的嵌入式源码资产。
- `Embedded_Engineering_TOOLS`：构建、烧录、调试和通信等通用工具实现。

## 目录结构

```text
.
├── skills/   # 正式 Skill 的唯一源码
├── docs/     # 仓库治理、设计说明和开发记录
├── tests/    # Skill 触发范围与行为回归材料
└── tools/    # 多个 Skill 共用的仓库级辅助工具
```

每个正式 Skill 使用 `skills/<skill-name>/` 独立目录，并以 `SKILL.md` 作为入口。仅在确有需要时添加 `scripts/`、`references/`、`assets/` 或 `agents/openai.yaml`。

## 开发流程

1. 明确任务边界并完成设计。
2. 在没有目标 Skill 的条件下运行场景，记录失败基线。
3. 创建最小 Skill，使基线场景通过。
4. 验证显式调用、隐式触发、误触发边界和核心行为。
5. 完成一个 Skill 的验收后，再开始下一个 Skill。

## 本地加载

本仓库中的 `skills/` 是唯一源码。进入触发测试阶段后，将单个 `skills/<skill-name>` 目录链接到用户级 `.agents/skills/<skill-name>`；不要把整个用户级 Skill 目录指向本仓库。

## 许可证

本仓库原创内容采用 [MIT License](LICENSE)。未来引入的第三方材料仍遵循其自身许可证，根许可证不覆盖第三方内容。
