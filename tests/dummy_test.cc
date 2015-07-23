#include "gtest/gtest.h"
#include "yahttp/Server.hh"

using namespace yahttp;

TEST(Server, ServesDirectly) {
  EXPECT_EQ(1, 1);
}
