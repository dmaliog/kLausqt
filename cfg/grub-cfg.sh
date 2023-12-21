#name_ru_RU Конфигурация загрузчика GRUB2 (/boot/grub/grub.cfg)
#msg_ru_RU <b>/boot/grub/grub.cfg</b> — Конфигурационный файл загрузчика GRUB2, который используется в большинстве современных дистрибутивов Linux. Он содержит информацию о различных операционных системах и ядрах, установленных на компьютере, а также параметры, необходимые для их загрузки.

#name_en_US GRUB2 Bootloader Configuration (/boot/grub/grub.cfg)
#msg_en_US <b>/boot/grub/grub.cfg</b> is a GRUB2 bootloader configuration file that is used in most modern Linux distributions. It contains information about the various operating systems and kernels installed on the computer, as well as the parameters necessary to boot them.

#icon gnome-do

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"

sudo nano /boot/grub/grub.cfg

echo -e "\n${enter_ok}"
read
