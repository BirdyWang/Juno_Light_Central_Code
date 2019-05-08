del Juno_Light_Central.hex
del juno_light_central_dfu_package_v2.zip
:: copy the application from the build folder to the OTA folder
copy ..\pca10040\s132\arm5_no_packs\_build\Juno_Light_Central.hex
:: generate the new DFU package to be uploaded
nrfutil pkg generate --hw-version 52 --application-version 2 --application Juno_Light_Central.hex --sd-req 0xb7 --key-file private.key juno_light_central_dfu_package_v2.zip
:: pause the cmd so the user can see what happened
pause