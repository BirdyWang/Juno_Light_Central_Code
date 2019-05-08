To correctly compile this project:

1. nRF5_SDK_12.3.0_d7731ad needs to be in C:\Keil_v5 folder.

2. The project needs to be placed in C:\Keil_v5\Projects folder.

To upload the hex software:

The hex software is located at ble_app_uart/build/IO_Board_Software.hex

Open nRFgo Studio, in Device Manager, choose nRF5x Programming.

A LOT of the times, the nRFgo Studio will not show the programming interface correctly. 

The only solution so far is to continuously switch between "Motherboards" and "nRF5x Programming" tabs until the correct interface shows up.

![Alt text](https://bitbucket.org/psyonicinc/i-o-board-software/src/master/nRFgo_Studio.jpg "nRF Programming Tab")

Go to "Program Application", choose the IO_Board_Software.hex and click Program. 
