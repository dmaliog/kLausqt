#name_ru_RU Список установленных пакетов (из официальных репозиториев)
#msg_ru_RU 'yay -Qn' показывает список установленных пакетов, которые не являются зависимостями других пакетов (т.е. которые были установлены явно, а не в результате установки других пакетов).
#name_en_US List of installed packages (from official repositories)
#msg_en_US 'yay -Qn' shows a list of installed packages that are not dependencies of other packages (i.e. that were installed explicitly, and not as a result of installing other packages).
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/kLaus/other/$translations_file"
yay -Qn
read -p "${enter_ok}"
