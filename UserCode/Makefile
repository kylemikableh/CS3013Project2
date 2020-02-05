all: testsysinvoke

testsysinvoke: testsysinvoke.o
	gcc -pthread testsysinvoke.o -o testsysinvoke

testsysinvoke.o: testsysinvoke.c
	gcc -g -c testsysinvoke.c

clean:
	rm -f *.o testsysinvoke
