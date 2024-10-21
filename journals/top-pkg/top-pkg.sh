#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"
helper="$2"

output=$($helper -Ps)
$helper -Ps
send_to_server "$output"

echo -e "\n${enter_ok}"
read
