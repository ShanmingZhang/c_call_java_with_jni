################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/calljavawithjni.cpp 

OBJS += \
./src/calljavawithjni.o 

CPP_DEPS += \
./src/calljavawithjni.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/Library/Java/JavaVirtualMachines/jdk1.8.0_101.jdk/Contents/Home -I/Library/Java/JavaVirtualMachines/jdk1.8.0_101.jdk/Contents/Home/include/darwin -I/Library/Java/JavaVirtualMachines/jdk1.8.0_101.jdk/Contents/Home/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


