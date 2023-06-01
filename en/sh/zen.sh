#name Install Zen Core
#msg Do you really want to install Zen core?
#!/bin/bash

# Checking for Zen packages installed
if [ -n "$(yay -Qs linux-zen)" ] || [ -n "$(yay -Qs linux-zen-headers)" ]; then
echo "You already have Zen core installed!"
echo "Want to delete it and update GRUB? (d/n)"
read answer
if [ "$answer" == "d" ]; then
sudo yay -R --noconfirm linux-zen linux-zen-headers

# Checking for Grub
if [ ! -f "/boot/grub/grub.cfg" ]; then
notify-send "ZEN installation" "Grub not detected..." -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
else
# Grub update
sudo grub-mkconfig -o /boot/grub/grub.cfg
notify-send "ZEN installation" "Removal completed successfully" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
fi
else
# Installing the Linux Zen kernel and
yay -S headers --noconfirm linux-zen linux-zen-headers

# Checking for Grub
if [ ! -f "/boot/grub/grub.cfg" ]; then
notify-send "ZEN installation" "Grub not detected" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
else
# Grub Update
sudo grub-mkconfig -o /boot/grub/grub.cfg
notify-send "ZEN installation" "Installation completed successfully" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
fi
