CX = g++
CXFLAGS = -g -Wall 
DXLFLAGS = -I/usr/local/include/dynamixel_sdk_cpp
DXLFLAGS += -ldxl_x64_cpp 
DXLFLAGS += `pkg-config opencv4 --cflags --libs`
TARGET = linetracer
OBJS = main.o dxl.o vision.o
$(TARGET) : $(OBJS)
	$(CX) $(CXFLAGS) -o $(TARGET) $(OBJS) $(DXLFLAGS)
main.o : main.cpp
	$(CX) $(CXFLAGS) -c main.cpp $(DXLFLAGS)
dxl.o : dxl.hpp dxl.cpp
	$(CX) $(CXFLAGS) -c dxl.cpp $(DXLFLAGS)
vision.o : vision.hpp vision.cpp
	$(CX) $(CXFLAGS) -c vision.cpp $(DXLFLAGS)
.PHONY: all clean
all: $(TARGET)
clean:
	rm -rf $(TARGET) $(OBJS)
