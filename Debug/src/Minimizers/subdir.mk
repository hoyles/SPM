################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Minimizers/CMinimizer.cpp 

OBJS += \
./src/Minimizers/CMinimizer.o 

CPP_DEPS += \
./src/Minimizers/CMinimizer.d 


# Each subdirectory must supply rules for building sources it contributes
src/Minimizers/%.o: ../src/Minimizers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DOPTIMISE -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


