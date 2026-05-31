CXX = g++
CXXFLAGS = -Wall -std=c++14 -I include
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRC = $(wildcard *.cpp src/*.cpp)
OBJ = $(SRC:.cpp=.o)
EXEC = TopDownShooter

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)