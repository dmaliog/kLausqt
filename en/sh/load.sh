#name Install a local package
#msg Do you really want to install a local package? Attention: Install only those packages that you created with
#!/bin/bash

# Request the path to the program archive
archive_path=$(zenity --file-selection --title="Select program archive")

# Checking whether the archive was selected
if [ -n "$archive_path" ]; then
# Unpacking the archive to the yay cache directory
tar xvf "$archive_path" -C "$HOME/.cache/yay/"

# Getting the package name from the archive name
package_name=$(basename "$archive_path" .tar.gz)

yaya "$HOME/.cache/yay/$package_name"/*.pkg.tar.zst

else
notify-send "Local package installer" "No package installation archive selected" -i $HOME/kLaus/other/notify.png -a "kLaus" -t 10000
fi
