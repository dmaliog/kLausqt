#name_ru_RU Список установленных пакетов (из сторонних репозиториев)
#msg_ru_RU <b>helper -Qm</b> — Отображает спискок всех пакетов, которые были установлены в систему вручную, то есть не из официальных репозиториев, а из сторонних источников, таких как AUR (Arch User Repository).

#name_en_US List of installed packages (from third-party repositories)
#msg_en_US <b>helper -Qm</b> shows a list of all packages that were installed into the system manually, that is, not from official repositories, but from third-party sources, such as AUR (Arch User Repository).

#icon applications-all

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

output=$($helper -Qm)
$helper -Qm
send_to_server "$output"

echo -e "\n${enter_ok}"
read
