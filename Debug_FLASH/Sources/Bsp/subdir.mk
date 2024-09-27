################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Bsp/BSP_Config.c \
../Sources/Bsp/CallBack.c \
../Sources/Bsp/osif_ext.c 

OBJS += \
./Sources/Bsp/BSP_Config.o \
./Sources/Bsp/CallBack.o \
./Sources/Bsp/osif_ext.o 

C_DEPS += \
./Sources/Bsp/BSP_Config.d \
./Sources/Bsp/CallBack.d \
./Sources/Bsp/osif_ext.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/Bsp/%.o: ../Sources/Bsp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/Bsp/BSP_Config.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


