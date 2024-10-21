#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Запрос пути к архиву программы
archive_path=$(zenity --file-selection --title="${path}")

# Проверка, был ли выбран архив
if [ -n "$archive_path" ]; then
    # Распаковка архива в каталог кэша
    tar xvf "$archive_path" -C "$HOME/.cache/$helper/"

    # Получение имени пакета из имени архива
    package_name=$(basename "$archive_path" .tar.gz)

    $helper -U "$HOME/.cache/$helper/$package_name"/*.pkg.tar.zst

else
    notify-send "${local_setup}" "${no_ark}" -i "$HOME/.config/kLaus/other/notify.png" -a "kLaus" -t 10000
fi
