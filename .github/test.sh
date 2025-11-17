#!/bin/bash

NL=$'\n'
Code="$NL\`\`\`$NL"

Comment() {
    COMMENTS_URL=$(cat $GITHUB_EVENT_PATH | jq -r .pull_request.comments_url)
    if [ ${#1} -ne 0 ]; then
        PAYLOAD=$(echo '{}' | jq --arg body "$1" '.body = $body' 2>/dev/null)
        curl -s -S -H "Authorization: token $GITHUB_TOKEN" \
             --header "Content-Type: application/vnd.github.VERSION.text+json" \
             --data "${PAYLOAD}" "$COMMENTS_URL" &>/dev/null
    fi
}

if [[ -z "$GITHUB_TOKEN" ]]; then
    echo "The GITHUB_TOKEN is required."
    exit 1
fi

sudo apt-get update >/dev/null
sudo apt-get -qq -y install curl clang clang-tidy jq libgtest-dev >/dev/null

URL="https://api.github.com/repos/${GITHUB_REPOSITORY}/pulls/$PR_ID/files"
changeFile=$(curl -s -X GET -H "Authorization: Bearer $GITHUB_TOKEN" $URL | jq -r '.[].filename')

clang++ "$changeFile" strbuf/test.cpp -lgtest -lgtest_main -lpthread -Wall -std=c++17 2>compiler.log
backUp=$?
compiler_output=$(cat compiler.log)

if [ $backUp == 0 ]; then
    timeout 20s ./a.out
    backUp=$?
else
    Comment "编译失败$NL$NL编译输出$Code$compiler_output$Code"
    exit -1
fi

clang++ "$changeFile" strbuf/test.cpp -lgtest -lgtest_main -lpthread \
    -fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope \
    -g -D __SA_TEST__ -std=c++17 -o a.out
timeout 20s ./a.out > Runtime.log 2>&1
AddressSanitize_output=$(cat Runtime.log)

OUTPUT="编译成功$NL编译输出$Code$compiler_output$Code测试输出$Code$AddressSanitize_output$Code"
Comment "$OUTPUT"

if echo "$AddressSanitize_output" | grep -q "FAILED"; then
    exit -1
fi
if ! echo "$AddressSanitize_output" | grep -q "PASSED"; then
    exit -1
fi

