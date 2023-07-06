#name_ru_RU Корзина (принудительно)
#msg_ru_RU Вы действительно хотите удалить все файлы и папки из корзины?
#name_en_US Trash (forced)
#msg_en_US Do you really want to delete all files and folders from the trash?
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

TRASH_DIR=$(find ~/ -name Trash)
rm -rf "$TRASH_DIR"/*
notify-send "${trash}" "${trash_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
