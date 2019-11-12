all:
        echo ERROR: proc or thread target must be specified

proc:
        gcc -c multitest_proc.c
        gcc searchtest.c multitest_proc.o -o searchtest

thread:
        gcc -c multitest_thread.c
        gcc searchtest.c multitest_thread.o -o searchtest

clean:
        rm searchtest
        rm multitest_proc.o
        rm multitest_thread.o