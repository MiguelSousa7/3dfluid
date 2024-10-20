CPP = g++ -Wall -pg -march=native -mtune=native  -Ofast -fprefetch-loop-arrays -ffast-math -funroll-loops -flto -
fprefetch-loop-arrays -floop-block -floop-interchange
SRCS = main.cpp fluid_solver.cpp EventManager.cpp

all:
    $(CPP) $(SRCS) -o fluid_sim

clean:
    @echo Cleaning up...
    @rm fluid
    @echo Done.