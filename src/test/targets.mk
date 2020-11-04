build/tests: build/test/Tests.o $(OBJECTS)
	@ $(MKBUILD)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)
