################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/Broad/uCommon.c \
../Sources/Broad/uInit.c 

OBJS += \
./Sources/Broad/uCommon.o \
./Sources/Broad/uInit.o 

C_DEPS += \
./Sources/Broad/uCommon.d \
./Sources/Broad/uInit.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/Broad/%.o: ../Sources/Broad/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/Broad/uCommon.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


