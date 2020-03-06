
CXX       := arm-linux-gnueabihf-gcc

CXX_FLAGS := #-Wall -Wextra -std=c11 -ggdb -march=armv6 -mfloat-abi=hard -mfpu=vfp


BIN     := bin

SRC     := src

INCLUDE := include

LIB     := lib


LIBRARIES   := -lwiringPi -lm -lpthread -lrt -lcrypt

EXECUTABLE  := displayTemp



all: $(BIN)/$(EXECUTABLE)


run: clean all

	clear

	./$(BIN)/$(EXECUTABLE)


$(BIN)/$(EXECUTABLE): $(SRC)/*.c

	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)


clean:

	-rm $(BIN)/*
