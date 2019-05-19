COMPILER		:= clang++
CFLAGS			:= -std=c++17 -g -O3 -Wall -Wextra
CC				 = $(COMPILER) $(CFLAGS)
MKBUILD			:= mkdir -p build

.PHONY: first depend clean all builddir
first: all

# Peter Miller, "Recursive Make Considered Harmful" (http://aegis.sourceforge.net/auug97.pdf)
MODULES			:= core test
COMMONSRC		:=
CFLAGS			+= $(patsubst %,-I%,$(MODULES))
LIBS			:=
SRC				:=
COMMONOBJ		 = $(patsubst %.cpp,%.o, $(filter %.cpp,$(COMMONSRC)))
OBJ				 = $(patsubst %.cpp,%.o, $(filter %.cpp,$(SRC)))
include $(patsubst %,%/module.mk,$(MODULES))
SRC				+= $(COMMONSRC)

all: $(COMMONOBJ)

test: build/tests
	./build/tests

builddir:
	mkdir -p build

clean:
	rm -rf build
	rm -f *.o **/*.o

%.o: %.cpp
	$(CC) -c $^ -o $@

DEPFILE  = .dep

depend:
	$(CC) -MM $(SRC) > $(DEPFILE)

sinclude $(DEPFILE)