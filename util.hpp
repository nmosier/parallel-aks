#pragma once

#include <chrono>
#include <string>
#include <iostream>

class timer {
public:
  using clock = std::chrono::high_resolution_clock;
  using time_point = std::chrono::high_resolution_clock::time_point;
  timer(const std::string& title): title(title), begin(clock::now()) {}
  ~timer() {
    if (!global_enable) { return; }
    
    const auto duration = clock::now() - begin;
    std::clog << title
	      << ": "
	      << std::chrono::duration_cast<std::chrono::duration<float>>(duration).count()
	      << "s"
	      << std::endl;
  }

  static bool global_enable;

private:
  std::string title;
  time_point begin;
  time_point end;
};
