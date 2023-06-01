#name Remove SUDO password (caution!)
#msg Do you really want to remove sudo password for this user?
#!/bin/bash

# getting the name of the current user
username=$(whoami)

# check if the file exists
if sudo [ -f /etc/sudoers.d/$username ]; then
# if the file already exists, then we output a message with the option "return all"
read -p "You have already removed the password. Do you want everything back? (d/n): " answer
if [ "$answer" == "d" ]; then
sudo rm /etc/sudoers.d/$username
notify-send "SUDO password" "File successfully deleted" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
else
exit 0
fi
else
echo "$username ALL=(ALL) NOPASSWD:ALL" | sudo tee /etc/sudoers.d/$username
notify-send "SUDO password" "File successfully created" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
