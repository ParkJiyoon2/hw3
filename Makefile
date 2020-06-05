all:
	gcc simple.c pglist.c -o vm

clean:
	rm vm *.o
