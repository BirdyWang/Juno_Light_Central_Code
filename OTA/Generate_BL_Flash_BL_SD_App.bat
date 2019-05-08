del Juno_Light_Central.hex
del bootloader_setting.hex
del bootloader.hex
:: copy the application from the build folder to the OTA folder
copy ..\pca10040\s132\arm5_no_packs\_build\Juno_Light_Central.hex
:: generate the map setting based on the current applicaiton
nrfutil settings generate --family NRF52 --application Juno_Light_Central.hex --application-version 1 --bootloader-version 1 --bl-settings-version 1 bootloader_setting.hex
:: merge the bootloader with the map setting to generate the final bootloader file
mergehex --merge bootloader_setting.hex bootloader_pre_map.hex --output bootloader.hex
:: erase all flash
nrfjprog -f nrf52 --eraseall
:: program the softdevice
nrfjprog -f nrf52 --program s132_nrf52_6.1.1_softdevice.hex
:: verify that the softdevice is programmed correctly
nrfjprog --verify s132_nrf52_6.1.1_softdevice.hex
:: program the bootloader
nrfjprog -f nrf52 --program bootloader.hex
:: verify that the bootloader is programmed correctly
nrfjprog --verify bootloader.hex
:: program the application
nrfjprog -f nrf52 --program Juno_Light_Central.hex
:: verify that the application is programmed correctly
nrfjprog --verify Juno_Light_Central.hex
:: reset the NRF52832 chip
nrfjprog -r
:: run the newly flashed firmware
nrfjprog --run
:: pause the cmd so the user can see what happened
pause