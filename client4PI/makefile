CXX = g++
CFLAGS = -std=c++17 -Wall -g -pthread
TARGET = pi_net_client4pi
OBJS = *.cpp ../public/*.cpp
LINK = `mysql_config --cflags --libs`

all:
	mkdir -p ../build/
	$(CXX) $(CFLAGS) $(OBJS) -o $(TARGET) $(LINK)
	cp $(TARGET) ../build/

clean:
	mkdir -p ../build/
	rm -rf $(TARGET) ../build/$(TARGET)
