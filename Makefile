C=g++
CXXFLAGS= -Wall -Wvla -Wextra -Werror -g -std=c++14
LDFLAGS= -lm
HEADERS= vl_vector.h
OBJS= test_runner.o tests.o

%.o : %.c

ex7_tests: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

run_tests: $(OBJS)
	make ex7_tests && ./ex7_tests

run_valgrind: $(OBJS)
	make ex7_tests && valgrind --leak-check=full ./ex7_tests

run_presubmit:
	tar -zcvf ex7.tar.gz vl_vector.h && ./run ex7.tar.gz

$(OBJS) : $(HEADERS)

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf ex7_tests
