CXX = g++
CXXFLAGS = -O3 -march=native -std=c++17 -pthread
PCAP_LIBS = -lpcap

INC = -I./include
SRC = src/main.cpp src/packet_handler.cpp src/logger.cpp src/dns_cache.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = sniffer

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(PCAP_LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
