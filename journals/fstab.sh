#name_ru_RU Конфигурация монтирования файловых систем (/etc/fstab)
#msg_ru_RU '/etc/fstab' - это конфигурационный файл для монтирования файловых систем. Он содержит информацию о том, какие файловые системы автоматически монтируются при загрузке системы, а также опции монтирования, такие как параметры безопасности и права доступа.
#name_en_US File System Mount Configuration (/etc/fstab)
#msg_en_US '/etc/fstab' is a configuration file for mounting file systems. It contains information about which file systems are automatically mounted when the system boots, as well as mounting options such as security settings and access rights.
#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

sudo nano /etc/fstab

read -p "${enter_ok}"
