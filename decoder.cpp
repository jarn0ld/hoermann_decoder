#include "decoder.h"
#include <complex>
#include <iostream>
#include <fstream>
#include <ostream>

decoder::decoder() : grabber()
{
}

void decoder::run()
{
  grabber.start_streaming();

  std::ofstream log;
  log.open("decoder_out.csv");

  bool in_packet=false;
  int packet_start = 0;
  std::complex<float> carry;

  while(true) {

    auto buffer = grabber.get_buffer_from_queue();

    if(!in_packet) {

      for(int i = 0;i < buffer.size(); i++) {

        std::complex<float> samp = buffer[i];

        if(std::abs(samp) > 0.01) {

          in_packet = true;
          packet_start = i;
          std::cout << "found packet at sample: "
                    << i << std::endl;
          break;

        }

      }

    }

    if(in_packet) {
      for(int i = packet_start; i < buffer.size() - 1; i++) {
        log << std::arg(buffer[i + 1] * std::conj(buffer[i])) << ";" << std::flush;
      }

    }

  }

}

