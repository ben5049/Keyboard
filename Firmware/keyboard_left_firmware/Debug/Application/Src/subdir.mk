################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/Src/app_init.c \
../Application/Src/hid_keyboard_thread.c \
../Application/Src/hid_mouse_thread.c \
../Application/Src/msc_thread.c \
../Application/Src/scan_keys_thread.c \
../Application/Src/ux_init.c \
../Application/Src/ux_thread.c 

OBJS += \
./Application/Src/app_init.o \
./Application/Src/hid_keyboard_thread.o \
./Application/Src/hid_mouse_thread.o \
./Application/Src/msc_thread.o \
./Application/Src/scan_keys_thread.o \
./Application/Src/ux_init.o \
./Application/Src/ux_thread.o 

C_DEPS += \
./Application/Src/app_init.d \
./Application/Src/hid_keyboard_thread.d \
./Application/Src/hid_mouse_thread.d \
./Application/Src/msc_thread.d \
./Application/Src/scan_keys_thread.d \
./Application/Src/ux_init.d \
./Application/Src/ux_thread.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Src/%.o Application/Src/%.su Application/Src/%.cyclo: ../Application/Src/%.c Application/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32G0C1xx -DUSBPD_PORT_COUNT=2 -D_DRP -DUSBPDCORE_LIB_PD3_FULL -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../AZURE_RTOS/App -I../USBX/App -I../USBX/Target -I../USBPD/App -I../USBPD/Target -I../Middlewares/ST/STM32_USBPD_Library/Core/inc -I../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G0XX/inc -I../Middlewares/ST/usbx/common/core/inc/ -I../Middlewares/ST/usbx/ports/generic/inc/ -I../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m0/gnu/inc/ -I../Application/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Application-2f-Src

clean-Application-2f-Src:
	-$(RM) ./Application/Src/app_init.cyclo ./Application/Src/app_init.d ./Application/Src/app_init.o ./Application/Src/app_init.su ./Application/Src/hid_keyboard_thread.cyclo ./Application/Src/hid_keyboard_thread.d ./Application/Src/hid_keyboard_thread.o ./Application/Src/hid_keyboard_thread.su ./Application/Src/hid_mouse_thread.cyclo ./Application/Src/hid_mouse_thread.d ./Application/Src/hid_mouse_thread.o ./Application/Src/hid_mouse_thread.su ./Application/Src/msc_thread.cyclo ./Application/Src/msc_thread.d ./Application/Src/msc_thread.o ./Application/Src/msc_thread.su ./Application/Src/scan_keys_thread.cyclo ./Application/Src/scan_keys_thread.d ./Application/Src/scan_keys_thread.o ./Application/Src/scan_keys_thread.su ./Application/Src/ux_init.cyclo ./Application/Src/ux_init.d ./Application/Src/ux_init.o ./Application/Src/ux_init.su ./Application/Src/ux_thread.cyclo ./Application/Src/ux_thread.d ./Application/Src/ux_thread.o ./Application/Src/ux_thread.su

.PHONY: clean-Application-2f-Src

