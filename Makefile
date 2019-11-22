all:
	echo ERROR: proc or thread target must be specified

proc:
	gcc -c multitest_proc.c
	gcc searchtest.c multitest_proc.o -o searchtest -lm

thread:
	gcc -c multitest_thread.c
	gcc searchtest.c multitest_thread.o -o searchtest -lpthread -lm

clean:
	rm -f searchtest
	rm -f multitest_proc.o
	rm -f multitest_thread.o