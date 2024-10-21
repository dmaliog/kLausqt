#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

if ! $helper -Q phoronix-test-suite &>/dev/null; then

    # Предлагаем установить phoronix-test-suite
    read -p "${pkg} phoronix-test-suite ${no_found}. ${instq} phoronix-test-suite? (y/n): " answer
    if [[ "$answer" == [yY] ]]; then
        if [ "$helper" = "yay" ]; then
            $helper -S phoronix-test-suite
        else
            $helper -S phoronix-test-suite --skipreview
        fi
    else
        notify-send "${install}" "${noinstall}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
        exit 1
    fi
fi

# Запускаем phoronix-test-suite
bash -c "phoronix-test-suite; bash"
