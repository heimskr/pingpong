COMPILER		:= clang++
CFLAGS			:= -std=c++17 -g -O3 -Wall -Wextra -fdiagnostics-color=always
LDFLAGS			:=
CC				 = $(COMPILER) $(CFLAGS)
MKBUILD			:= mkdir -p build

.PHONY: all test clean depend
all:

# Peter Miller, "Recursive Make Considered Harmful" (http://aegis.sourceforge.net/auug97.pdf)
MODULES			:= core test commands
COMMONSRC		:=
CFLAGS			+= $(patsubst %,-I%,$(MODULES))
LIBS			:=
SRC				:=
include $(patsubst %,%/module.mk,$(MODULES))
SRC				+= $(COMMONSRC)
COMMONOBJ		:= $(patsubst %.cpp,%.o, $(filter %.cpp,$(COMMONSRC)))
OBJ				:= $(patsubst %.cpp,%.o, $(filter %.cpp,$(SRC)))
include $(patsubst %,%/targets.mk,$(MODULES))

all: $(COMMONOBJ)
	

test: build/tests
	./build/tests

clean:
	rm -rf build
	rm -f *.o **/*.o

%.o: %.cpp
	$(CC) -c $^ -o $@

DEPFILE  = .dep

depend:
	$(CC) -MM $(SRC) > $(DEPFILE)

sinclude $(DEPFILE)
