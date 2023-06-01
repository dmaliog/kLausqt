#name Detailed information about hardware (inxi)
#msg 'inxi -Fxxc0z' shows detailed information about hardware (CPU/RAM/GPU - their frequency and number of cores), disks, etc.
#!/bin/bash
inxi -Fxxc0z
read -p "Press Enter to continue..."
