#name_ru_RU Статистика установленных пакетов (yay)
#msg_ru_RU 'yay -Ps' показывает статистику использования пакетов на вашей системе, включая число установленных, обновленных и удаленных пакетов, а также сколько времени прошло с последнего обновления системы. Она также показывает список установленных пакетов, которые не являются зависимостями других пакетов.
#name_en_US Statistics of installed packages (yay)
#msg_en_US 'yay -Ps' shows statistics of package usage on your system, including the number of installed, updated and deleted packages, as well as how much time has passed since the last system update. It also shows a list of installed packages that are not dependencies of other packages.
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/kLaus/other/$translations_file"
yay -Ps
read -p "${enter_ok}"
