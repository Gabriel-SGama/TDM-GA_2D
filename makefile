# Name of the project
PROJ_NAME=tdm-GA.o
PROJ_NAME_DEBUG=debugTDM.o

# .cpp files
CPP_SOURCE=$(wildcard ./src/*.cpp)
CU_SOURCE = $(wildcard ./src/*.cu)

# .h files
H_SOURCE=$(wildcard ./src/*.h)
CUH_SOURCE = $(wildcard ./src/*.cuh)

# Object files
OBJ=$(subst .cpp,.o,$(subst src,objects,$(CPP_SOURCE)))
CUDA_OBJ=$(subst .cu,.o,$(subst src,objects,$(CU_SOURCE)))

# Compiler and linker
# CC=g++
CC=nvcc

# Flags for compiler
CC_FLAGS= -c	\
		  -lpthread \
		  -O3

CC_FLAGS_DEBUG = -g \
		   		 -c \
		  		 -lpthread \
				 -O3

# Command used at clean target
RM = rm -rf

OPENCV = `pkg-config --cflags --libs opencv4`
LIBS = $(OPENCV)

#
# Compilation and linking
#
all: objFolder $(PROJ_NAME)

$(PROJ_NAME): $(OBJ) $(CUDA_OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@ $(LIBS) -lpthread
	@ echo 'Finished building binary: $@'
	@ echo ' '

./objects/%.o: ./src/%.cpp ./src/headers/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@ $(LIBS)
	@ echo ' '

./objects/%.o: ./src/%.cu ./src/headers/%.cuh
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

$(PROJ_NAME_DEBUG): $(OBJ) $(CUDA_OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@ $(LIBS) -lpthread
	@ echo 'Finished building binary: $@'
	@ echo ' '

./objectsDebug/%.o: ./src/%.cpp ./src/headers/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS_DEBUG) -o $@ $(LIBS)
	@ echo ' '

./objects/%.o: ./src/%.cu ./src/headers/%.cuh
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@ $(LIBS)
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
