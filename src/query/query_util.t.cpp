#include <gtest/gtest.h>
#include <query_util.h>

namespace dpt {
namespace query {

TEST(OnlinePopulationVariance, EdgeCase) {
    EXPECT_EQ(0, QueryUtil::newVariance(0, 0, 0, 0, 4));
}

TEST(OnlinePopulationVariance, Regular) {
    // Given:  Variance of [1] is 0
    // Expect: Variance of [1, 2] is 0.25
    EXPECT_EQ(0.25, QueryUtil::newVariance(0, 1, 1, 1, 2));

    // Given:  Variance of [1, 2] is 0.25
    // Expect: Variance of [1, 2, 3] is 0.666667
    EXPECT_NEAR(0.666667, QueryUtil::newVariance(0.25, 1.5, 3, 2, 3), 0.0001);

    // Given:  Variance of [1, 2, 3, 4, 5] is 2
    // Expect: Variance of [1, 2, 3, 4, 5, 6] is 2.91667
    EXPECT_NEAR(2.91667, QueryUtil::newVariance(2, 3, 15, 5, 6), 0.0001);
}

}
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}