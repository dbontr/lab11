CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

TARGET = lab11
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	rm -f $(OBJS) $(TARGET)
