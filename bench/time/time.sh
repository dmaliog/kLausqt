#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

echo -e "${time}"

# Запускаем time
bash -c "time klauspro; bash"
