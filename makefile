OBJ = $(patsubst %.c, %.o, $(wildcard *.c));
shell : $(OBJ)
	gcc -o $@ $^
clean:
	rm shell *.o
