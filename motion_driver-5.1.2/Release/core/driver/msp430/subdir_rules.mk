################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
core/driver/msp430/msp430_clock.obj: ../core/driver/msp430/msp430_clock.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.1/bin/cl430" -vmspx --abi=coffabi --code_model=large --data_model=restricted --near_data=globals -O2 --opt_for_speed=0 --symdebug:none --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.1/include" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/msp430/USB_eMPL" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/eMPL" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/msp430" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/msp430/F5xx_F6xx_Core_Lib" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/msp430/USB_API" --include_path="C:/CC_workspace/motion_driver-5.1.2/simple_apps/msp430" --gcc --define=__MSP430F5528__ --define=USE_DMP --define=CONFIG_INTERFACE_USB --define=MPU6050 --define=I2C_B0 --define=EMPL --define=MOTION_DRIVER_TARGET_MSP430 --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="core/driver/msp430/msp430_clock.pp" --obj_directory="core/driver/msp430" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

core/driver/msp430/msp430_i2c.obj: ../core/driver/msp430/msp430_i2c.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.1/bin/cl430" -vmspx --abi=coffabi --code_model=large --data_model=restricted --near_data=globals -O2 --opt_for_speed=0 --symdebug:none --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.1/include" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/msp430/USB_eMPL" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/eMPL" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/msp430" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/msp430/F5xx_F6xx_Core_Lib" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/msp430/USB_API" --include_path="C:/CC_workspace/motion_driver-5.1.2/simple_apps/msp430" --gcc --define=__MSP430F5528__ --define=USE_DMP --define=CONFIG_INTERFACE_USB --define=MPU6050 --define=I2C_B0 --define=EMPL --define=MOTION_DRIVER_TARGET_MSP430 --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="core/driver/msp430/msp430_i2c.pp" --obj_directory="core/driver/msp430" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

core/driver/msp430/msp430_interrupt.obj: ../core/driver/msp430/msp430_interrupt.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.1/bin/cl430" -vmspx --abi=coffabi --code_model=large --data_model=restricted --near_data=globals -O2 --opt_for_speed=0 --symdebug:none --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.1/include" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/msp430/USB_eMPL" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/eMPL" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/msp430" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/msp430/F5xx_F6xx_Core_Lib" --include_path="C:/CC_workspace/motion_driver-5.1.2/core/driver/msp430/USB_API" --include_path="C:/CC_workspace/motion_driver-5.1.2/simple_apps/msp430" --gcc --define=__MSP430F5528__ --define=USE_DMP --define=CONFIG_INTERFACE_USB --define=MPU6050 --define=I2C_B0 --define=EMPL --define=MOTION_DRIVER_TARGET_MSP430 --diag_warning=225 --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="core/driver/msp430/msp430_interrupt.pp" --obj_directory="core/driver/msp430" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


