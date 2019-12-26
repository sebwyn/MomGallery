IDIR=../include
ENGINC=../../Engine/include
CC=g++
CFLAGS= -g -std=c++11 -I $(IDIR) -I $(ENGINC) -I/usr/local/opt/libxml2/include/libxml2

LIBS=-lGLEW -lglfw -framework OpenGL -lxml2 -L/usr/local/opt/libxml2/lib

ODIR=.obj
DEPDIR=$(ODIR)/.dep
DEPFLAGS=-MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

FINDSRC=$(shell find . -type f -name "*.cpp")
SRCS=$(FINDSRC:./%.cpp=%.cpp)
OBJ=$(SRCS:%.cpp=$(ODIR)/%.o)
OBJ+=../../Engine/lib/Engine.o
DEPS=$(SRCS:%.cpp=$(DEPDIR)/%.d)

COMPILE.c=$(CC) $(DEPFLAGS) $(CFLAGS) -c

$(ODIR)/%.o: %.cpp $(DEPDIR)/%.d | $(DEPDIR) 
	mkdir -p "$(@D)"
	mkdir -p $(patsubst $(ODIR)/%, $(DEPDIR)/%, $(@D))		
	$(COMPILE.c) -o $@ $<
	$(POSTCOMPILE)

Game: $(OBJ)
	$(CC) -g -o $@ $^ $(LIBS)

$(DEPS):
include $(wildcard $(DEPS))

.PHONY: clean

clean:
	rm main.o MyGame.o Game
	rm -f $(COMPDIR)/obj/*.o 
