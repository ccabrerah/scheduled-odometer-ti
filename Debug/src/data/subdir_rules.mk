################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/data/movement_db.obj: ../src/data/movement_db.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/TMS470 Code Generation Tools 4.6.6/bin/cl470" -mv7M3 -g -O2 --relaxed_ansi --gcc --define=ccs --define=TARGET_IS_DUSTDEVIL_RA0 --define=PART_LM3S3748 --include_path="C:/Program Files (x86)/Texas Instruments/ccsv4/tools/compiler/TMS470 Code Generation Tools 4.6.6/include" --include_path="C:/StellarisWare/" --include_path="C:/StellarisWare/boards/ek-lm3s3748" --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="src/data/movement_db.pp" --obj_directory="src/data" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


