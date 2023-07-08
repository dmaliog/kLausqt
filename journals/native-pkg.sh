#name_ru_RU Список установленных пакетов (из официальных репозиториев)
#msg_ru_RU 'yay -Qn' показывает список установленных пакетов, которые не являются зависимостями других пакетов (т.е. которые были установлены явно, а не в результате установки других пакетов).
#name_en_US List of installed packages (from official repositories)
#msg_en_US 'yay -Qn' shows a list of installed packages that are not dependencies of other packages (i.e. that were installed explicitly, and not as a result of installing other packages).
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

output=$(yay -Qn)
yay -Qn
send_to_server "$output"

echo -e "\n${enter_ok}"
read
