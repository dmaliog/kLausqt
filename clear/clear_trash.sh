#name Корзина (принудительно)
#msg Вы действительно хотите удалить все файлы и папки из корзины?
#!/bin/bash
  sudo rm -Rf ~/.local/share/Trash/*
  notify-send "Корзина" "Корзина успешно очищена" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
