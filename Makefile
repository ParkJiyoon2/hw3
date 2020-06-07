
all:
	gcc vm_hw3.c pglist_hw3.c -o vm

clean:
	rm vm *.o
