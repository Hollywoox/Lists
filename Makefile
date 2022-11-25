CC = g++

all: ListArray/list.cpp ListArray/main.cpp ListPointer/list.cpp ListPointer/main.cpp
	$(CC) ListArray/list.cpp ListArray/main.cpp -o list_arr
	$(CC) ListPointer/list.cpp ListPointer/main.cpp -o list_ptr

list_arr: ListArray/list.cpp ListArray/main.cpp
	$(CC) ListArray/list.cpp ListArray/main.cpp -o list_arr

list_ptr: ListPointer/list.cpp ListPointer/main.cpp
	$(CC) ListPointer/list.cpp ListPointer/main.cpp -o list_ptr