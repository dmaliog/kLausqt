#name_ru_RU Убрать пароль SUDO (осторожно!)
#msg_ru_RU <b>Sudo</b> — Утилита, предоставляющая привилегии root для выполнения административных операций в соответствии со своими настройками. Она позволяет легко контролировать доступ к важным приложениям в системе. По умолчанию, при установке Ubuntu первому пользователю (тому, который создаётся во время установки) предоставляются полные права на использование sudo.<br><br>Скрипт убирает запрос пароля sudo для этого пользователя.

#name_en_US Remove SUDO password (caution!)
#msg_en_US <b>Sudo</b> is a utility that grants root privileges to perform administrative operations according to its settings. It allows you to easily control access to important applications in the system. By default, when installing Ubuntu, the first user (the one that is created during installation) is granted full rights to use sudo.<br><br>The script removes the sudo password request for this user.

#icon 51

#!/bin/bash

# Импорт файла main.sh
source "$HOME/.config/kLaus/other/main.sh"
lang "$1"


# получаем имя текущего пользователя
username=$(whoami)

# проверяем, существует ли файл
if sudo [ -f /etc/sudoers.d/$username ]; then
  # если файл уже существует, то выводим сообщение с опцией "вернуть все"
  read -p "${sudo_ok}" answer
  if [[ "$answer" == [yY] ]]; then
    sudo rm /etc/sudoers.d/$username
    notify-send "${sudo}" "${removefile_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
  else
    exit 0
  fi
else
    echo "$username ALL=(ALL) NOPASSWD:ALL" | sudo tee /etc/sudoers.d/$username
    notify-send "${sudo}" "${createfile_ok}" -i $HOME/.config/kLaus/other/notify.png -a "kLaus" -t 10000
fi
