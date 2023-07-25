#name_ru_RU hdparm - измеряет производительность дисков
#msg_ru_RU hdparm - позволяет оценить пропускную способность чтения данных с жесткого диска и его кеша, что может быть полезно при анализе и тестировании производительности хранилища данных.

#name_en_US hdparm - measures disk performance
#msg_en_US hdparm - allows you to estimate the throughput of reading data from the hard disk and its cache, which can be useful in analyzing and testing the performance of data storage.

#icon 53

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

# Проверяем наличие установленного пакета hdparm
if ! $helper -Q hdparm &>/dev/null; then

# Предлагаем установить hdparm
    read -p "${pkg} hdparm ${no_found}. ${instq} hdparm? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S hdparm
        else
            $helper -S hdparm --skipreview
        fi
    else
        notify-send "${error}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

notify-send "${klaus}" "${hdparm_result}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000

# Количество повторений для усреднения
repetitions=3

# Получаем список блочных устройств с помощью lsblk и сохраняем результат в переменную devices
devices=$(lsblk -o NAME -n -l -d)

# Выбираем первое блочное устройство из списка
device=$(echo "$devices" | head -n 1)

# Проверяем, что блочное устройство определено
if [[ -n "$device" ]]; then
    # Создаем пустой массив для сохранения результатов
    results=()
    # Выполняем команду несколько раз и сохраняем результаты
    for ((i=0; i<repetitions; i++)); do
        result=$(sudo hdparm -Tt "/dev/$device" 2>/dev/null)
        results+=("$result")
    done

    # Выводим результаты каждой итерации
    for ((i=0; i<repetitions; i++)); do
        echo -e "\n${iteration} $((i+1)): ${results[$i]}"
    done

    # Вычисляем среднее значение
    total_cached=0
    total_buffered=0
    for result in "${results[@]}"; do
        cached=$(grep -oP 'cached reads:\s+\K\d+' <<< "$result")
        buffered=$(grep -oP 'buffered disk reads:\s+\K\d+' <<< "$result")
        total_cached=$((total_cached + cached))
        total_buffered=$((total_buffered + buffered))
    done
    average_cached=$((total_cached / repetitions))
    average_buffered=$((total_buffered / repetitions))

    # Выводим усредненные результаты
    echo -e "\n${average_results}"
    echo "${timing_cached} ${average_cached} ${mb_2} = $(awk "BEGIN{printf \"%.2f\", ${average_cached} / 2}") ${mbsec}"
    echo "${timing_buffered} ${average_buffered} ${mb_3} = $(awk "BEGIN{printf \"%.2f\", ${average_buffered} / 3}") ${mbsec}"

    echo -e "\n${enter_ok}"
    read
else
    notify-send "${error}" "${no_hdparm}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
fi
