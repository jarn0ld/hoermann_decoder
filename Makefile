a.out: decoder.cpp sample_grabber.cpp main.cpp
	g++ --debug -std=c++11 main.cpp decoder.cpp sample_grabber.cpp -luhd -lboost_system -lpthread

