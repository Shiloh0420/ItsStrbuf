# Strbuf 验收目录

请将 Strbuf 的源代码提交到本目录，并按以下方式组织文件：

## 提交方式

### 1. 单文件提交
如果你的实现只有一个 `.c` 文件，请将其重命名为：

- **大驼峰命名法的姓名拼音全拼**（例如：`ZhangSan.c`）

并放在本目录下。

### 2. 多文件提交
如果你的实现包含多个源文件，请在本目录中创建一个文件夹：

- 文件夹名称同样使用 **大驼峰命名法的姓名拼音全拼**（例如：`ZhangSan/`）

然后将所有相关源文件放入该文件夹。

### 目录示例

```
strbuf
├── LICENSE
├── README.md
├── submissions
│   ├── README.md
│   ├── ZhangSan.c
│   └── LiSi
│       ├── strbuf.c
│       └── utils.c
└── StrbufTest
```

如果 `submissions` 目录不存在，请自行创建。

---

# 提交流程

1. fork 本仓库。
2. 在你 fork 的仓库中进行修改，将文件放到 `submissions` 目录下。
3. 建议新建一个分支，例如：

   ```bash
   git checkout -b submission/ZhangSan
   ```

4. 提交修改：

   ```bash
   git add submissions/
   git commit -m "submission: ZhangSan - Strbuf implementation"
   git push origin submission/ZhangSan
   ```

5. 在 GitHub 上发起 Pull Request。
    1) 打开你 fork 的仓库页面
    2) 点击页面上方的 "Pull requests" 标签
    3) 点击右侧的 "New pull request" 按钮
    4) 在 "base repository" 中选择本仓库，"base branch" 选择 strbuf_25（或指定的目标分支）
    5) 在 "compare" 中选择你刚刚 push 的分支
    6) 检查文件列表，确保提交的文件或文件夹正确
    7) 填写 PR 标题，例如：
        submission: ZhangSan - Strbuf
    9) 点击 "Create pull request" 提交

---

# 验收流程

- 提交 PR 后，GitHub 会自动运行 CI 进行测试。
- CI 测试结果将作为验收依据之一。
- 代码由大二成员进行审核，审核通过后会合并到指定分支（如 `strbuf_25`）。

---

如需补充说明（例如本地编译步骤），可以在 PR 描述里写清楚。
