#name_ru_RU HardInfo - измеряет производительность системы
#msg_ru_RU HardInfo - это системный профилировщик и бенчмарк. Он способен получать информацию как от аппаратного, так и от базового программного обеспечения и организовывать ее в простом в использовании графическом интерфейсе.

#name_en_US HardInfo - measures system performance
#msg_en_US HardInfo is a system profiler and benchmark. It is able to receive information from both hardware and basic software and organize it in an easy-to-use graphical interface.

#icon 49

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

if ! yay -Q hardinfo-git &>/dev/null; then

    # Предлагаем установить hardinfo-git
    read -p "${pkg} hardinfo-git ${no_found}. ${instq} hardinfo-git? (y/n): " choice
    if [[ $choice == "y" || $choice == "Y" ]]; then
        yay -S hardinfo-git
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем hardinfo-git
bash -c "hardinfo; bash"
