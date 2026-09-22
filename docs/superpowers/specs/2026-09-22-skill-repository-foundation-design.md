# 嵌入式 Skill 仓库基础设施设计

## 目标

初始化 `Embedded_Engineering_SKILLS` 的仓库治理和基础目录，为后续逐个开发、测试和发布嵌入式工程 Skill 提供稳定边界。

本阶段仅建设仓库框架，不创建 `embedded-design-decision` 或其他具体 Skill。

## 仓库职责

本仓库保存：

- 面向 Codex/Agent 的嵌入式 AI 工作流 Skill。
- 嵌入式工程分析、设计和决策类 Skill。
- Skill 的测试场景、设计文档及必要的仓库级辅助工具。

本仓库不保存：

- 通用嵌入式源码资产；此类内容归入 `Embedded_Engineering_Library`。
- 通用构建、烧录、调试、通信工具实现；此类内容归入 `Embedded_Engineering_TOOLS`。
- 公司内部代码、密钥、个人隐私或其他保密材料。

## 目录结构

```text
Embedded_Engineering_SKILLS/
├── README.md
├── AGENTS.md
├── LICENSE
├── CHANGELOG.md
├── skills/
├── docs/
├── tests/
└── tools/
```

空目录使用简短的 `.gitkeep` 保留；出现真实内容后删除对应 `.gitkeep`。

### 目录职责

- `skills/`：正式 Skill 的唯一源码。每个 Skill 使用独立目录，入口为 `SKILL.md`。
- `docs/`：仓库治理、设计说明和开发记录，不代替具体 Skill 的运行时说明。
- `tests/`：Skill 触发范围、行为回归和必要的验证材料。
- `tools/`：仅保存多个 Skill 或仓库维护共同需要的辅助工具；单个 Skill 专用脚本放入该 Skill 的 `scripts/`。

## Skill 组织规则

- 一个 Skill 对应一个明确、可重复的工程任务或决策过程。
- 不将单个项目经验直接提升为全局规则。
- Skill 默认服从目标项目已有架构、规范和可验证事实。
- `SKILL.md` 保持精简；大量条件性资料放入 `references/`。
- 仅在确有需要时创建 `scripts/`、`references/`、`assets/` 和 `agents/openai.yaml`。
- 禁止在 Skill 中硬编码本机绝对路径。
- 修改 Skill 后必须验证触发范围和行为回归。
- 每次只开发和验证一个 Skill，完成后再进入下一个。

## 本地加载模型

仓库中的 `skills/` 是唯一源码。正式进入某个 Skill 的触发测试阶段后，为该 Skill 单独创建用户级目录联接：

```text
~/.agents/skills/<skill-name>
    -> <repository>/skills/<skill-name>
```

不把整个用户级 `.agents/skills` 目录指向本仓库，避免覆盖或干扰其他 Skill。

## 根文件职责

- `README.md`：说明仓库定位、目录、开发流程和与另外两个仓库的边界。
- `AGENTS.md`：约束在本仓库工作的 Agent，包括改动范围、Skill 结构、测试和安全要求。
- `LICENSE`：覆盖本仓库原创内容；第三方材料仍服从其自身许可证。
- `CHANGELOG.md`：记录正式发布或仓库级重要变化，不记录日常开发流水账。

## 初始化验收标准

- 根文件齐全，内容不存在职责冲突。
- 四个基础目录均可被 Git 跟踪。
- 未提前创建任何具体 Skill。
- 文档明确区分 SKILLS、Library 和 TOOLS 三个仓库。
- 文档未包含本机绑定路径、密钥或保密内容。
- Git 差异仅包含 S00 仓库基础设施。

## 后续阶段

S00 验收完成后，再启动 S01：设计并开发首个 `embedded-design-decision` Skill。S01 需要独立完成基线场景、Skill 设计、实现和行为验证，不属于本次初始化范围。
