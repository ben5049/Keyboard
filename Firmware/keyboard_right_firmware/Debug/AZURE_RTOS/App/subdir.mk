################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AZURE_RTOS/App/app_azure_rtos.c 

OBJS += \
./AZURE_RTOS/App/app_azure_rtos.o 

C_DEPS += \
./AZURE_RTOS/App/app_azure_rtos.d 


# Each subdirectory must supply rules for building sources it contributes
AZURE_RTOS/App/%.o AZURE_RTOS/App/%.su AZURE_RTOS/App/%.cyclo: ../AZURE_RTOS/App/%.c AZURE_RTOS/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32G0C1xx -DUSBPD_PORT_COUNT=2 -D_DRP -DUSBPDCORE_LIB_PD3_FULL -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../Core/Inc -I../AZURE_RTOS/App -I../USBX/App -I../USBX/Target -I../USBPD/App -I../USBPD/Target -I../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../Middlewares/ST/usbx/common/core/inc/ -I../Middlewares/ST/usbx/ports/generic/inc/ -I../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m0/gnu/inc/ -IC:/Users/bens1/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.2/Drivers/STM32G0xx_HAL_Driver/Inc -IC:/Users/bens1/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.2/Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -IC:/Users/bens1/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.2/Middlewares/ST/STM32_USBPD_Library/Core/inc -IC:/Users/bens1/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.2/Middlewares/ST/STM32_USBPD_Library/Devices/STM32G0XX/inc -IC:/Users/bens1/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.2/Drivers/CMSIS/Device/ST/STM32G0xx/Include -IC:/Users/bens1/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.2/Drivers/CMSIS/Include -IC:/Users/bens1/STM32Cube/Repository//Packs/STMicroelectronics/X-CUBE-AZRTOS-G0/1.1.0/Middlewares/ST/usbx/common/usbx_device_classes/inc/ -IC:/Users/bens1/STM32Cube/Repository//Packs/STMicroelectronics/X-CUBE-AZRTOS-G0/1.1.0/Middlewares/ST/usbx/common/core/inc/ -IC:/Users/bens1/STM32Cube/Repository//Packs/STMicroelectronics/X-CUBE-AZRTOS-G0/1.1.0/Middlewares/ST/usbx/ports/generic/inc/ -IC:/Users/bens1/STM32Cube/Repository//Packs/STMicroelectronics/X-CUBE-AZRTOS-G0/1.1.0/Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -IC:/Users/bens1/STM32Cube/Repository//Packs/STMicroelectronics/X-CUBE-AZRTOS-G0/1.1.0/Middlewares/ST/threadx/common/inc/ -IC:/Users/bens1/STM32Cube/Repository//Packs/STMicroelectronics/X-CUBE-AZRTOS-G0/1.1.0/Middlewares/ST/threadx/ports/cortex_m0/gnu/inc/ -I"C:/Users/bens1/Documents/Other/Projects/Keyboard/Firmware/keyboard_right_firmware/Application/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-AZURE_RTOS-2f-App

clean-AZURE_RTOS-2f-App:
	-$(RM) ./AZURE_RTOS/App/app_azure_rtos.cyclo ./AZURE_RTOS/App/app_azure_rtos.d ./AZURE_RTOS/App/app_azure_rtos.o ./AZURE_RTOS/App/app_azure_rtos.su

.PHONY: clean-AZURE_RTOS-2f-App

