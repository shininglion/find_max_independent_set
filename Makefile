CXX	=	g++ -std=c++11 -Wall -O2

all: mis

mis: main.cpp mis.h mis.cpp value_type.hpp
	$(CXX) $< -o $@

clean:
	rm mis
