#name_ru_RU Детальная информация о железе (inxi)
#msg_ru_RU 'inxi -Fxxc0z' показывает детальную информацию о железе (CPU/RAM/GPU - их частоте и количестве ядер), дисках и т.д.

#name_en_US Detailed information about hardware (inxi)
#msg_en_US 'ini -Fxxc0z' shows detailed information about hardware (CPU/RAM/GPU - their frequency and number of cores), disks, etc.

#icon 31

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

# Проверка наличия установленного inxi
if ! yay -Q inxi &>/dev/null; then
    read -p "${pkg} inxi ${no_found}. ${instq} inxi? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        yay -S inxi
    else
        notify-send "${install} inxi" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 0
    fi
fi

output=$(inxi -Fxxc0z)
inxi -Fxxc0z
send_to_server "$output"

echo -e "\n${enter_ok}"
read
