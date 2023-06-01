#name Disk and Partition Information (fdisk)
#msg 'fdisk -l' is used to display information about all available hard drives and their partitions on the computer. It allows you to find out the sizes and types of partitions, as well as information about the file systems that are installed on them.
#!/bin/bash
sudo fdisk -l
read -p "Press Enter to continue..."
