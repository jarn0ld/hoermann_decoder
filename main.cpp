#include "sample_grabber.h"
#include <iostream>
#include <thread>
#include <chrono>

int main()
{
  sample_grabber grabber;
  grabber.start_streaming();
  while(true) {

    std::cout << "processing" << std::endl;
    auto buffer = grabber.get_buffer_from_queue();
    std::cout << buffer.size() << std::endl;

  }
  return 0;
}
