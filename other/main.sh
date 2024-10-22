#!/bin/bash

# Функция определения языка
lang() {
    # Устанавливаем язык по умолчанию
    language="en_US"
    if [ -n "$1" ]; then
        language="$1"
    fi

    # Устанавливаем путь к файлу переводов
    translations_file="$language.ini" # Изменен на .ini, как вы упомянули
    script_dir=$(dirname "$0") # Получаем директорию, где находится скрипт
    translations_path="$script_dir/$translations_file" # Формируем полный путь к файлу переводов

    # Проверяем существование файла переводов
    if [[ ! -f "$translations_path" ]]; then
        echo "Файл переводов не найден: $translations_path"
        exit 1
    fi

    # Загружаем переводы
    source "$translations_path"
}

# Функция для отправки вывода на сервер
send_to_server() {
    local output="$1"
    local url=$(echo "$output" | curl -s -F 'f:1=<-' ix.io)
    if [ $? -eq 0 ] && [[ $url =~ ^https?:// ]]; then
        echo -e "\n$link $url\n"
    fi
}
