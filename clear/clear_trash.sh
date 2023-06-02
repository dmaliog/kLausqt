#name_ru_RU Корзина (принудительно)
#msg_ru_RU Вы действительно хотите удалить все файлы и папки из корзины?
#name_en_US Trash (forced)
#msg_en_US Do you really want to delete all files and folders from the trash?
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/kLaus/other/$translations_file"

TRASH_DIR=$(find ~/ -name Trash)
rm -rf "$TRASH_DIR"/*
notify-send "${trash}" "${trash_ok}" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
