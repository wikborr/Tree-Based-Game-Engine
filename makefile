OBJS = *.cpp src/*.cpp src/servers/*.cpp src/resources/*.cpp src/leaves/*.cpp src/leaves/leaves2D/*.cpp deps/*.c deps/*.cpp projects/demo/prj_src/*.cpp projects/demo/res.res

CC = g++

INCLUDE_PATHS = -I./deps/include -I./src  -I./projects/demo/prj_src

LIBRARY_PATHS = -L./deps/lib

# -w - warning supression
# -Wl,-subsystem,windows - no console window
COMPILER_FLAGS = 

LINKER_FLAGS = -lglfw3 -lgdi32 -lopengl32

OBJ_NAME = main

all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)