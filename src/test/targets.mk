build/tests: build/test/tests.o $(OBJECTS)
	@ $(MKBUILD)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)
