################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
EX5_Taxali.obj: ../EX5_Taxali.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"E:/Installed Softwares/ti/ccs730/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="E:/Installed Softwares/ti/ccs730/ccsv7/ccs_base/msp430/include" --include_path="C:/Users/DELL/workspace_v7/Exercise5_Taxali" --include_path="E:/Installed Softwares/ti/ccs730/ccsv7/tools/compiler/ti-cgt-msp430_16.9.4.LTS/include" --advice:power=all --define=__MSP430G2253__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="EX5_Taxali.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


