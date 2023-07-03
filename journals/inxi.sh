#name_ru_RU Детальная информация о железе (inxi)
#msg_ru_RU 'inxi -Fxxc0z' показывает детальную информацию о железе (CPU/RAM/GPU - их частоте и количестве ядер), дисках и т.д.
#name_en_US Detailed information about hardware (inxi)
#msg_en_US 'ini -Fxxc0z' shows detailed information about hardware (CPU/RAM/GPU - their frequency and number of cores), disks, etc.
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/.config/kLaus/other/$translations_file"

# Проверка наличия установленного inxi
if ! command -v inxi &>/dev/null; then
    read -p "${inxi}" answer
    if [[ "$answer" == [yY] ]]; then
        yay -S inxi
    else
        notify-send "${install} inxi" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 0
    fi
fi

inxi -Fxxc0z
read -p "${enter_ok}"
