#name_ru_RU Общая информация (neofetch)
#msg_ru_RU 'neofetch' показывает информацию о версии операционной системы, используемом процессоре, типе и количестве памяти, а также другую информацию, такую как имя пользователя и его аватар, а также информацию о языке и терминальном эмуляторе.
#name_en_US General Information (neofetch)
#msg_en_US 'neofetch' shows information about the operating system version, PROCESSOR used, type and amount of memory, as well as other information such as the user's name and avatar, as well as information about the language and terminal emulator.
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/.config/kLaus/other/$translations_file"

# Проверка наличия установленного neofetch
if ! command -v neofetch &>/dev/null; then
    read -p "${neofetch}" answer
    if [[ "$answer" == [yY] ]]; then
        yay -S neofetch
    else
        notify-send "${install} neofetch" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 0
    fi
fi
neofetch
read -p "${enter_ok}"
