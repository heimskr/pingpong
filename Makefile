COMPILER		 = g++
CFLAGS			:= -std=c++17 -g -ggdb -O0 -Wall -Wextra
LDFLAGS			:=
CC				 = $(COMPILER) $(strip $(CFLAGS) $(CHECKFLAGS))
CHECKFLAGS		:=
MKBUILD			:= mkdir -p build

ifeq ($(shell uname -s), Darwin)
	SDKFLAGS	:= --sysroot /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk
endif

ifeq ($(CHECK), asan)
	CHECKFLAGS += -fsanitize=address -fno-common
else ifeq ($(CHECK), msan)
	CHECKFLAGS += -fsanitize=memory -fno-common
endif

.PHONY: all test clean depend mkbuild
all: Makefile


SOURCES			:= $(shell find -L src -name '*.cpp' | sed -nE '/(tests?|test_.+)\.cpp$$/!p')
OBJECTS			:= $(patsubst src/%.cpp,build/%.o, $(SOURCES))

sinclude $(shell find src -name 'targets.mk')


all: $(OBJECTS) build/tests

test: build/tests
	./build/tests

grind: build/tests
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=yes ./build/tests

clean:
	rm -rf build

build/%.o: src/%.cpp
	@ mkdir -p "$(shell dirname "$@")"
	$(CC) $(strip $(SDKFLAGS) $(CPPFLAGS) $(CXXFLAGS) -Iinclude -c) $< -o $@

DEPFILE  = .dep
DEPTOKEN = "\# MAKEDEPENDS"
DEPFLAGS = -f $(DEPFILE) -s $(DEPTOKEN)

depend:
	@ echo $(DEPTOKEN) > $(DEPFILE)
	makedepend $(DEPFLAGS) -- $(CC) -- $(SRC) 2>/dev/null
	@ sed -i.sed 's/^src\//build\//' $(DEPFILE)
	@ rm $(DEPFILE).bak $(DEPFILE).sed

sinclude $(DEPFILE)
