CC = gcc
CFLAGS = -Wall -I./common
SIM_CFLAGS = $(CFLAGS) -DTARGET_SIM

SIM_SRCS = app/main_logic.c hw_sim/hw_sim.c tests/test_runner.c

ifeq ($(OS), Windows_NT)
	PYTHON = py
else 
	PYTHON = python3
endif

sim: 
	@$(CC) $(SIM_CFLAGS) $(SIM_SRCS) -o sim_test
	
run: sim 
	@./sim_test
	@$(PYTHON) tests/validate_output.py
	
clean: 
	@rm -f sim_test
