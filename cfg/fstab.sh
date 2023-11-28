#name_ru_RU Конфигурация монтирования файловых систем (/etc/fstab)
#msg_ru_RU <b>/etc/fstab</b> — Конфигурационный файл для монтирования файловых систем. Он содержит информацию о том, какие файловые системы автоматически монтируются при загрузке системы, а также опции монтирования, такие как параметры безопасности и права доступа.

#name_en_US File System Mount Configuration (/etc/fstab)
#msg_en_US <b>/etc/fstab</b> is a configuration file for mounting file systems. It contains information about which file systems are automatically mounted when the system boots, as well as mounting options such as security settings and access rights.

#icon 27

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

sudo nano /etc/fstab

echo -e "\n${enter_ok}"
read
