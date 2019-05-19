build/tests: test/tests.o $(COMMONOBJ)
	@ $(MKBUILD)
	$(CC) $^ -o $@ $(LDFLAGS)
