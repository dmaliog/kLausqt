#!/bin/bash

# Функция определения языка
lang() {
    language="en_US"
    if [ -n "$1" ]; then
        language="$1"
    fi

    translations_file="translations_$language.txt"
    source "$HOME/.config/kLaus/other/$translations_file"
}

# Функция для отправки вывода на сервер
send_to_server() {
    local output="$1"
    local url=$(echo "$output" | curl -s -F 'f:1=<-' ix.io)
    if [ $? -eq 0 ] && [[ $url =~ ^https?:// ]]; then
        echo -e "\n$link $url\n"
    fi
}
