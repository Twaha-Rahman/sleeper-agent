#include "libseed.hpp"

#include <gtest/gtest.h>

TEST(LibSeed, AddPositiveNumbers) { EXPECT_EQ(seed::Add(2, 3), 5); }

TEST(LibSeed, AddWithZero) {
  EXPECT_EQ(seed::Add(0, 0), 0);
  EXPECT_EQ(seed::Add(7, 0), 7);
}

TEST(LibSeed, AddNegativeNumbers) {
  EXPECT_EQ(seed::Add(-2, -3), -5);
  EXPECT_EQ(seed::Add(-2, 3), 1);
}
