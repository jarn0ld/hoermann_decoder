#include "sample_grabber.h"

#include <uhd/types/tune_request.hpp>
#include <uhd/usrp/multi_usrp.hpp>

#include <iostream>
#include <chrono>
#include <vector>


sample_grabber::sample_grabber() :
  sample_queue(),
  run_streaming(true),
  grabber_thread(),
  queue_mutex()
{

}

void sample_grabber::start_streaming()
{
  grabber_thread = std::thread(&sample_grabber::grab_samples, this);
}

void sample_grabber::grab_samples()
{
    uhd::rx_metadata_t md;
    std::vector<std::complex<float>> buff(10000);

    uhd::usrp::multi_usrp::sptr usrp =
                    uhd::usrp::multi_usrp::make(uhd::device_addr_t());

    usrp->set_rx_rate(250e3);
    std::cout << "tuning" << std::endl;
    uhd::tune_request_t tune_request(868.3e6);
    usrp->set_rx_freq(tune_request);

    usrp->set_rx_gain(15);

    uhd::stream_args_t stream_args("fc32", "sc16");
    uhd::rx_streamer::sptr rx_stream = usrp->get_rx_stream(stream_args);

    uhd::stream_cmd_t stream_command(
          uhd::stream_cmd_t::stream_mode_t::STREAM_MODE_START_CONTINUOUS);
    stream_command.stream_now = true;
    stream_command.num_samps = size_t(0);
    stream_command.time_spec = uhd::time_spec_t();

    rx_stream->issue_stream_cmd(stream_command);

    uhd::ref_vector<void*> buffer(&buff.front());
    while(run_streaming) {
      size_t num_rx_samps = rx_stream->recv(buffer, buff.size(), md);
      //lock
      queue_mutex.lock();
      sample_queue.push(buff);
      cond_var.notify_one();
      queue_mutex.unlock();
    }

}

std::vector<std::complex<float>> sample_grabber::get_buffer_from_queue()
{
  std::unique_lock<std::mutex> lock(queue_mutex);
  while(sample_queue.size() == 0) {
    cond_var.wait(lock);
  }
  std::vector<std::complex<float>> buffer = sample_queue.front();
  sample_queue.pop();
  std::cout << "sample queue size now: " << sample_queue.size() << std::endl;
  return buffer;
}
