CXX = g++
CFLAGS = -std=c++17 -Wall -g -pthread `mysql_config --cflags --libs`
TARGET = pi_net
OBJS = *.cpp ../public/*.cpp

all:
	mkdir -p ../build/
	$(CXX) $(CFLAGS) $(OBJS) -o $(TARGET)
	cp $(TARGET) ../build/

clean:
	mkdir -p ../build/
	rm -rf $(TARGET) ../build/$(TARGET)
