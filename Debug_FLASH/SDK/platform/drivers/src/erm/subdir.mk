################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SDK/platform/drivers/src/erm/erm_driver.c \
../SDK/platform/drivers/src/erm/erm_hw_access.c 

OBJS += \
./SDK/platform/drivers/src/erm/erm_driver.o \
./SDK/platform/drivers/src/erm/erm_hw_access.o 

C_DEPS += \
./SDK/platform/drivers/src/erm/erm_driver.d \
./SDK/platform/drivers/src/erm/erm_hw_access.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/erm/%.o: ../SDK/platform/drivers/src/erm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@SDK/platform/drivers/src/erm/erm_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


