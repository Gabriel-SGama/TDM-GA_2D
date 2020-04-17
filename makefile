# Name of the project
PROJ_NAME=trabSimoes.o
PROJ_NAME_DEBUG=trabSimoesDebug.o

# .c files
C_SOURCE=$(wildcard ./src/*.cpp)

# .h files
H_SOURCE=$(wildcard ./src/*.h)

# Object files
OBJ=$(subst .cpp,.o,$(subst src,objects,$(C_SOURCE)))
OBJ_DEBUG=$(subst .cpp,.o,$(subst src,objectsDebug,$(C_SOURCE)))

# Compiler and linker
CC=g++

# Flags for compiler
CC_FLAGS= -I /usr/local/include/eigen3 \
		  -c	\
          -W	\
          -Wall	\
		  -Wextra \
		  -pedantic \
		  -O3 \
		  -std=c++14

CC_FLAGS_DEBUG = -g \
		   		 -c \
		  		 -Wall

# Command used at clean target
RM = rm -rf

OPENCV = `pkg-config --cflags --libs opencv4`
LIBS = $(OPENCV)

#
# Compilation and linking
#
all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@ $(LIBS)
	@ echo 'Finished building binary: $@'
	@ echo ' '

./objects/%.o: ./src/%.cpp ./src/headers/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@ $(LIBS)
	@ echo ' '

./objects/main.o: ./src/main.cpp $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@ $(LIBS)
	@ echo ' '

objFolder:
	@ mkdir -p objects

clean:
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects

.PHONY: all clean


debug: objFolderDebug $(PROJ_NAME_DEBUG)

$(PROJ_NAME_DEBUG): $(OBJ_DEBUG)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@ $(LIBS)
	@ echo 'Finished building binary: $@'
	@ echo ' '

./objectsDebug/%.o: ./src/%.cpp ./src/headers/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS_DEBUG) -o $@ $(LIBS)
	@ echo ' '

./objectsDebug/main.o: ./src/main.cpp $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS_DEBUG) -o $@ $(LIBS)
	@ echo ' '

objFolderDebug:
	@ mkdir -p objectsDebug

cleanDebug:
	@ $(RM) ./objectsDebug/*.o $(PROJ_NAME_DEBUG) *~
	@ rmdir objectsDebug


.PHONY: all clean
