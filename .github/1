#!/bin/bash

NL=$'\n'
Code="$NL\`\`\`$NL"

# PR 评论
Comment() {
    COMMENTS_URL=$(jq -r .pull_request.comments_url <"$GITHUB_EVENT_PATH")
    if [[ -n "$1" ]]; then
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
sudo apt-get -qq -y install curl clang clang-tidy jq libgtest-dev >/dev/null

# 获取PR修改的文件
URL="https://api.github.com/repos/${GITHUB_REPOSITORY}/pulls/$PR_ID/files"
changeFile=$(curl -s -H "Authorization: Bearer $GITHUB_TOKEN" $URL | jq -r '.[].filename')

# 编译普通测试
clang++ "$changeFile" .github/test.cpp -lgtest -lgtest_main -lpthread -Wall -std=c++17 2>compiler.log
backUp=$?
compiler_output=$(cat compiler.log)

if [[ $backUp -ne 0 ]]; then
    Comment "编译失败$NL$NL编译输出$Code$compiler_output$Code"
    exit 1
fi

# 运行普通测试
timeout 20s ./a.out

# 编译 AddressSanitize 测试
clang++ "$changeFile" .github/test.cpp -lgtest -lgtest_main -lpthread \
    -fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope \
    -g -D __SA_TEST__ -std=c++17 -o a.out

# 运行 AddressSanitize 测试
timeout 20s ./a.out > Runtime.log 2>&1
AddressSanitize_output=$(cat Runtime.log)

# 提交 PR 评论
OUTPUT="编译成功$NL编译输出$Code$compiler_output$Code测试输出$Code$AddressSanitize_output$Code"
Comment "$OUTPUT"

# 判断测试结果
if echo "$AddressSanitize_output" | grep -q "FAILED"; then
    exit 1
fi
if ! echo "$AddressSanitize_output" | grep -q "PASSED"; then
    exit 1
fi

OUTPUT_="你通过了 Strbuf 的所有测试!"
    Comment "$OUTPUT_"