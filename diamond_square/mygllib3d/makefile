 #------------------------------------------------------------------------------
# Makefile for object code in mygllib
# Y. Liow
#------------------------------------------------------------------------------
# Macros
#------------------------------------------------------------------------------
CXX 	= g++
CXXFLAGS= -g -Wall
LINK = g++
LINKFLAGS = -lGL -lGLU -lglut -lGLEW
OBJS = config.o reshape.o SingletonView.o Keyboard.o DisplayLists.o View.o

#------------------------------------------------------------------------------
# Build all object code
#------------------------------------------------------------------------------
all:	$(OBJS)
	$(NOOP)

#------------------------------------------------------------------------------
# Object codes
#------------------------------------------------------------------------------
config.o: config.h config.cpp
	$(CXX) $(CXXFLAGS) config.cpp -c -o config.o
	
SingletonView.o: SingletonView.h SingletonView.cpp
	$(CXX) $(CXXFLAGS) SingletonView.cpp -c -o View.o

reshape.o: reshape.h reshape.cpp
	$(CXX) $(CXXFLAGS) reshape.cpp -c -o reshape.o
	
Keyboard.o: Keyboard.h Keyboard.cpp
	$(CXX) $(CXXFLAGS) Keyboard.cpp -c -o Keyboard.o

DisplayLists.o: DisplayLists.h
	$(CXX) $(CXXFLAGS) DisplayLists.h -c -o DisplayLists.o

#------------------------------------------------------------------------------
# Utilities
#------------------------------------------------------------------------------
clean:
	rm -f $(OBJS)
c:
	rm -f $(OBJS)
