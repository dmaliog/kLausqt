#name Information about disks and partitions (lsblk)
#msg 'lsblk -f -o+SIZE' shows information about block devices such as hard drives, flash drives, CD/DVD drives, etc. It allows you to find out the list of devices, their file systems and sizes.
#!/bin/bash
lsblk -f -o+SIZE
read -p "Press Enter to continue..."
