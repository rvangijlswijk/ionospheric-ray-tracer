#include "gtest/gtest.h"
#include "Vector2f.h"
namespace {
  class FooTest : public ::testing::Test {
  protected:
	  Vector2f f;
  };
  TEST_F(FooTest, Foo) {
    ASSERT_TRUE(f.x);
  }
}
