SRC += test/tests.cpp

build/tests: test/tests.o $(COMMONOBJ)
	@ $(MKBUILD)
	$(CC) $^ -o $@
