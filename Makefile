EIGEN ?= /home/maxiwell/devel/eigen/
CXX ?= /opt/at14.0/bin/g++

all:
	$(CXX) $(CXXFLAGS) -fno-inline-small-functions -fno-inline-functions -g main.cpp -o main -I$(EIGEN)

trace:
	$(CXX) -c trace.cpp
	$(CXX) -fno-inline-small-functions -finstrument-functions -g -c main.cpp -I$(EIGEN)
	$(CXX) -no-pie trace.o main.o -o main


