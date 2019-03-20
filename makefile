CFLAGS = -Wall -Wextra -Werror -g -std=c11 -O2
program: my_string.o command_recognition.o input_read.o glowny.o history_tree.o find_and_union.o
	gcc -o program my_string.o command_recognition.o input_read.o glowny.o history_tree.o find_and_union.o

glowny.o: glowny.c my_string.h input_read.h command_recognition.h
	gcc -c $(CFLAGS) glowny.c -o glowny.o

input_read.o: input_read.c input_read.h my_string.h command_recognition.h
	gcc -c $(CFLAGS) input_read.c -o input_read.o

command_recognition.o: command_recognition.c command_recognition.h my_string.h
	gcc -c $(CFLAGS) command_recognition.c -o command_recognition.o

history_tree.o: history_tree.c history_tree.h find_and_union.h my_string.h command_recognition.h
	gcc -c $(CFLAGS) history_tree.c -o history_tree.o	

find_and_union.o: find_and_union.c find_and_union.h
	gcc -c $(CFLAGS) find_and_union.c -o find_and_union.o

my_string.o: my_string.c my_string.h
	gcc -c $(CFLAGS) my_string.c -o my_string.o

clean:
	rm -f *.o program