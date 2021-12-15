SCHEDULING_POLICIES_SRC_DIR = scheduling_policies/src
SCHEDULING_POLICIES_BUILD_DIR = scheduling_policies/build
ROOT_BUILD_DIR = build


permission_management: clean_up
	@for f in $(shell ls ${SCHEDULING_POLICIES_BUILD_DIR}); do chmod 700 ./${SCHEDULING_POLICIES_BUILD_DIR}/$${f} ; done && chmod 700 ./main


clean_up: main_executable
	rm ${SCHEDULING_POLICIES_BUILD_DIR}/*.o && rm ${ROOT_BUILD_DIR}/*.o


main_executable: scheduler.o file_manager.o analysis.o scheduling_policies_executables
	gcc main.c -o main


scheduling_policies_executables: scheduling_policies.o 
	@for f in $(shell ls -1 ${SCHEDULING_POLICIES_BUILD_DIR}/*.o | xargs -n 1 basename | sed 's/\.[a-z]*//g'); do gcc ${ROOT_BUILD_DIR}/file_manager.o ${ROOT_BUILD_DIR}/scheduler.o ${ROOT_BUILD_DIR}/analysis.o ${SCHEDULING_POLICIES_BUILD_DIR}/$${f}.o -o ${SCHEDULING_POLICIES_BUILD_DIR}/$${f} ; done


scheduling_policies.o: scheduling_policies/src/*.c
	@for f in $(shell ls -1 ${SCHEDULING_POLICIES_SRC_DIR}/*.c | xargs -n 1 basename | sed 's/\.[a-z]*//g'); do gcc -c ${SCHEDULING_POLICIES_SRC_DIR}/$${f}.c -o ${SCHEDULING_POLICIES_BUILD_DIR}/$${f}.o ; done


scheduler.o: scheduler/scheduler.c scheduler/scheduler_functions.h scheduler/types.h
	gcc -c scheduler/scheduler.c -o ${ROOT_BUILD_DIR}/scheduler.o


file_manager.o: file_manager/file_manager.c file_manager/file_manager_functions.h scheduler/types.h
	gcc -c file_manager/file_manager.c -o build/file_manager.o


analysis.o: analysis/analysis.c analysis/analysis_functions.h analysis/analysis.h
	gcc -c analysis/analysis.c -o build/analysis.o