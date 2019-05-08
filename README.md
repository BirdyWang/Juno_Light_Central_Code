This project contains the embedded drivers for Juno_Light circuit assembly, the software is developed in Kevil_v5, files associated with OTA(Over The Air) Update are generated with Windows batch files. 

Project setup:

1. Create a new folder in C:\Keil_v5 directory named "NRF5_SDK". Download the SDK from git repository: . Copy the content of the SDK into NRF_SDK folder. 

2. Create a new folder in C:\Keil_v5 directory named "Project". In the Project folder, create a new folder named "Juno_Light_Central". Place the git content in the Folder. 

To compile the project in Keil_v5:

1. Make sure Keil_v5 is installed. 

2. Navigate to C:\Keil_v5\Project\Juno_Light_Central\pca10040\s132\arm5_no_packs. Double click Juno_Light_Central.uvprojx

3. Keil might warn you about installing or updating packages in the package installer pop up. Install all of those.

4. Go to Project -> Rebuild all target files. 

To Program the software onto the central microcontroller: 

1. Attach JLink to the Programming Ports on the central PCB. 

2. Go to OTA folder and double click on Generate_BL_Flash_BL_SD_App.bat. This step only needs to be done once if furture updates are done through OTA.

3. For OTA update with the app: double click Generate_New_OTA_Package.bat and put the generated zip file in firebase. 
