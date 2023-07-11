#name_ru_RU Общая информация (neofetch)
#msg_ru_RU 'neofetch' показывает информацию о версии операционной системы, используемом процессоре, типе и количестве памяти, а также другую информацию, такую как имя пользователя и его аватар, а также информацию о языке и терминальном эмуляторе.

#name_en_US General Information (neofetch)
#msg_en_US 'neofetch' shows information about the operating system version, PROCESSOR used, type and amount of memory, as well as other information such as the user's name and avatar, as well as information about the language and terminal emulator.

#icon 31

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

# Проверка наличия установленного neofetch
if ! yay -Q neofetch &>/dev/null; then
    read -p "${pkg} neofetch ${no_found}. ${instq} neofetch? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        yay -S neofetch
    else
        notify-send "${install} neofetch" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 0
    fi
fi

neofetch

echo -e "\n${enter_ok}"
read
