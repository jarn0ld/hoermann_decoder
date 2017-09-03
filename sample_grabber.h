#include <complex>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class sample_grabber {

  std::queue<std::vector<std::complex<float>>> sample_queue;
  bool run_streaming;
  std::thread grabber_thread;
  std::mutex queue_mutex;
  std::condition_variable cond_var;

  public:
    sample_grabber();
    void start_streaming();
    std::vector<std::complex<float>> get_buffer_from_queue();
  private:
    void grab_samples();
};
