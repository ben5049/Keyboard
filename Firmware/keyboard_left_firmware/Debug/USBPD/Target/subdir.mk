################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USBPD/Target/usbpd_dpm_user.c \
../USBPD/Target/usbpd_pwr_user.c \
../USBPD/Target/usbpd_vdm_user.c 

OBJS += \
./USBPD/Target/usbpd_dpm_user.o \
./USBPD/Target/usbpd_pwr_user.o \
./USBPD/Target/usbpd_vdm_user.o 

C_DEPS += \
./USBPD/Target/usbpd_dpm_user.d \
./USBPD/Target/usbpd_pwr_user.d \
./USBPD/Target/usbpd_vdm_user.d 


# Each subdirectory must supply rules for building sources it contributes
USBPD/Target/%.o USBPD/Target/%.su USBPD/Target/%.cyclo: ../USBPD/Target/%.c USBPD/Target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32G0C1xx -DUSBPD_PORT_COUNT=2 -D_DRP -DUSBPDCORE_LIB_PD3_FULL -DTX_INCLUDE_USER_DEFINE_FILE -DUX_INCLUDE_USER_DEFINE_FILE -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../AZURE_RTOS/App -I../USBX/App -I../USBX/Target -I../USBPD/App -I../USBPD/Target -I../Middlewares/ST/STM32_USBPD_Library/Core/inc -I../Middlewares/ST/STM32_USBPD_Library/Devices/STM32G0XX/inc -I../Middlewares/ST/usbx/common/core/inc/ -I../Middlewares/ST/usbx/ports/generic/inc/ -I../Middlewares/ST/usbx/common/usbx_device_classes/inc/ -I../Middlewares/ST/usbx/common/usbx_stm32_device_controllers/ -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m0/gnu/inc/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-USBPD-2f-Target

clean-USBPD-2f-Target:
	-$(RM) ./USBPD/Target/usbpd_dpm_user.cyclo ./USBPD/Target/usbpd_dpm_user.d ./USBPD/Target/usbpd_dpm_user.o ./USBPD/Target/usbpd_dpm_user.su ./USBPD/Target/usbpd_pwr_user.cyclo ./USBPD/Target/usbpd_pwr_user.d ./USBPD/Target/usbpd_pwr_user.o ./USBPD/Target/usbpd_pwr_user.su ./USBPD/Target/usbpd_vdm_user.cyclo ./USBPD/Target/usbpd_vdm_user.d ./USBPD/Target/usbpd_vdm_user.o ./USBPD/Target/usbpd_vdm_user.su

.PHONY: clean-USBPD-2f-Target
