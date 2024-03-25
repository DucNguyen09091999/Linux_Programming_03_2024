PROJ_NAME := list dir
INC_DIR :=inc/
SRC_DIR := src/
CC := gcc
OBJ_FILE := obj/main.o
OBJ_FILE += obj/file_ops.o \
			obj/log.o \
			main_func.o
CFLAGS := -I./inc/ -fms-extensions

obj/main.o: main.c 
	@$(CC) -c $(CFLAGS) $^ -o $@ -fPIC
	
obj/file_ops.o: $(SRC_DIR)file_ops.c 
	@$(CC) -c $(CFLAGS) $^ -o $@ -fPIC

obj/log.o: $(SRC_DIR)log.c 
	@$(CC) -c $(CFLAGS) $^ -o $@ -fPIC

obj/main_func.o: $(SRC_DIR)main_func.c 
	@$(CC) -c $(CFLAGS) $^ -o $@ -fPIC

clean:
	@rm -rf bin/* obj/* lib/shared/* lib/static/*
	@echo "Delete all file"

main_run:
	@./bin/main

build: $(OBJ_FILE)	
	@$(CC) -o bin/main $^
	@echo "Finished build process"