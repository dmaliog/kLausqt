#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"
file_path="/var/lib/pacman/db.lck"

if [ -e "$file_path" ]; then
    sudo rm "$file_path"
else
    notify-send "${pacman}" "${pacman_nodb}" -i "$HOME/.config/kLaus/other/notify.png" -a "kLaus" -t 10000
fi
