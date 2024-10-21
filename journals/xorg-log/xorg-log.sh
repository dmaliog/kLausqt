#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

nano /var/log/Xorg.0.log

echo -e "\n${enter_ok}"
read
