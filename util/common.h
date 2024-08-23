#ifndef UTIL_COMMON_H_
#define UTIL_COMMON_H_
enum class retcode {
  SUCCESS = 0,
  FAIL,
};
#define RETCODE_SUCCESS retcode::SUCCESS
#define RETCODE_FAIL    retcode::FAIL
using RETCODE = retcode;
#endif  // UTIL_COMMON_H_
