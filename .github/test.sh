#!/bin/bash

NL=$'\n'
Code="$NL\`\`\`$NL"

# PR 评论函数
Comment() {
    if [[ -n "$GITHUB_TOKEN" && -n "$1" ]]; then
        COMMENTS_URL=$(jq -r .pull_request.comments_url <"$GITHUB_EVENT_PATH")
        PAYLOAD=$(jq -n --arg body "$1" '{body:$body}')
        curl -s -S -H "Authorization: token $GITHUB_TOKEN" \
             -H "Content-Type: application/vnd.github.VERSION.text+json" \
             --data "$PAYLOAD" "$COMMENTS_URL" &>/dev/null
    fi
}

# 检查 GITHUB_TOKEN
if [[ -z "$GITHUB_TOKEN" ]]; then
    echo "The GITHUB_TOKEN is required."
    exit 1
fi

# 安装依赖
sudo apt-get update >/dev/null
sudo apt-get -qq -y install curl clang clang-tidy jq libgtest-dev g++ >/dev/null

# 获取PR修改的文件
URL="https://api.github.com/repos/${GITHUB_REPOSITORY}/pulls/$PR_ID/files"
changeFiles=$(curl -s -H "Authorization: Bearer $GITHUB_TOKEN" $URL | jq -r '.[].filename')

# 将所有 .c 和 .cpp 文件组合成参数列表
files_to_compile=""
for f in $changeFiles; do
    if [[ -f "$f" ]]; then
        files_to_compile="$files_to_compile $f"
    fi
done
# 始终加上 test.cpp
files_to_compile="$files_to_compile .github/test.cpp"

# 编译普通测试
clang++ $files_to_compile -lgtest -lgtest_main -lpthread -Wall -std=c++17 -o a.out 2> compiler.log
compiler_output=$(cat compiler.log)
if [[ $? -ne 0 ]]; then
    Comment "编译失败$NL$NL编译输出$Code$compiler_output$Code"
    exit 1
fi

# 运行普通测试
timeout 20s ./a.out || code=$?
if [[ $code -eq 139 ]]; then
    Comment "程序崩溃（Segmentation fault）$NL$NL$Code$(cat Runtime.log)$Code"
    exit 1
fi

# 编译 AddressSanitize 测试
clang++ $files_to_compile -lgtest -lgtest_main -lpthread \
    -fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope \
    -g -D __SA_TEST__ -std=c++17 -o a.out 2> compiler.log
compiler_output=$(cat compiler.log)
if [[ $? -ne 0 ]]; then
    Comment "ASan 编译失败$NL$NL编译输出$Code$compiler_output$Code"
    exit 1
fi

# 运行 AddressSanitize 测试
timeout 20s ./a.out > Runtime.log 2>&1 || code=$?
AddressSanitize_output=$(cat Runtime.log)

# 提交 PR 评论
Comment "编译成功$NL编译输出$Code$comp
