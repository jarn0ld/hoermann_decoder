a.out: sample_grabber.cpp main.cpp
	g++ --debug main.cpp sample_grabber.cpp -luhd -lboost_system -lpthread

