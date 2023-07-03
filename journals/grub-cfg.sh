#name_ru_RU Конфигурация загрузчика GRUB2 (/boot/grub/grub.cfg)
#msg_ru_RU '/boot/grub/grub.cfg' - это конфигурационный файл загрузчика GRUB2, который используется в большинстве современных дистрибутивов Linux. Он содержит информацию о различных операционных системах и ядрах, установленных на компьютере, а также параметры, необходимые для их загрузки.
#name_en_US GRUB2 Bootloader Configuration (/boot/grub/grub.cfg)
#msg_en_US '/boot/grub/grub.cfg' is a GRUB2 bootloader configuration file that is used in most modern Linux distributions. It contains information about the various operating systems and kernels installed on the computer, as well as the parameters necessary to boot them.
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/.config/kLaus/other/$translations_file"
sudo nano /boot/grub/grub.cfg
read -p "${enter_ok}"
