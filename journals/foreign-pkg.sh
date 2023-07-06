#name_ru_RU Список установленных пакетов (из сторонних репозиториев)
#msg_ru_RU 'yay -Qm' показывает спискок всех пакетов, которые были установлены в систему вручную, то есть не из официальных репозиториев, а из сторонних источников, таких как AUR (Arch User Repository).
#name_en_US List of installed packages (from third-party repositories)
#msg_en_US 'yay -Qm' shows a list of all packages that were installed into the system manually, that is, not from official repositories, but from third-party sources, such as AUR (Arch User Repository).
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

output=$(yay -Qm)
yay -Qm
send_to_server "$output"

read -p "${enter_ok}"
