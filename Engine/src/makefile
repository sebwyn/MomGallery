IDIR=../include
CC=g++
LINKER=ld

INCLUDES=-I/usr/local/opt/libxml2/include/libxml2
LIBS=-lxml2 -lGLEW -lglfw -framework OpenGL -L/usr/local/opt/libxml2/lib

CFLAGS=-g -std=c++11 -I $(IDIR) $(INCLUDES)
LFLAGS=-r -v

ODIR=.obj

EXEC=../lib/Engine.o

DEPDIR=$(ODIR)/.deps
DEPFLAGS=-MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

FINDSRC=$(shell find . -type f -name "*.cpp")
SRCS=$(FINDSRC:./%.cpp=%.cpp)
OBJ=$(SRCS:%.cpp=$(ODIR)/%.o)
DEPS=$(SRCS:%.cpp=$(DEPDIR)/%.d)

COMPILE.c=$(CC) $(CFLAGS) $(DEPFLAGS) -c

%.o : %.cpp
$(ODIR)/%.o: %.cpp $(DEPDIR)/%.d | $(DEPDIR)
	mkdir -p "$(@D)"
	mkdir -p $(patsubst $(ODIR)/%, $(DEPDIR)/%, $(@D))
	$(COMPILE.c) $< -o $@ 
	$(POSTCOMPILE)

$(EXEC): $(OBJ)
	$(LINKER) $(LFLAGS) -o $@ $^

$(DEPDIR):
	@mkdir -p $@

$(DEPS): 
include $(wildcard $(DEPS))

.PHONY: clean

clean:
	-rm $(OBJ)
	-rm $(EXEC)
	-rm $(DEPS)
