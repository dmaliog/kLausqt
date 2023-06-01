#name Hardware Summary (hwinfo)
#msg 'hwinfo --short' shows brief information about hardware (processor, motherboard, graphics card, hard disk, sound card, etc.). Contains information about each detected device, including its model, manufacturer, type, condition, and drivers used.
#!/bin/bash
hwinfo --short
read -p "Press Enter to continue..."

#!/bin/bash
sudo nano /boot/grub/grub.cfg
read -p "Press Enter to continue..."
