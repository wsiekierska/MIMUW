CFLAGS = -std=c17 -pedantic -Wall -Wextra -Wformat-security\
         -Wfloat-equal -Wshadow -Wconversion\
         -Wlogical-not-parentheses -Wnull-dereference -Wvla -Werror\
         -fstack-protector-strong -fsanitize=undefined -fno-sanitize-recover\
         -g -fno-omit-frame-pointer -O1
         
test.e: test.c ary.o
	gcc $(CFLAGS) test.c ary.o -o test.e

tester.e: tester.c ary.o
	gcc $(CFLAGS) tester.c ary.o -o tester.e

gra.o: ary.c ary.h
	gcc $(CFLAGS) -c ary.c

clean:
	rm -f *.e *.o
