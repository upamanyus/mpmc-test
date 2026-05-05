.PHONY: all
all: test

test: test.c mpmc.h

.PHONY: clean
clean:
	rm -f test
