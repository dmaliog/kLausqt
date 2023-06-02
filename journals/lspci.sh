#name_ru_RU Информация о подключенных PCI устройствах (lspci)
#msg_ru_RU 'lspci -vnn' показывает информацию о устройствах, подключенных к шине PCI. О производителе, модели и ревизии устройства, а также о том, какой драйвер используется для этого устройства.
#name_en_US Information about Connected PCI Devices (lspci)
#msg_en_US 'lspci -vnn' shows information about devices connected to the PCI bus. About the manufacturer, model and revision of the device, as well as which driver is used for this device.
#!/bin/bash
# Определение языка
language="en_US"
if [ -n "$1" ]; then
    language="$1"
fi

# Загрузка файла перевода
translations_file="translations_$language.txt"
source "$HOME/kLaus/other/$translations_file"
lspci -vnn
read -p "${enter_ok}"
