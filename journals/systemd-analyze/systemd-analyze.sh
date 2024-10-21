#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

output=$(systemd-analyze blame)
systemd-analyze blame
send_to_server "$output"

echo -e "\n${enter_ok}"
read
