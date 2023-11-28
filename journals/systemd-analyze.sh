#name_ru_RU Список сервисов Systemd и время их загрузки (systemd-analyze)
#msg_ru_RU <b>systemd-analyze blame</b> — Отображает сервисы которые занимают большую часть времени при загрузке системы и могут быть оптимизированы или отключены для более быстрой загрузки.

#name_en_US The list of Systemd services and their loading time (systemd-analyze)
#msg_en_US <b>systemd-analyze blame</b> shows which services take up most of the time when the system boots and can be optimized or disabled for faster loading.

#icon 31

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

output=$(systemd-analyze blame)
systemd-analyze blame
send_to_server "$output"

echo -e "\n${enter_ok}"
read
