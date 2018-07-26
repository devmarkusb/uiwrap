#include "convert_Qt.h"
#include "toolib/math/floating_point.h"
#include "gtest/gtest.h"


using namespace uiw::implQt;

TEST(fundamental_types_convert_Qt, from_qreal)
{
    EXPECT_TRUE(too::math::almost_equal(2.45, from_qreal<double>(qreal{2.45})));
    EXPECT_TRUE(too::math::approx_equal(2.45L, from_qreal<long double>(qreal{2.45}), 1e-15L));
    EXPECT_TRUE(too::math::approx_equal(2.45f, from_qreal<float>(qreal{2.45}), 1e-4f));

    EXPECT_TRUE(too::math::almost_equal(0.0, from_qreal<double>(qreal{0.0})));
    EXPECT_TRUE(too::math::approx_equal(0.0L, from_qreal<long double>(qreal{0.0}), 1e-15L));
    EXPECT_TRUE(too::math::approx_equal(0.0f, from_qreal<float>(qreal{0.0}), 1e-4f));

    EXPECT_TRUE(too::math::almost_equal(-2.45, from_qreal<double>(qreal{-2.45})));
    EXPECT_TRUE(too::math::approx_equal(-2.45L, from_qreal<long double>(qreal{-2.45}), 1e-15L));
    EXPECT_TRUE(too::math::approx_equal(-2.45f, from_qreal<float>(qreal{-2.45}), 1e-4f));
}

TEST(fundamental_types_convert_Qt, to_qreal)
{
    EXPECT_TRUE(too::math::almost_equal(qreal{2.45}, to_qreal(2.45)));
    EXPECT_TRUE(too::math::approx_equal(qreal{2.45}, to_qreal(2.45L), qreal{1e-15}));
    EXPECT_TRUE(too::math::approx_equal(qreal{2.45}, to_qreal(2.45f), qreal{1e-4}));

    EXPECT_TRUE(too::math::almost_equal(qreal{0.0}, to_qreal(0.0)));
    EXPECT_TRUE(too::math::approx_equal(qreal{0.0}, to_qreal(0.0L), qreal{1e-15}));
    EXPECT_TRUE(too::math::approx_equal(qreal{0.0}, to_qreal(0.0f), qreal{1e-4}));

    EXPECT_TRUE(too::math::almost_equal(qreal{-2.45}, to_qreal(-2.45)));
    EXPECT_TRUE(too::math::approx_equal(qreal{-2.45}, to_qreal(-2.45L), qreal{1e-15}));
    EXPECT_TRUE(too::math::approx_equal(qreal{-2.45}, to_qreal(-2.45f), qreal{1e-4}));
}
