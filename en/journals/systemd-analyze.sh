#name The list of Systemd services and their loading time (systemd-analyze)
#msg 'systemd-analyze blame' shows which services take up most of the time when the system boots and can be optimized or disabled for faster loading.
#!/bin/bash
systemd-analyze blame
read -p "Press Enter to continue..."
