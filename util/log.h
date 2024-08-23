#ifndef UTIL_LOG_H_
#define UTIL_LOG_H_
#include <iostream>
#define LOG(INFO) std::cout
#define LOG(ERROR) std::cerr
#define ENDL std::endl
#define BASE_INFO() fmt::format("{} {} {}: ", __FUNCTION__, __FILE__, __LINE__)
#endif  // UTIL_LOG_H_
