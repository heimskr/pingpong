COMPILER		 = clang++
CFLAGS			:= -std=c++2a -g -ggdb -O0 -Wall -Wextra
LDFLAGS			:=
CC				 = $(COMPILER) $(strip $(CFLAGS) $(CHECKFLAGS))
CHECKFLAGS		:=
MKBUILD			:= mkdir -p build
CHECK			:= asan
SDKFLAGS		:= --sysroot /etc/sdk

ifeq ($(CHECK), asan)
	CHECKFLAGS += -fsanitize=address -fno-common
else ifeq ($(CHECK), msan)
	CHECKFLAGS += -fsanitize=memory -fno-common
endif

.PHONY: all test clean depend mkbuild
all: Makefile

# Peter Miller, "Recursive Make Considered Harmful" (http://aegis.sourceforge.net/auug97.pdf)
MODULES			:= core commands events messages lib test
COMMONSRC		:=
CFLAGS			+= -Iinclude
LIBS			:=
SRC				:=
include $(patsubst %,src/%/module.mk,$(MODULES))
SRC				+= $(COMMONSRC)
COMMONOBJ		:= $(patsubst src/%.cpp,build/%.o, $(filter %.cpp,$(COMMONSRC)))
OBJ				:= $(patsubst src/%.cpp,build/%.o, $(filter %.cpp,$(SRC)))
sinclude $(patsubst %,src/%/targets.mk,$(MODULES))

all: $(COMMONOBJ)

test: build/tests
	./build/tests

grind: build/tests
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=yes ./build/tests

clean:
	rm -rf build

build/%.o: src/%.cpp
	@ mkdir -p "$(shell dirname "$@")"
	$(CC) $(strip $(SDKFLAGS) $(strip $(CPPFLAGS)) $(strip $(CXXFLAGS))) -c $< -o $@

DEPFILE  = .dep
DEPTOKEN = "\# MAKEDEPENDS"
DEPFLAGS = -f $(DEPFILE) -s $(DEPTOKEN)

depend:
	@ echo $(DEPTOKEN) > $(DEPFILE)
	makedepend $(DEPFLAGS) -- $(CC) -- $(SRC) 2>/dev/null
	@ sed -i .sed 's/^src\//build\//' $(DEPFILE)
	@ rm $(DEPFILE).bak $(DEPFILE).sed

sinclude $(DEPFILE)
