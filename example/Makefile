CC       := gcc
CXX      := g++
CFLAGS   := -g -finstrument-functions -rdynamic
CXXFLAGS := -g -finstrument-functions -rdynamic

.PHONY: run
run: crumble_c crumble_cpp
	echo "C executable:"
	./crumble_c
	echo "C++ executable:"
	./crumble_cpp

crumble_c: crumble.c ../src/etrace.c
	$(CC) $(CFLAGS) -o $@ $^ -ldl -lrt

crumble_cpp: crumble.cpp ../src/etrace.c
	$(CXX) $(CXXFLAGS) -o $@ $^ -ldl -lrt

.PHONY: clean
clean:
	rm -f crumble_c crumble_cpp
