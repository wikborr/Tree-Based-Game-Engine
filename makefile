OBJS = *.cpp src/*.cpp deps/*.c deps/*.cpp projects/test/src/*.hpp

CC = g++

INCLUDE_PATHS = -I./deps/include -I./src  -I./projects

LIBRARY_PATHS = -L./deps/lib

# -w - warning supression
# -Wl,-subsystem,windows - no console window
COMPILER_FLAGS = 

LINKER_FLAGS = -lglfw3 -lgdi32 -lopengl32

OBJ_NAME = main

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)