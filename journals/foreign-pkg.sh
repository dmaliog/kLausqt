#name_ru_RU Список установленных пакетов (из сторонних репозиториев)
#msg_ru_RU 'yay -Qm' показывает спискок всех пакетов, которые были установлены в систему вручную, то есть не из официальных репозиториев, а из сторонних источников, таких как AUR (Arch User Repository).
#name_en_US List of installed packages (from third-party repositories)
#msg_en_US 'yay -Qm' shows a list of all packages that were installed into the system manually, that is, not from official repositories, but from third-party sources, such as AUR (Arch User Repository).
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/.config/kLaus/other/$translations_file"
yay -Qm
read -p "${enter_ok}"
