build/tests: build/test/tests.o $(COMMONOBJ)
	@ $(MKBUILD)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)
