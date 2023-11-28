#name_ru_RU time - измеряет время выполнения команд
#msg_ru_RU <b>time</b> — Используется для измерения времени выполнения команд или программ. Она позволяет определить общее время, затраченное на выполнение команды, а также оценить использование процессорного времени и памяти.

#name_en_US time - measures the execution time of commands
#msg_en_US <b>time</b> is used to measure the execution time of commands or programs. It allows you to determine the total time spent on executing a command, as well as to evaluate the use of processor time and memory.

#icon 53

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

echo -e "${time}"

# Запускаем time
bash -c "time klauspro; bash"
