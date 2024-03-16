	
PROJ_NAME := list dir
INC_DIR :=inc/
SRC_DIR := ./src/
CC := gcc
OBJ_FILE := obj/main.o
OBJ_FILE += obj/data.o \
			obj/parser.o \
			obj/print.o \
			obj/sort.o
CFLAGS := -I./inc/ -fms-extensions
STATIC_DIR := lib/static/
SHARED_DIR := lib/shared/

.PHONY:build clean main_run static static_link shared shared_link static_run shared_run

obj/main.o: main.c 
	@$(CC) -c $(CFLAGS) $^ -o $@ -fPIC
	
obj/data.o: $(SRC_DIR)data.c 
	@$(CC) -c $(CFLAGS) $^ -o $@ -fPIC

obj/parser.o: $(SRC_DIR)parser.c 
	@$(CC) -c $(CFLAGS) $^ -o $@ -fPIC

obj/print.o: $(SRC_DIR)print.c 
	@$(CC) -c $(CFLAGS) $^ -o $@ -fPIC	

obj/sort.o: $(SRC_DIR)sort.c 
	@$(CC) -c $(CFLAGS) $^ -o $@ -fPIC

clean:
	@rm -rf bin/* obj/* lib/shared/* lib/static/*
	@echo "Delete all file"
main_run:
	@./bin/main

build: $(OBJ_FILE)	
	@$(CC) -o bin/main $^
	@echo "Finished build process"

#create static library
static:
	@ar rcs $(STATIC_DIR)liblist.a $(OBJ_FILE)
	@echo "Create static library"
#use static lib to create main.exe
static_link:
	@$(CC) obj/main.o -Llib/static -llist -o bin/static_main
	@echo "Finished build process"

shared:
	@$(CC) -shared -o $(SHARED_DIR)liblist.so $(OBJ_FILE)

shared_link:
	@$(CC) obj/main.o -Llib/shared -llist -o bin/shared_main
static_run:
	@./bin/static_main
shared_run:
	@./bin/shared_main
#khi tao shared file
#sudo cp libcalc.so /usr/lib
#de them shared lib vao linux

#neu muon xoa shared lib
#sudo rm /usr/lib/libcalc.so