#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

# Находим директорию Trash, игнорируя ошибки
TRASH_DIR=$(find ~/ -name Trash 2>/dev/null)

# Удаляем содержимое корзины, если директория найдена
if [ -d "$TRASH_DIR" ]; then
    rm -rf "$TRASH_DIR"/*
    notify-send "${trash}" "${trash_ok}" -i "$HOME/.config/kLaus/other/notify.png" -a "kLaus" -t 10000
else
    notify-send "${trash}" "${trash_nofound}" -i "$HOME/.config/kLaus/other/notify.png" -a "kLaus" -t 10000
fi
