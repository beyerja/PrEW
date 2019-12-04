#include <_test/test.h>
#include "spdlog/spdlog.h"

bool test::test() {
  int a=1;
  int b=0;
  int c = a+b;
  spdlog::info("Wups");
  return c;
}
