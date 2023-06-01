#name Downloaded packages Yay
#msg /.cache/yay/: Downloaded Yay packages are stored in this folder. If you do not plan to roll back to previous versions of packages, then you can delete the old packages to free up space. However, keep in mind that if you want to delete packages, you will need to download them again if necessary.
#!/bin/bash

# Подсчет размера папки /var/cache/pacman/pkg/
folder_size=$(du -sh $HOME/.cache/yay/ | awk '{print $1}')

# Вывод размера папки
notify-send "Yay Packages" "Cleaning Size: $folder_size" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
# Очистка папки $HOME/.cache/yay/
sudo rm -rf $HOME/.cache/yay/*

notify-send "Yay packages" "Directory $HOME/.cache/yay/ cleared" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
