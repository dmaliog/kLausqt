#name_ru_RU Список сервисов Systemd и время их загрузки (systemd-analyze)
#msg_ru_RU 'systemd-analyze blame' показывает какие сервисы занимают большую часть времени при загрузке системы и могут быть оптимизированы или отключены для более быстрой загрузки.
#name_en_US The list of Systemd services and their loading time (systemd-analyze)
#msg_en_US 'systemd-analyze blame' shows which services take up most of the time when the system boots and can be optimized or disabled for faster loading.
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

output=$(systemd-analyze blame)
systemd-analyze blame
send_to_server "$output"

read -p "${enter_ok}"
