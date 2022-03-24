#please use 'make clean' to clean the directory of intermediate build files and the executable
#simply typing 'make' will compile all source code files to object files .o, and then link all
#object files into an executable
#we are using a lot of makefile macros

#changing platform dependant stuff, do not change this
# Linux (default)
LDFLAGS = -lGL -lGLU -lglut
CFLAGS=-O2 -Wall -std=c++11
LIBFLAGS= -L./lib
CC=g++ -I./include
EXEEXT=
RM=rm

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	EXEEXT=.exe #on windows applications must have .exe extension
	RM=del #rm command for windows powershell
    LDFLAGS = -lfreeglut -lglu32 -lopengl32 -lgdi32 -lwinmm
else
	# OS X
	OS := $(shell uname)
	ifeq ($(OS), Darwin)
	        LDFLAGS = -framework Carbon -framework OpenGL -framework GLUT
	endif
endif

#change the 't1' name to the name you want to call your application
PROGRAM_NAME= Project

#run target to compile and build, and then launch the executable
run: $(PROGRAM_NAME)
	./$(PROGRAM_NAME)$(EXEEXT)

#when adding additional source files, such as boilerplateClass.cpp
#or yourFile.cpp, add the filename with an object extension below
#ie. boilerplateClass.o and yourFile.o
#make will automatically know that the objectfile needs to be compiled
#form a cpp source file and find it itself :)
$(PROGRAM_NAME): main.o Game.o object.o TTexture.o glutUtil.cpp objLoader.cpp MyObj.cpp
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBFLAGS)

main.o: main.cpp
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

Game.o: Game.cpp
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

object.o: object.cpp
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)
	
TTexture.o: TTexture.cpp
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)
	
glutUtil.o: glutUtil.cpp
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)
	
objLoader.o: objLoader.cpp
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)
	
MyObj.o: MyObj.cpp
	$(CC) -c $< $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) *.o $(PROGRAM_NAME)$(EXEEXT)