CC     := gcc
CFLAGS := -g -finstrument-functions --std=gnu99

crumble: crumble.c ../src/ptrace.c
	$(CC) $(CFLAGS) -o crumble crumble.c ../src/ptrace.c

run: crumble
	../src/etrace crumble

clean:
	rm -f crumble TRACE
