// pbrt is Copyright(c) 1998-2020 Matt Pharr, Wenzel Jakob, and Greg Humphreys.
// The pbrt source code is licensed under the Apache License, Version 2.0.
// SPDX: Apache-2.0

#include "../ext/gtest/gtest.h"

//#include <pbrt/pbrt.h>

//#include <pbrt/shapes.h>
//#include <pbrt/util/sampling.h>
#include "../Math/Transform.h"
#include "../Math/Math.h"

#include <cmath>

using namespace Theia;

using RNG = RandomNumberGenerator;

TEST(RNG, Reseed) {
    RNG rng(1234);
    std::vector<uint32_t> values;
    for (int i = 0; i < 100; ++i)
        values.push_back(rng.Uniform<uint32_t>());

    rng.SetSequence(1234);
    for (int i = 0; i < values.size(); ++i)
        EXPECT_EQ(values[i], rng.Uniform<uint32_t>());
}

TEST(RNG, Advance) {
    // Note: must use float not Float, since doubles consume two 32-bit
    // values from the stream, so the advance tests (as written)
    // consequently fail.
    RNG rng;
    rng.SetSequence(1234, 6502);
    std::vector<float> v;
    for (int i = 0; i < 1000; ++i)
        v.push_back(rng.Uniform<float>());

    rng.SetSequence(1234, 6502);
    rng.Advance(16);
    EXPECT_EQ(rng.Uniform<float>(), v[16]);

    for (int i = v.size() - 1; i >= 0; --i) {
        rng.SetSequence(1234, 6502);
        rng.Advance(i);
        EXPECT_EQ(rng.Uniform<float>(), v[i]);
    }

    // Switch to another sequence
    rng.SetSequence(32);
    rng.Uniform<float>();

    // Go back and check one last time
    for (int i : {5, 998, 552, 37, 16}) {
        rng.SetSequence(1234, 6502);
        rng.Advance(i);
        EXPECT_EQ(rng.Uniform<float>(), v[i]);
    }
}

TEST(RNG, OperatorMinus) {
    RNG ra(1337), rb(1337);
    RNG rng;
    for (int i = 0; i < 10; ++i) {
        int step = 1 + rng.Uniform<uint32_t>(1000);
        for (int j = 0; j < step; ++j)
            (void)ra.Uniform<uint32_t>();
        EXPECT_EQ(step, ra - rb);
        EXPECT_EQ(-step, rb - ra);

        // Reysnchronize them
        if ((rng.Uniform<uint32_t>() & 1) != 0u)
            rb.Advance(step);
        else
            ra.Advance(-step);
        EXPECT_EQ(0, ra - rb);
        EXPECT_EQ(0, rb - ra);
    }
}

TEST(RNG, Int) {
    RNG rng;
    int positive = 0, negative = 0, zero = 0;
    int count = 10000;
    for (int i = 0; i < count; ++i) {
        int v = rng.Uniform<int>();
        if (v < 0)
            ++negative;
        else if (v == 0)
            ++zero;
        else
            ++positive;
    }

    EXPECT_GT(positive, .48 * count);
    EXPECT_LT(positive, .52 * count);
    EXPECT_GT(negative, .48 * count);
    EXPECT_LT(negative, .52 * count);
    EXPECT_LT(zero, .001 * count);
}

TEST(RNG, Uint64) {
    RNG rng;
    std::array<int, 64> bitCounts = { 0 };
    int count = 10000;
    for (int i = 0; i < count; ++i) {
        uint64_t v = rng.Uniform<uint64_t>();
        for (int b = 0; b < 64; ++b)
            if ((v & (1ull << b)) != 0u)
                ++bitCounts[b];
    }

    for (int b = 0; b < 64; ++b) {
        EXPECT_GT(bitCounts[b], .48 * count);
        EXPECT_LT(bitCounts[b], .52 * count);
    }
}

TEST(RNG, Double) {
    RNG rng;
    for (int i = 0; i < 10; ++i) {
        double v = rng.Uniform<double>();
        EXPECT_NE(v, float(v));
    }
}

#if 0
TEST(RNG, ImageVis) {
    constexpr int nseeds = 256, ndims = 512;
    //CO    std::array<int, n> histogram = { };
        // badness (if always using default seed
    uint64_t base = 127 * 756023296ull << 8, step = 1;
    // more bad: (LB,S): (16,1), (20, 65536), (0,65536)
//CO    uint64_t base = 1ull << atoi(getenv("LB")), step = atoi(getenv("S"));
    printf("step %d\n", step);
    Image im(PixelFormat::Float, { ndims, nseeds }, { "Y" });
    for (int i = 0; i < nseeds; ++i) {
        RNG rng(base + i * step);
        for (int j = 0; j < ndims; ++j) {
            Float u = rng.Uniform<Float>();
            //CO            ++histogram[std::min<int>(u * n, n - 1)];
            im.SetChannel({ j, i }, 0, u);
        }
    }

    //CO    for (int i = 0; i < n; ++i)
    //CO        printf("%.2f\t", Float(n * histogram[i]) / Float(n * n));
    //CO    printf("\n");

    im.Write("rng.exr");
}
#endif

TEST(Pow2, Basics) {
    for (int i = 0; i < 32; ++i) {
        uint32_t ui = 1u << i;
        EXPECT_EQ(true, IsPowerOf2(ui));
        if (ui > 1) {
            EXPECT_EQ(false, IsPowerOf2(ui + 1));
        }
        if (ui > 2) {
            EXPECT_EQ(false, IsPowerOf2(ui - 1));
        }
    }
}

TEST(RoundUpPow2, Basics) {
    EXPECT_EQ(RoundUpPowerOf2(7), 8);
    for (int i = 1; i < (1 << 24); ++i)
        if (IsPowerOf2(i))
            EXPECT_EQ(RoundUpPowerOf2(i), i);
        /*else
            EXPECT_EQ(RoundUpPowerOf2(i), 1 << (Log2Int(i) + 1));*/

    for (int64_t i = 1; i < (1 << 24); ++i)
        if (IsPowerOf2(i))
            EXPECT_EQ(RoundUpPowerOf2(i), i);
        /*else
            EXPECT_EQ(RoundUpPowerOf2(i), 1 << (Log2Int(i) + 1));*/

    for (int i = 0; i < 30; ++i) {
        int v = 1 << i;
        EXPECT_EQ(RoundUpPowerOf2(v), v);
        if (v > 2) {
            EXPECT_EQ(RoundUpPowerOf2(v - 1), v);
        }
        EXPECT_EQ(RoundUpPowerOf2(v + 1), 2 * v);
    }

    for (int i = 0; i < 62; ++i) {
        int64_t v = 1ll << i;
        EXPECT_EQ(RoundUpPowerOf2(v), v);
        if (v > 2) {
            EXPECT_EQ(RoundUpPowerOf2(v - 1), v);
        }
        EXPECT_EQ(RoundUpPowerOf2(v + 1), 2 * v);
    }
}

//TEST(Morton2, Basics) {
//    uint16_t x = 0b01010111, y = 0b11000101;
//    uint32_t m = EncodeMorton2(x, y);
//    EXPECT_EQ(m, 0b1011000100110111);
//
//#if 0
//    for (int x = 0; x <= 65535; ++x)
//        for (int y = 0; y <= 65535; ++y) {
//            uint32_t m = EncodeMorton2(x, y);
//            uint16_t xp, yp;
//            DecodeMorton2(m, &xp, &yp);
//
//            EXPECT_EQ(x, xp);
//            EXPECT_EQ(y, yp);
//        }
//#endif
//
//    RNG rng(12351);
//    for (int i = 0; i < 100000; ++i) {
//        uint32_t x = rng.Uniform<uint32_t>();
//        uint32_t y = rng.Uniform<uint32_t>();
//        uint64_t m = EncodeMorton2(x, y);
//
//        uint32_t xp, yp;
//        DecodeMorton2(m, &xp, &yp);
//        EXPECT_EQ(x, xp);
//        EXPECT_EQ(y, yp);
//    }
//}
//
//TEST(Math, Pow) {
//    EXPECT_EQ(Pow<0>(2.f), 1 << 0);
//    EXPECT_EQ(Pow<1>(2.f), 1 << 1);
//    EXPECT_EQ(Pow<2>(2.f), 1 << 2);
//    // Test remainder of pow template powers to 29
//    EXPECT_EQ(Pow<3>(2.f), 1 << 3);
//    EXPECT_EQ(Pow<4>(2.f), 1 << 4);
//    EXPECT_EQ(Pow<5>(2.f), 1 << 5);
//    EXPECT_EQ(Pow<6>(2.f), 1 << 6);
//    EXPECT_EQ(Pow<7>(2.f), 1 << 7);
//    EXPECT_EQ(Pow<8>(2.f), 1 << 8);
//    EXPECT_EQ(Pow<9>(2.f), 1 << 9);
//    EXPECT_EQ(Pow<10>(2.f), 1 << 10);
//    EXPECT_EQ(Pow<11>(2.f), 1 << 11);
//    EXPECT_EQ(Pow<12>(2.f), 1 << 12);
//    EXPECT_EQ(Pow<13>(2.f), 1 << 13);
//    EXPECT_EQ(Pow<14>(2.f), 1 << 14);
//    EXPECT_EQ(Pow<15>(2.f), 1 << 15);
//    EXPECT_EQ(Pow<16>(2.f), 1 << 16);
//    EXPECT_EQ(Pow<17>(2.f), 1 << 17);
//    EXPECT_EQ(Pow<18>(2.f), 1 << 18);
//    EXPECT_EQ(Pow<19>(2.f), 1 << 19);
//    EXPECT_EQ(Pow<20>(2.f), 1 << 20);
//    EXPECT_EQ(Pow<21>(2.f), 1 << 21);
//    EXPECT_EQ(Pow<22>(2.f), 1 << 22);
//    EXPECT_EQ(Pow<23>(2.f), 1 << 23);
//    EXPECT_EQ(Pow<24>(2.f), 1 << 24);
//    EXPECT_EQ(Pow<25>(2.f), 1 << 25);
//    EXPECT_EQ(Pow<26>(2.f), 1 << 26);
//    EXPECT_EQ(Pow<27>(2.f), 1 << 27);
//    EXPECT_EQ(Pow<28>(2.f), 1 << 28);
//    EXPECT_EQ(Pow<29>(2.f), 1 << 29);
//}
//
//TEST(Math, NewtonBisection) {
//    EXPECT_FLOAT_EQ(1, NewtonBisection(0, 10, [](Float x) -> std::pair<Float, Float> {
//        return { Float(-1 + x), Float(1) };
//        }));
//    EXPECT_FLOAT_EQ(Pi / 2, NewtonBisection(0, 2, [](Float x) -> std::pair<Float, Float> {
//        return { std::cos(x), -std::sin(x) };
//        }));
//
//    // The derivative is a lie--pointing in the wrong direction, even--but
//    // it should still work.
//    Float bad = NewtonBisection(0, 2, [](Float x) -> std::pair<Float, Float> {
//        return { std::cos(x), 10 * std::sin(x) };
//        });
//    EXPECT_LT(std::abs(Pi / 2 - bad), 1e-5);
//
//    // Multiple zeros in the domain; make sure we find one.
//    Float zero = NewtonBisection(.1, 10.1, [](Float x) -> std::pair<Float, Float> {
//        return { std::sin(x), std::cos(x) };
//        });
//    EXPECT_LT(std::abs(std::sin(zero)), 1e-6);
//
//    // Ill-behaved function with derivatives that go to infinity (and also
//    // multiple zeros).
//    auto f = [](Float x) -> std::pair<Float, Float> {
//        return { std::pow(Sqr(std::sin(x)), .05) - 0.3,
//                0.1 * std::cos(x) * std::sin(x) / std::pow(Sqr(std::sin(x)), 0.95) };
//    };
//    zero = NewtonBisection(.01, 9.42477798, f);
//    // Extra slop for a messy function.
//    EXPECT_LT(std::abs(f(zero).first), 1e-2);
//}
//
//TEST(Math, EvaluatePolynomial) {
//    EXPECT_EQ(4, EvaluatePolynomial(100, 4));
//    EXPECT_EQ(10, EvaluatePolynomial(2, 4, 3));
//
//    EXPECT_EQ(1.5 + 2.75 * .5 - 4.25 * Pow<2>(.5) + 15.125 * Pow<3>(.5),
//        EvaluatePolynomial(.5, 1.5, 2.75, -4.25, 15.125));
//}
//
//TEST(Math, CompensatedSum) {
//    // In order of decreasing accuracy...
//    CompensatedSum<double> kahanSumD;
//    long double ldSum = 0;  // note: is plain old double with MSVC
//    double doubleSum = 0;
//    CompensatedSum<float> kahanSumF;
//    float floatSum = 0;
//
//    RNG rng;
//    for (int i = 0; i < 16 * 1024 * 1024; ++i) {
//        // Hard to sum accurately since the values span many magnitudes.
//        float v = std::exp(Lerp(rng.Uniform<Float>(), -5, 20));
//        ldSum += v;
//        kahanSumD += v;
//        doubleSum += v;
//        kahanSumF += v;
//        floatSum += v;
//    }
//
//    int64_t kahanDBits = FloatToBits(double(kahanSumD));
//    int64_t ldBits = FloatToBits(double(ldSum));
//    int64_t doubleBits = FloatToBits(doubleSum);
//    int64_t kahanFBits = FloatToBits(double(float(kahanSumF)));
//    int64_t floatBits = FloatToBits(double(floatSum));
//
//    int64_t ldErrorUlps = std::abs(ldBits - kahanDBits);
//    int64_t doubleErrorUlps = std::abs(doubleBits - kahanDBits);
//    int64_t kahanFErrorUlps = std::abs(kahanFBits - kahanDBits);
//    int64_t floatErrorUlps = std::abs(floatBits - kahanDBits);
//
//    // Expect each to be much more accurate than the one before it.
//    if (sizeof(long double) > sizeof(double)) {
//        EXPECT_LT(ldErrorUlps * 10000, doubleErrorUlps);
//    }
//    // Less slop between double and Kahan with floats.
//    EXPECT_LT(doubleErrorUlps * 1000, kahanFErrorUlps);
//    EXPECT_LT(kahanFErrorUlps * 10000, floatErrorUlps);
//}
//
//TEST(Log2Int, Basics) {
//    for (int i = 0; i < 32; ++i) {
//        uint32_t ui = 1u << i;
//        EXPECT_EQ(i, Log2Int(ui));
//        EXPECT_EQ(i, Log2Int((uint64_t)ui));
//    }
//
//    for (int i = 1; i < 31; ++i) {
//        uint32_t ui = 1u << i;
//        EXPECT_EQ(i, Log2Int(ui + 1));
//        EXPECT_EQ(i, Log2Int((uint64_t)(ui + 1)));
//    }
//
//    for (int i = 0; i < 64; ++i) {
//        uint64_t ui = 1ull << i;
//        EXPECT_EQ(i, Log2Int(ui));
//    }
//
//    for (int i = 1; i < 64; ++i) {
//        uint64_t ui = 1ull << i;
//        EXPECT_EQ(i, Log2Int(ui + 1));
//    }
//
//    // Exact powers of two
//    for (int i = 120; i >= -120; --i)
//        EXPECT_EQ(i, Log2Int(float(std::pow(2.f, i))));
//    for (int i = 120; i >= -120; --i)
//        EXPECT_EQ(i, Log2Int(double(std::pow(2., i))));
//
//    // Specifically exercise round to nearest even stuff for the Float version
//    for (int i = -31; i < 31; ++i) {
//        float vlow = std::pow(2, i + 0.499f);
//        EXPECT_EQ(i, Log2Int(vlow));
//        float vhigh = std::pow(2, i + 0.501f);
//        EXPECT_EQ(i + 1, Log2Int(vhigh));
//    }
//    for (int i = -31; i < 31; ++i) {
//        double vlow = pow(2., i + 0.4999999);
//        EXPECT_EQ(i, Log2Int(vlow));
//        double vhigh = pow(2., i + 0.5000001);
//        EXPECT_EQ(i + 1, Log2Int(vhigh));
//    }
//}
//
//TEST(Log4Int, Basics) {
//    EXPECT_EQ(0, Log4Int(1));
//    EXPECT_EQ(1, Log4Int(4));
//    EXPECT_EQ(2, Log4Int(16));
//    EXPECT_EQ(3, Log4Int(64));
//
//    int log4 = 0, next = 4;
//    for (int v = 1; v < 16385; ++v) {
//        if (v == next) {
//            ++log4;
//            next *= 4;
//        }
//        EXPECT_EQ(log4, Log4Int(v));
//    }
//}
//
//TEST(Pow4, Basics) {
//    for (int i = 0; i < 12; ++i) {
//        int p4 = 1 << (2 * i);
//        EXPECT_TRUE(IsPowerOf4(p4));
//        EXPECT_EQ(p4, RoundUpPow4(p4));
//        if (i > 0) {
//            EXPECT_EQ(p4, RoundUpPow4(p4 - 1));
//        }
//        EXPECT_EQ(p4 * 4, RoundUpPow4(p4 + 1));
//    }
//}
//
//TEST(NextPrime, Basics) {
//    EXPECT_EQ(3, NextPrime(2));
//    EXPECT_EQ(11, NextPrime(10));
//    EXPECT_EQ(37, NextPrime(32));
//    EXPECT_EQ(37, NextPrime(37));
//
//    auto isPrime = [](int x) {
//        if ((x & 1) == 0)
//            return false;
//        for (int i = 3; i < std::sqrt(x) + 1; i += 3) {
//            if ((x % i) == 0)
//                return false;
//        }
//        return true;
//    };
//
//    for (int n = 3; n < 8000; n += 3) {
//        if (isPrime(n)) {
//            EXPECT_EQ(n, NextPrime(n));
//        }
//        else {
//            int np = NextPrime(n);
//            EXPECT_TRUE(isPrime(np));
//        }
//    }
//}
//
//TEST(Math, ErfInv) {
//    float xvl[] = { 0., 0.1125, 0.25, .753, 1.521, 2.5115 };
//    for (float x : xvl) {
//        Float e = std::erf(x);
//        if (e < 1) {
//            Float ei = ErfInv(e);
//            if (x == 0)
//                EXPECT_EQ(0, ei);
//            else {
//                Float err = std::abs(ei - x) / x;
//                EXPECT_LT(err, 1e-4) << x << " erf " << e << " inv " << ei;
//            }
//        }
//    }
//}
//
//#ifndef PBRT_FLOAT_AS_DOUBLE
//// The next two expect a higher-precision option to verify with.
//TEST(Math, DifferenceOfProducts) {
//    for (int i = 0; i < 100000; ++i) {
//        RNG rng(i);
//        auto r = [&rng]() {
//            Float logu = Lerp(rng.Uniform<Float>(), -8, 8);
//            return std::pow(10, logu);
//        };
//        Float a = r(), b = r(), c = r(), d = r();
//        Float sign = rng.Uniform<Float>() < -0.5 ? -1 : 1;
//        b *= sign;
//        c *= sign;
//        Float dp = DifferenceOfProducts(a, b, c, d);
//        Float dp2 = FMA(double(a), double(b), -double(c) * double(d));
//        Float err = std::abs(dp - dp2);
//        Float ulp = NextFloatUp(std::abs(dp2)) - std::abs(dp2);
//        EXPECT_LT(err, 2 * ulp);
//    }
//}
//
//TEST(Math, SumOfProducts) {
//    for (int i = 0; i < 100000; ++i) {
//        RNG rng(i);
//        auto r = [&rng]() {
//            Float logu = Lerp(rng.Uniform<Float>(), -8, 8);
//            return std::pow(10, logu);
//        };
//        // Make sure mixed signs...
//        Float a = r(), b = r(), c = r(), d = -r();
//        Float sign = rng.Uniform<Float>() < -0.5 ? -1 : 1;
//        b *= sign;
//        c *= sign;
//        Float sp = SumOfProducts(a, b, c, d);
//        Float sp2 = FMA(double(a), double(b), double(c) * double(d));
//        Float err = std::abs(sp - sp2);
//        Float ulp = NextFloatUp(std::abs(sp2)) - std::abs(sp2);
//        EXPECT_LT(err, 2 * ulp);
//    }
//}
//#endif  // !PBRT_FLOAT_AS_DOUBLE
//
//TEST(FastExp, Accuracy) {
//    EXPECT_EQ(1, FastExp(0));
//
//    Float maxErr = 0;
//    RNG rng(6502);
//    for (int i = 0; i < 100; ++i) {
//        Float v = Lerp(rng.Uniform<Float>(), -20.f, 20.f);
//        Float f = FastExp(v);
//        Float e = std::exp(v);
//        Float err = std::abs((f - e) / e);
//        maxErr = std::max(err, maxErr);
//        EXPECT_LE(err, 0.0003f) << "At " << v << ", fast = " << f << ", accurate = " << e
//            << " -> relative error = " << err;
//    }
//#if 0
//    fprintf(stderr, "max error %f\n", maxErr);
//
//    // performance
//    Float sum = 0;
//    std::chrono::steady_clock::time_point start =
//        std::chrono::steady_clock::now();
//    for (int i = 0; i < 10000000; ++i) {
//        Float v = Lerp(rng.Uniform<Float>(), -20.f, 20.f);
//        sum += std::exp(v);
//    }
//    std::chrono::steady_clock::time_point now =
//        std::chrono::steady_clock::now();
//    Float elapsedMS =
//        std::chrono::duration_cast<std::chrono::microseconds>(now - start)
//        .count() / 1000.;
//    fprintf(stderr, "%.3f ms\n", (Float)elapsedMS);
//
//    EXPECT_NE(sum, 0); // use it
//#endif
//}
//
//TEST(Math, GaussianIntegral) {
//    Float muSigma[][2] = { {0, 1}, {0, 2}, {0, .1}, {1, 2}, {-2, 1} };
//    for (int i = 0; i < sizeof(muSigma) / sizeof(muSigma[0]); ++i) {
//        RNG rng;
//        for (int j = 0; j < 5; ++j) {
//            Float x0 = -5 + 10 + rng.Uniform<Float>();
//            Float x1 = -5 + 10 + rng.Uniform<Float>();
//            if (x0 > x1)
//                pstd::swap(x0, x1);
//
//            Float mu = muSigma[i][0], sigma = muSigma[i][1];
//            Float sum = 0;
//            int n = 8192;
//            for (int k = 0; k < n; ++k) {
//                Float u = (k + rng.Uniform<Float>()) / n;
//                Float x = Lerp(u, x0, x1);
//                sum += Gaussian(x, mu, sigma);
//            }
//            Float est = (x1 - x0) * sum / n;
//
//            auto compareFloats = [](Float ref, Float v) {
//                if (std::abs(ref) < 1e-4)
//                    return std::abs(ref - v) < 1e-5;
//                return std::abs((ref - v) / ref) < 1e-3;
//            };
//            Float in = GaussianIntegral(x0, x1, mu, sigma);
//            EXPECT_TRUE(compareFloats(est, in)) << est << " vs " << in;
//        }
//    }
//}
//
template <int N> using Matrix = SquareMatrix<Float, N>;

TEST(SquareMatrix, Basics2) {
    Matrix<2> m2;

    EXPECT_TRUE(m2.IsIdentity());

    EXPECT_EQ(m2, Matrix<2>(1, 0, 0, 1));
    EXPECT_NE(m2, Matrix<2>(0, 1, 1, 0));

    EXPECT_EQ(Matrix<2>(2, 0, 0, -1), Matrix<2>::Diagonal(2, -1));

    Matrix<2> m(1, 2, 3, 4);
    EXPECT_FALSE(m.IsIdentity());
    Matrix<2> mt(1, 3, 2, 4);
    EXPECT_EQ(Transpose(m), mt);

    std::array<Float, 2> v{1, -2};
    std::array<Float, 2> vt = m * v;
    EXPECT_EQ(1 - 2 * 2, vt[0]);
    EXPECT_EQ(3 - 4 * 2, vt[1]);

    Vector2f v2(1, -2);
    Vector2f v2t = m * v2;
    EXPECT_EQ(1 - 2 * 2, v2t[0]);
    EXPECT_EQ(3 - 4 * 2, v2t[1]);

    /*pstd::optional<SquareMatrix<2>> inv = Inverse(m2);
    EXPECT_TRUE(inv.has_value());
    EXPECT_EQ(m2, *inv);

    SquareMatrix<2> ms(2, 4, -4, 8);
    inv = Inverse(ms);
    EXPECT_TRUE(inv.has_value());
    EXPECT_EQ(SquareMatrix<2>(1. / 4., -1. / 8., 1. / 8., 1. / 16.), *inv);

    SquareMatrix<2> degen(0, 0, 2, 0);
    inv = Inverse(degen);
    EXPECT_FALSE(inv.has_value());*/
}

TEST(SquareMatrix, Basics3) {
    Matrix<3> m3;
    EXPECT_TRUE(m3.IsIdentity());

    EXPECT_EQ(m3, Matrix<3>(1, 0, 0, 0, 1, 0, 0, 0, 1));
    EXPECT_NE(m3, Matrix<3>(0, 1, 0, 0, 1, 0, 0, 0, 1));

    EXPECT_EQ(Matrix<3>(2, 0, 0, 0, -1, 0, 0, 0, 3), Matrix<3>::Diagonal(2, -1, 3));

    Matrix<3> m(1, 2, 3, 4, 5, 6, 7, 8, 9);
    EXPECT_FALSE(m.IsIdentity());
    Matrix<3> mt(1, 4, 7, 2, 5, 8, 3, 6, 9);
    EXPECT_EQ(Transpose(m), mt);

    std::array<Float, 3> v{1, -2, 4};
    std::array<Float, 3> vt = m * v;
    EXPECT_EQ(1 - 4 + 12, vt[0]);
    EXPECT_EQ(4 - 10 + 24, vt[1]);
    EXPECT_EQ(7 - 16 + 36, vt[2]);

    Vector3f v3(1, -2, 4);
    Vector3f v3t = m * v3;
    EXPECT_EQ(1 - 4 + 12, v3t[0]);
    EXPECT_EQ(4 - 10 + 24, v3t[1]);
    EXPECT_EQ(7 - 16 + 36, v3t[2]);

    /*pstd::optional<SquareMatrix<3>> inv = Inverse(m3);
    EXPECT_TRUE(inv.has_value());
    EXPECT_EQ(m3, *inv);

    SquareMatrix<3> ms(2, 0, 0, 0, 4, 0, 0, 0, -1);
    inv = Inverse(ms);
    EXPECT_TRUE(inv.has_value());
    EXPECT_EQ(SquareMatrix<3>(0.5, 0, 0, 0, .25, 0, 0, 0, -1), *inv);

    SquareMatrix<3> degen(0, 0, 2, 0, 0, 0, 1, 1, 1);
    inv = Inverse(degen);
    EXPECT_FALSE(inv.has_value());*/
}


TEST(SquareMatrix, Basics4) {
    Matrix<4> m4;
    EXPECT_TRUE(m4.IsIdentity());
    EXPECT_EQ(m4, Matrix<4>(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
    EXPECT_NE(m4, Matrix<4>(0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0));

    Matrix<4> diag(8, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, .5);
    EXPECT_EQ(diag, Matrix<4>::Diagonal(8, 2, 1, .5));

    Matrix<4> m(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    EXPECT_FALSE(m.IsIdentity());
    Matrix<4> mt(1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16);
    EXPECT_EQ(Transpose(m), mt);

    /*pstd::optional<SquareMatrix<4>> inv = Inverse(m4);
    EXPECT_TRUE(inv.has_value());
    EXPECT_EQ(m4, *inv);

    inv = Inverse(diag);
    EXPECT_TRUE(inv.has_value());
    EXPECT_EQ(SquareMatrix<4>::Diag(.125, .5, 1, 2), *inv);

    SquareMatrix<4> degen(2, 0, 0, 0, 0, 4, 0, 0, 0, -3, 0, 1, 0, 0, 0, 0);
    inv = Inverse(degen);
    EXPECT_FALSE(inv.has_value());*/
}

template <int N>
static Matrix<N> randomMatrix(RNG& rng) {
    Matrix<N> m;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            m[i][j] = -10 + 20 * rng.Uniform<Float>();
    return m;
}

//TEST(SquareMatrix, Inverse) {
//    auto equal = [](Float a, Float b, Float tol = 1e-4) {
//        if (std::abs(a) < 1e-5 || std::abs(b) < 1e-5)
//            return std::abs(a) - std::abs(b) < tol;
//        return (std::abs(a) - std::abs(b)) / ((std::abs(a) + std::abs(b)) / 2) < tol;
//    };
//
//    int nFail = 0;
//    int nIters = 1000;
//    {
//        constexpr int N = 2;
//        for (int i = 0; i < nIters; ++i) {
//            RNG rng(i);
//            SquareMatrix<N> m = randomMatrix<N>(rng);
//            pstd::optional<SquareMatrix<N>> inv = Inverse(m);
//            if (!inv) {
//                ++nFail;
//                continue;
//            }
//            SquareMatrix<N> id = m * *inv;
//
//            for (int j = 0; j < N; ++j)
//                for (int k = 0; k < N; ++k) {
//                    if (j == k)
//                        EXPECT_TRUE(equal(id[j][k], 1))
//                        << m << ", inv " << *inv << " prod " << id;
//                    else
//                        EXPECT_LT(std::abs(id[j][k]), 1e-4)
//                        << m << ", inv " << *inv << " prod " << id;
//                }
//        }
//    }
//    {
//        constexpr int N = 3;
//        for (int i = 0; i < nIters; ++i) {
//            RNG rng(i);
//            SquareMatrix<N> m = randomMatrix<N>(rng);
//            pstd::optional<SquareMatrix<N>> inv = Inverse(m);
//            if (!inv) {
//                ++nFail;
//                continue;
//            }
//            SquareMatrix<N> id = m * *inv;
//
//            for (int j = 0; j < N; ++j)
//                for (int k = 0; k < N; ++k) {
//                    if (j == k)
//                        EXPECT_TRUE(equal(id[j][k], 1))
//                        << m << ", inv " << *inv << " prod " << id;
//                    else
//                        EXPECT_LT(std::abs(id[j][k]), 1e-4)
//                        << m << ", inv " << *inv << " prod " << id;
//                }
//        }
//    }
//    {
//        constexpr int N = 4;
//        for (int i = 0; i < nIters; ++i) {
//            RNG rng(i);
//            SquareMatrix<N> m = randomMatrix<N>(rng);
//            pstd::optional<SquareMatrix<N>> inv = Inverse(m);
//            if (!inv) {
//                ++nFail;
//                continue;
//            }
//            SquareMatrix<N> id = m * *inv;
//
//            for (int j = 0; j < N; ++j)
//                for (int k = 0; k < N; ++k) {
//                    if (j == k)
//                        EXPECT_TRUE(equal(id[j][k], 1))
//                        << m << ", inv " << *inv << " prod " << id;
//                    else
//                        EXPECT_LT(std::abs(id[j][k]), 1e-4)
//                        << m << ", inv " << *inv << " prod " << id;
//                }
//        }
//    }
//
//    EXPECT_LT(nFail, 3);
//}
//
//TEST(FindInterval, Basics) {
//    std::vector<float> a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//
//    // Check clamping for out of range
//    EXPECT_EQ(0, FindInterval(a.size(), [&](int index) { return a[index] <= -1; }));
//    EXPECT_EQ(a.size() - 2,
//        FindInterval(a.size(), [&](int index) { return a[index] <= 100; }));
//
//    for (size_t i = 0; i < a.size() - 1; ++i) {
//        EXPECT_EQ(i, FindInterval(a.size(), [&](int index) { return a[index] <= i; }));
//        EXPECT_EQ(i,
//            FindInterval(a.size(), [&](int index) { return a[index] <= i + 0.5; }));
//        if (i > 0)
//            EXPECT_EQ(i - 1, FindInterval(a.size(), [&](int index) {
//            return a[index] <= i - 0.5;
//                }));
//    }
//}
//
///////////////////////////////////////////////////////////////////////////
// Interval tests

// Return an exponentially-distributed floating-point value.
static Interval getFloat(RNG& rng, Float minExp = -6., Float maxExp = 6.) {
    Float logu = Lerp(rng.Uniform<Float>(), minExp, maxExp);
    Float val = std::pow(10, logu);

    // Choose a random error bound.
    Float err = 0;
    switch (rng.Uniform<uint32_t>(4)) {
    case 0:
        // no error
        break;
    case 1: {
        // small typical/reasonable error
        uint32_t ulpError = rng.Uniform<uint32_t>(1024);
        Float offset = FloatBitsToFloat(FloatToFloatBits(val) + ulpError);
        err = std::abs(offset - val);
        break;
    }
    case 2: {
        // bigger ~reasonable error
        uint32_t ulpError = rng.Uniform<uint32_t>(1024 * 1024);
        Float offset = FloatBitsToFloat(FloatToFloatBits(val) + ulpError);
        err = std::abs(offset - val);
        break;
    }
    case 3: {
        err = (4 * rng.Uniform<Float>()) * std::abs(val);
    }
    }
    Float sign = rng.Uniform<Float>() < .5 ? -1. : 1.;
    return Interval::FromValueAndError(sign * val, err);
}

// Given an Interval covering some range, choose a double-precision
// "precise" value that is in the Interval's range.
static double getPrecise(const Interval& ef, RNG& rng) {
    switch (rng.Uniform<uint32_t>(3)) {
        // 2/3 of the time, pick a value that is right at the end of the range;
        // this is a maximally difficult / adversarial choice, so should help
        // ferret out any bugs.
    case 0:
        return ef.GetLow();
    case 1:
        return ef.GetHigh();
    case 2: {
        // Otherwise choose a value uniformly inside the Interval's range.
        Float t = rng.Uniform<Float>();
        double p = (1 - t) * ef.GetLow() + t * ef.GetHigh();
        if (p > ef.GetHigh())
            p = ef.GetHigh();
        if (p < ef.GetLow())
            p = ef.GetLow();
        return p;
    }
    }
    return (Float)ef;  // NOTREACHED
}

static const int kFloatIntervalIters = 1000000;

//TEST(FloatInterval, Abs) {
//    for (int trial = 0; trial < kFloatIntervalIters; ++trial) {
//        RNG rng(trial);
//
//        Interval ef = getFloat(rng);
//        double precise = getPrecise(ef, rng);
//
//        Interval efResult = Abs(ef);
//        double preciseResult = std::abs(precise);
//
//        EXPECT_GE(preciseResult, efResult.GetLow());
//        EXPECT_LE(preciseResult, efResult.GetHigh());
//    }
//}

//TEST(FloatInterval, Sqrt) {
//    for (int trial = 0; trial < kFloatIntervalIters; ++trial) {
//        RNG rng(trial);
//
//        Interval ef = getFloat(rng);
//        double precise = getPrecise(ef, rng);
//
//        Interval efResult = Sqrt(Abs(ef));
//        double preciseResult = std::sqrt(std::abs(precise));
//
//        EXPECT_GE(preciseResult, efResult.LowerBound());
//        EXPECT_LE(preciseResult, efResult.UpperBound());
//    }
//}

TEST(FloatInterval, Add) {
    for (int trial = 0; trial < kFloatIntervalIters; ++trial) {
        RNG rng(trial);

        Interval ef[2] = { getFloat(rng), getFloat(rng) };
        double precise[2] = { getPrecise(ef[0], rng), getPrecise(ef[1], rng) };

        Interval efResult = ef[0] + ef[1];
        float preciseResult = precise[0] + precise[1];

        EXPECT_GE(preciseResult, efResult.GetLow());
        EXPECT_LE(preciseResult, efResult.GetHigh());
    }
}

TEST(FloatInterval, Sub) {
    for (int trial = 0; trial < kFloatIntervalIters; ++trial) {
        RNG rng(trial);

        Interval ef[2] = { getFloat(rng), getFloat(rng) };
        double precise[2] = { getPrecise(ef[0], rng), getPrecise(ef[1], rng) };

        Interval efResult = ef[0] - ef[1];
        float preciseResult = precise[0] - precise[1];

        EXPECT_GE(preciseResult, efResult.GetLow());
        EXPECT_LE(preciseResult, efResult.GetHigh());
    }
}

TEST(FloatInterval, Mul) {
    for (int trial = 0; trial < kFloatIntervalIters; ++trial) {
        RNG rng(trial);

        Interval ef[2] = { getFloat(rng), getFloat(rng) };
        double precise[2] = { getPrecise(ef[0], rng), getPrecise(ef[1], rng) };

        Interval efResult = ef[0] * ef[1];
        float preciseResult = precise[0] * precise[1];

        EXPECT_GE(preciseResult, efResult.GetLow());
        EXPECT_LE(preciseResult, efResult.GetHigh());
    }
}

TEST(FloatInterval, Div) {
    for (int trial = 0; trial < kFloatIntervalIters; ++trial) {
        RNG rng(trial);

        Interval ef[2] = { getFloat(rng), getFloat(rng) };
        double precise[2] = { getPrecise(ef[0], rng), getPrecise(ef[1], rng) };

        // Things get messy if the denominator's interval straddles zero...
        if (ef[1].GetLow() * ef[1].GetHigh() < 0.)
            continue;

        Interval efResult = ef[0] / ef[1];
        float preciseResult = precise[0] / precise[1];

        EXPECT_GE(preciseResult, efResult.GetLow());
        EXPECT_LE(preciseResult, efResult.GetHigh());
    }
}

//TEST(FloatInterval, FMA) {
//    int nTrials = 10000, nIters = 400;
//    int ratioCount = 0;
//    int nBetter = 0;
//    for (int i = 0; i < nTrials; ++i) {
//        RNG rng(i);
//        Interval v = Abs(getFloat(rng));
//        for (int j = 0; j < nIters; ++j) {
//            Interval a = v;
//            Interval b = getFloat(rng);
//            Interval c = getFloat(rng);
//
//            v = FMA(a, b, c);
//
//            if (std::isinf(v.LowerBound()) || std::isinf(v.UpperBound()))
//                break;
//
//            double pa = getPrecise(a, rng);
//            double pb = getPrecise(b, rng);
//            double pc = getPrecise(c, rng);
//            float preciseResult = FMA(pa, pb, pc);
//            EXPECT_GE(preciseResult, v.LowerBound()) << v;
//            EXPECT_LE(preciseResult, v.UpperBound()) << v;
//
//            Interval vp = a * b + c;
//            EXPECT_GE(v.LowerBound(), vp.LowerBound()) << v << " vs " << vp;
//            EXPECT_LE(v.UpperBound(), vp.UpperBound()) << v << " vs " << vp;
//
//            nBetter +=
//                (v.LowerBound() > vp.LowerBound() || v.UpperBound() < vp.UpperBound());
//        }
//    }
//
//    EXPECT_GT(nBetter, .85 * ratioCount);
//}
//
//TEST(FloatInterval, Sqr) {
//    Interval a = Interval(1.75, 2.25);
//    Interval as = Sqr(a), at = a * a;
//    EXPECT_EQ(as.UpperBound(), at.UpperBound());
//    EXPECT_EQ(as.LowerBound(), at.LowerBound());
//
//    // Straddle 0
//    Interval b = Interval(-.75, 1.25);
//    Interval bs = Sqr(b), b2 = b * b;
//    EXPECT_EQ(bs.UpperBound(), b2.UpperBound());
//    EXPECT_EQ(0, bs.LowerBound());
//    EXPECT_LT(b2.LowerBound(), 0);
//}
//
//TEST(FloatInterval, SumSquares) {
//    {
//        Interval a(1), b(2), c(3);
//        EXPECT_EQ(1, Float(SumSquares(a)));
//        EXPECT_EQ(4, Float(SumSquares(b)));
//        EXPECT_EQ(5, Float(SumSquares(a, b)));
//        EXPECT_EQ(14, Float(SumSquares(a, b, c)));
//    }
//}
//
//TEST(FloatInterval, DifferenceOfProducts) {
//    for (int trial = 0; trial < kFloatIntervalIters; ++trial) {
//        RNG rng(trial);
//
//        Interval a = Abs(getFloat(rng));
//        Interval b = Abs(getFloat(rng));
//        Interval c = Abs(getFloat(rng));
//        Interval d = Abs(getFloat(rng));
//
//        Float sign = rng.Uniform<Float>() < -0.5 ? -1 : 1;
//        b *= sign;
//        c *= sign;
//
//        double pa = getPrecise(a, rng);
//        double pb = getPrecise(b, rng);
//        double pc = getPrecise(c, rng);
//        double pd = getPrecise(d, rng);
//
//        Interval r = DifferenceOfProducts(a, b, c, d);
//        double pr = DifferenceOfProducts(pa, pb, pc, pd);
//
//        EXPECT_GE(pr, r.LowerBound()) << trial;
//        EXPECT_LE(pr, r.UpperBound()) << trial;
//    }
//}
//
//TEST(FloatInterval, SumOfProducts) {
//    for (int trial = 0; trial < kFloatIntervalIters; ++trial) {
//        RNG rng(trial);
//
//        // Make sure signs are mixed
//        Interval a = Abs(getFloat(rng));
//        Interval b = Abs(getFloat(rng));
//        Interval c = Abs(getFloat(rng));
//        Interval d = -Abs(getFloat(rng));
//
//        Float sign = rng.Uniform<Float>() < -0.5 ? -1 : 1;
//        b *= sign;
//        c *= sign;
//
//        double pa = getPrecise(a, rng);
//        double pb = getPrecise(b, rng);
//        double pc = getPrecise(c, rng);
//        double pd = getPrecise(d, rng);
//
//        Interval r = SumOfProducts(a, b, c, d);
//        double pr = SumOfProducts(pa, pb, pc, pd);
//
//        EXPECT_GE(pr, r.LowerBound()) << trial;
//        EXPECT_LE(pr, r.UpperBound()) << trial;
//    }
//}
//
//TEST(Math, TwoProd) {
//    for (int i = 0; i < 100000; ++i) {
//        RNG rng(i);
//        auto r = [&rng](int minExp = -10, int maxExp = 10) {
//            Float logu = Lerp(rng.Uniform<Float>(), minExp, maxExp);
//            Float val = std::pow(10, logu);
//            Float sign = rng.Uniform<Float>() < .5 ? -1. : 1.;
//            return val * sign;
//        };
//
//        Float a = r(), b = r();
//        CompensatedFloat tp = TwoProd(a, b);
//        EXPECT_EQ((Float)tp, a * b);
//        EXPECT_EQ((double)tp, (double)a * (double)b);
//    }
//}
//
//TEST(Math, TwoSum) {
//    for (int i = 0; i < 100000; ++i) {
//        RNG rng(i);
//        auto r = [&rng](int minExp = -10, int maxExp = 10) {
//            Float logu = Lerp(rng.Uniform<Float>(), minExp, maxExp);
//            Float val = std::pow(10, logu);
//            Float sign = rng.Uniform<Float>() < .5 ? -1. : 1.;
//            return val * sign;
//        };
//
//        Float a = r(), b = r();
//        CompensatedFloat tp = TwoSum(a, b);
//        EXPECT_EQ((Float)tp, a + b);
//        EXPECT_EQ((double)tp, (double)a + (double)b);
//    }
//}
//
//// This depends on having a higher precision option to compare to.
//#ifndef PBRT_FLOAT_AS_DOUBLE
//TEST(Math, InnerProduct) {
//    for (int i = 0; i < 100000; ++i) {
//        RNG rng(i);
//        auto r = [&rng](int minExp = -10, int maxExp = 10) {
//            Float logu = Lerp(rng.Uniform<Float>(), minExp, maxExp);
//            Float val = std::pow(10, logu);
//            Float sign = rng.Uniform<Float>() < .5 ? -1. : 1.;
//            return val * sign;
//        };
//
//        Float a[4] = { r(), r(), r(), r() };
//        Float b[4] = { r(), r(), r(), r() };
//        Float ab = (Float)InnerProduct(a[0], b[0], a[1], b[1], a[2], b[2], a[3], b[3]);
//        Float dab = double(a[0]) * double(b[0]) + double(a[1]) * double(b[1]) +
//            double(a[2]) * double(b[2]) + double(a[3]) * double(b[3]);
//        EXPECT_EQ(ab, dab);
//    }
//}
//#endif  // !PBRT_FLOAT_AS_DOUBLE
//
//// Make sure that the permute function is in fact a valid permutation.
//TEST(PermutationElement, Valid) {
//    for (int len = 2; len < 1024; ++len) {
//        for (int iter = 0; iter < 10; ++iter) {
//            std::vector<bool> seen(len, false);
//
//            for (int i = 0; i < len; ++i) {
//                int offset = PermutationElement(i, len, MixBits(1 + iter));
//                ASSERT_TRUE(offset >= 0 && offset < seen.size()) << offset;
//                EXPECT_FALSE(seen[offset]) << StringPrintf("len %d index %d", len, i);
//                seen[offset] = true;
//            }
//        }
//    }
//}
//
//TEST(PermutationElement, Uniform) {
//    for (int n : {2, 3, 4, 5, 9, 14, 16, 22, 27, 36}) {
//        std::vector<int> count(n * n);
//
//        int numIters = 60000 * n;
//        for (int seed = 0; seed < numIters; ++seed) {
//            for (int i = 0; i < n; ++i) {
//                int ip = PermutationElement(i, n, MixBits(seed));
//                int offset = ip * n + i;
//                ++count[offset];
//            }
//        }
//
//        for (int i = 0; i < n; ++i) {
//            for (int j = 0; j < n; ++j) {
//                Float tol = 0.03f;
//                int offset = j * n + i;
//                EXPECT_TRUE(count[offset] >= (1 - tol) * numIters / n &&
//                    count[offset] <= (1 + tol) * numIters / n)
//                    << StringPrintf(
//                        "Got count %d for %d -> %d (perm size %d). Expected +/- %d.\n",
//                        count[offset], i, j, n, numIters / n);
//            }
//        }
//    }
//}
//
//TEST(PermutationElement, UniformDelta) {
//    for (int n : {2, 3, 4, 5, 9, 14, 16, 22, 27, 36}) {
//        std::vector<int> count(n * n);
//
//        int numIters = 60000 * n;
//        for (int seed = 0; seed < numIters; ++seed) {
//            for (int i = 0; i < n; ++i) {
//                int ip = PermutationElement(i, n, MixBits(seed));
//                int delta = ip - i;
//                if (delta < 0)
//                    delta += n;
//                CHECK_LT(delta, n);
//                int offset = delta * n + i;
//                ++count[offset];
//            }
//        }
//
//        for (int i = 0; i < n; ++i) {
//            for (int j = 0; j < n; ++j) {
//                Float tol = 0.03f;
//                int offset = j * n + i;
//                EXPECT_TRUE(count[offset] >= (1 - tol) * numIters / n &&
//                    count[offset] <= (1 + tol) * numIters / n)
//                    << StringPrintf(
//                        "Got count %d for %d -> %d (perm size %d). Expected +/- %d.\n",
//                        count[offset], i, j, n, numIters / n);
//            }
//        }
//    }
//}

TEST(Vector2, Basics) {
    Vector2f vf(-1, 10);
    EXPECT_EQ(vf, Vector2f(Vector2i(-1, 10)));
    EXPECT_NE(vf, Vector2f(-1, 100));
    EXPECT_EQ(Vector2f(-2, 20), vf + vf);
    EXPECT_EQ(Vector2f(0, 0), vf - vf);
    EXPECT_EQ(Vector2f(-2, 20), vf * 2);
    EXPECT_EQ(Vector2f(-2, 20), 2 * vf);
    EXPECT_EQ(Vector2f(-0.5, 5), vf / 2);
    EXPECT_EQ(Vector2f(1, 10), Abs(vf));
    EXPECT_EQ(vf, Ceil(Vector2f(-1.5, 9.9)));
    EXPECT_EQ(vf, Floor(Vector2f(-.5, 10.01)));
    EXPECT_EQ(Vector2f(-20, 10), Min(vf, Vector2f(-20, 20)));
    EXPECT_EQ(Vector2f(-1, 20), Max(vf, Vector2f(-20, 20)));
    EXPECT_EQ(-1, MinComponentValue(vf));
    EXPECT_EQ(-10, MinComponentValue(-vf));
    EXPECT_EQ(10, MaxComponentValue(vf));
    EXPECT_EQ(1, MaxComponentValue(-vf));
    EXPECT_EQ(1, MaxComponentIndex(vf));
    EXPECT_EQ(0, MaxComponentIndex(-vf));
    EXPECT_EQ(0, MinComponentIndex(vf));
    EXPECT_EQ(1, MinComponentIndex(-vf));
    EXPECT_EQ(vf, Permute(vf, {0, 1}));
    EXPECT_EQ(Vector2f(10, -1), Permute(vf, {1, 0}));
    EXPECT_EQ(Vector2f(10, 10), Permute(vf, {1, 1}));
}

TEST(Vector3, Basics) {
    Vector3f vf(-1, 10, 2);
    EXPECT_EQ(vf, Vector3f(Vector3i(-1, 10, 2)));
    EXPECT_NE(vf, Vector3f(-1, 100, 2));
    EXPECT_EQ(Vector3f(-2, 20, 4), vf + vf);
    EXPECT_EQ(Vector3f(0, 0, 0), vf - vf);
    EXPECT_EQ(Vector3f(-2, 20, 4), vf * 2);
    EXPECT_EQ(Vector3f(-2, 20, 4), 2 * vf);
    EXPECT_EQ(Vector3f(-0.5, 5, 1), vf / 2);
    EXPECT_EQ(Vector3f(1, 10, 2), Abs(vf));
    EXPECT_EQ(vf, Ceil(Vector3f(-1.5, 9.9, 1.01)));
    EXPECT_EQ(vf, Floor(Vector3f(-.5, 10.01, 2.99)));
    EXPECT_EQ(Vector3f(-20, 10, 1.5), Min(vf, Vector3f(-20, 20, 1.5)));
    EXPECT_EQ(Vector3f(-1, 20, 2), Max(vf, Vector3f(-20, 20, 0)));
    EXPECT_EQ(-1, MinComponentValue(vf));
    EXPECT_EQ(-10, MinComponentValue(-vf));
    EXPECT_EQ(10, MaxComponentValue(vf));
    EXPECT_EQ(1, MaxComponentValue(-vf));
    EXPECT_EQ(1, MaxComponentIndex(vf));
    EXPECT_EQ(0, MaxComponentIndex(-vf));
    EXPECT_EQ(0, MinComponentIndex(vf));
    EXPECT_EQ(1, MinComponentIndex(-vf));
    EXPECT_EQ(vf, Permute(vf, {0, 1, 2}));
    EXPECT_EQ(Vector3f(10, -1, 2), Permute(vf, {1, 0, 2}));
    EXPECT_EQ(Vector3f(2, -1, 10), Permute(vf, {2, 0, 1}));
    EXPECT_EQ(Vector3f(10, 10, -1), Permute(vf, {1, 1, 0}));
}

//TEST(Point2, InvertBilinear) {
//    auto bilerp = [](Point2f u, const Point2f *v) {
//        return Lerp(u[0], Lerp(u[1], v[0], v[2]), Lerp(u[1], v[1], v[3]));
//    };
//
//    RNG rng;
//    Point2f vRealSimple[4] = {Point2f(0, 0), Point2f(1, 0), Point2f(0, 1), Point2f(1, 1)};
//    for (int i = 0; i < 100; ++i) {
//        Point2f u{rng.Uniform<Float>(), rng.Uniform<Float>()};
//        Point2f up = bilerp(u, vRealSimple);
//        CHECK_LT(Distance(u, up), 1e-5f);  // check bilerp
//        CHECK_LT(Distance(u, InvertBilinear(up, vRealSimple)), 1e-5f);
//    }
//
//    Point2f vSimple[4] = {Point2f(10, -4), Point2f(10, -9), Point2f(15, -4),
//                          Point2f(15, -9)};
//    for (int i = 0; i < 100; ++i) {
//        Point2f u{rng.Uniform<Float>(), rng.Uniform<Float>()};
//        Point2f up = bilerp(u, vSimple);
//        Point2f uinv = InvertBilinear(up, vSimple);
//        CHECK_LT(Distance(u, uinv), 1e-5f);
//    }
//
//    for (int i = 0; i < 100; ++i) {
//        for (int j = 0; j < 100; ++j) {
//            Point2f u{rng.Uniform<Float>(), rng.Uniform<Float>()};
//            Point2f up = bilerp(u, vSimple);
//            Point2f uinv = InvertBilinear(up, vSimple);
//            CHECK_LT(Distance(u, uinv), 1e-5f);
//        }
//    }
//}
//
//TEST(Vector, AngleBetween) {
//    EXPECT_EQ(0, AngleBetween(Vector3f(1, 0, 0), Vector3f(1, 0, 0)));
//
//    EXPECT_LT(std::abs(AngleBetween(Vector3f(0, 0, 1), Vector3f(0, 0, -1))) - Pi, 1e-7);
//    EXPECT_LT(std::abs(AngleBetween(Vector3f(1, 0, 0), Vector3f(0, 1, 0))) - Pi / 2,
//              1e-7);
//
//    Vector3f x = Normalize(Vector3f(1, -3, 10));
//    EXPECT_EQ(0, AngleBetween(x, x));
//    EXPECT_LT(std::abs(AngleBetween(x, -x) - Pi), 3e-7);
//
//    Float maxErr = 0, sumErr = 0;
//    for (int i = 0; i < 100000; ++i) {
//        RNG rng(i);
//        Vector3f a = Normalize(Vector3f(-1 + 2 * rng.Uniform<Float>(),
//                                        -1 + 2 * rng.Uniform<Float>(),
//                                        -1 + 2 * rng.Uniform<Float>()));
//        Vector3f b = Normalize(Vector3f(-1 + 2 * rng.Uniform<Float>(),
//                                        -1 + 2 * rng.Uniform<Float>(),
//                                        -1 + 2 * rng.Uniform<Float>()));
//
//        Vector3<double> ad(a), bd(b);
//        ad = Normalize(ad);
//        bd = Normalize(bd);
//
//        Float v[2] = {Float(std::acos(Dot(ad, bd))), AngleBetween(a, b)};
//        Float err = std::abs(v[0] - v[1]) / v[0];
//        maxErr = std::max(err, maxErr);
//        sumErr += err;
//        EXPECT_LT(err, 5e-6) << v[0] << "vs " << v[1] << ", a: " << a << ", b: " << b;
//    }
//    // CO    LOG(WARNING) << "MAXERR " << maxErr << ", sum " << sumErr;
//    maxErr = 0;
//    sumErr = 0;
//
//    for (int i = 0; i < 100000; ++i) {
//        RNG rng(i + 10000000);
//        Vector3f a = Normalize(Vector3f(-1 + 2 * rng.Uniform<Float>(),
//                                        -1 + 2 * rng.Uniform<Float>(),
//                                        -1 + 2 * rng.Uniform<Float>()));
//        Vector3f b = Normalize(Vector3f(-1 + 2 * rng.Uniform<Float>(),
//                                        -1 + 2 * rng.Uniform<Float>(),
//                                        -1 + 2 * rng.Uniform<Float>()));
//        // Make them face in opposite-ish directions
//        a.x = std::copysign(a.x, -b.x);
//        a.y = std::copysign(a.y, -b.y);
//        a.z = std::copysign(a.z, -b.z);
//
//        Vector3<double> ad(a), bd(b);
//        ad = Normalize(ad);
//        bd = Normalize(bd);
//
//        Float v[2] = {Float(std::acos(Dot(ad, bd))), AngleBetween(a, b)};
//        Float err = std::abs(v[0] - v[1]) / v[0];
//        maxErr = std::max(err, maxErr);
//        sumErr += err;
//        EXPECT_LT(err, 5e-6) << v[0] << "vs " << v[1] << ", a: " << a << ", b: " << b;
//    }
//    // CO    LOG(WARNING) << "MAXERR " << maxErr << ", sum " << sumErr;
//
//    Vector3f a(1, 1, 1), b(-1, -1.0001, -1);
//    a = Normalize(a);
//    b = Normalize(b);
//    Vector3<long double> ad(1, 1, 1), bd(-1, -1.0001, -1);
//    ad = Normalize(ad);
//    bd = Normalize(bd);
//
//    Float naive = SafeACos(Dot(a, b));
//    Float precise = std::acos(Clamp(Dot(ad, bd), -1, 1));
//    Float abet = AngleBetween(a, b);
//    EXPECT_EQ(abet, precise) << StringPrintf("vs naive %f", naive);
//}
//
//TEST(Vector, CoordinateSystem) {
//    // Duff et al 2017 footnote 1
//    auto error = [](Vector3f a, Vector3f b, Vector3f c) {
//        return (Sqr(Length(a) - 1) + Sqr(Length(b) - 1) + Sqr(Length(c) - 1) +
//                Sqr(Dot(a, b)) + Sqr(Dot(b, c)) + Sqr(Dot(c, a))) /
//               6;
//    };
//
//    // Coordinate axes.
//    Vector3f a, b;
//    for (Vector3f v : {Vector3f(1, 0, 0), Vector3f(-1, 0, 0), Vector3f(0, 1, 0),
//                       Vector3f(0, -1, 0), Vector3f(0, 0, 1), Vector3f(0, 0, -1)}) {
//        CoordinateSystem(v, &a, &b);
//        for (int c = 0; c < 3; ++c) {
//            if (v[c] != 0) {
//                EXPECT_EQ(0, a[c]);
//                EXPECT_EQ(0, b[c]);
//            }
//        }
//    }
//
//    // Bad vectors from Duff et al
//    for (Vector3f v : {Vector3f(0.00038527316, 0.00038460016, -0.99999988079),
//                       Vector3f(-0.00019813581, -0.00008946839, -0.99999988079)}) {
//        CoordinateSystem(v, &a, &b);
//        EXPECT_LT(error(v, a, b), 1e-10);
//    }
//
//    // Random vectors
//    RNG rng;
//    for (int i = 0; i < 1000; ++i) {
//        Point2f u = {rng.Uniform<Float>(), rng.Uniform<Float>()};
//        Vector3f v = SampleUniformSphere(u);
//        CoordinateSystem(v, &a, &b);
//        EXPECT_LT(error(v, a, b), 1e-10);
//    }
//}
//
//TEST(Bounds2, IteratorBasic) {
//    Bounds2i b{{0, 1}, {2, 3}};
//    Point2i e[] = {{0, 1}, {1, 1}, {0, 2}, {1, 2}};
//    int offset = 0;
//    for (auto p : b) {
//        EXPECT_LT(offset, PBRT_ARRAYSIZE(e));
//        EXPECT_EQ(e[offset], p) << "offset = " << offset;
//        ++offset;
//    }
//}
//
//TEST(Bounds2, IteratorDegenerate) {
//    Bounds2i b{{0, 0}, {0, 10}};
//    for (auto p : b) {
//        // This loop should never run.
//        bool reached = true;
//        EXPECT_FALSE(reached) << "p = " << p;
//        break;
//    }
//
//    Bounds2i b2{{0, 0}, {4, 0}};
//    for (auto p : b2) {
//        // This loop should never run.
//        bool reached = true;
//        EXPECT_FALSE(reached) << "p = " << p;
//        break;
//    }
//
//    Bounds2i b3;
//    for (auto p : b3) {
//        // This loop should never run.
//        bool reached = true;
//        EXPECT_FALSE(reached) << "p = " << p;
//        break;
//    }
//}
//
//
//using Bounds3f = AABB3f;
//
//TEST(Bounds3, PointDistance) {
//    {
//        Bounds3f b(Point3f(0, 0, 0), Point3f(1, 1, 1));
//
//        // Points inside the bounding box or on faces
//        EXPECT_EQ(0., Distance(Point3f(.5, .5, .5), b));
//        EXPECT_EQ(0., Distance(Point3f(0, 1, 1), b));
//        EXPECT_EQ(0., Distance(Point3f(.25, .8, 1), b));
//        EXPECT_EQ(0., Distance(Point3f(0, .25, .8), b));
//        EXPECT_EQ(0., Distance(Point3f(.7, 0, .8), b));
//
//        // Aligned with the plane of one of the faces
//        EXPECT_EQ(5., Distance(Point3f(6, 1, 1), b));
//        EXPECT_EQ(10., Distance(Point3f(0, -10, 1), b));
//
//        // 2 of the dimensions inside the box's extent
//        EXPECT_EQ(2., Distance(Point3f(0.5, 0.5, 3), b));
//        EXPECT_EQ(3., Distance(Point3f(0.5, 0.5, -3), b));
//        EXPECT_EQ(2., Distance(Point3f(0.5, 3, 0.5), b));
//        EXPECT_EQ(3., Distance(Point3f(0.5, -3, 0.5), b));
//        EXPECT_EQ(2., Distance(Point3f(3, 0.5, 0.5), b));
//        EXPECT_EQ(3., Distance(Point3f(-3, 0.5, 0.5), b));
//
//        // General points
//        EXPECT_EQ(3 * 3 + 7 * 7 + 10 * 10, DistanceSquared(Point3f(4, 8, -10), b));
//        EXPECT_EQ(6 * 6 + 10 * 10 + 7 * 7, DistanceSquared(Point3f(-6, -10, 8), b));
//    }
//
//    {
//        // A few with a more irregular box, just to be sure
//        Bounds3f b(Point3f(-1, -3, 5), Point3f(2, -2, 18));
//        EXPECT_EQ(0., Distance(Point3f(-.99, -2, 5), b));
//        EXPECT_EQ(2 * 2 + 6 * 6 + 4 * 4, DistanceSquared(Point3f(-3, -9, 22), b));
//    }
//}
//
//
//using Bounds2f = AABB2f;
//
//TEST(Bounds2, Union) {
//    Bounds2f a(Point2f(-10, -10), Point2f(0, 20));
//    Bounds2f b;  // degenerate
//    Bounds2f c = Union(a, b);
//    EXPECT_EQ(a, c);
//
//    EXPECT_EQ(b, Union(b, b));
//
//    Bounds2f d(Point2f(-15, 10));
//    Bounds2f e = Union(a, d);
//    EXPECT_EQ(Bounds2f(Point2f(-15, -10), Point2f(0, 20)), e);
//}
//
//
//
//TEST(Bounds3, Union) {
//    Bounds3f a(Point3f(-10, -10, 5), Point3f(0, 20, 10));
//    Bounds3f b;  // degenerate
//    Bounds3f c = Union(a, b);
//    EXPECT_EQ(a, c);
//
//    EXPECT_EQ(b, Union(b, b));
//
//    Bounds3f d(Point3f(-15, 10, 30));
//    Bounds3f e = Union(a, d);
//    EXPECT_EQ(Bounds3f(Point3f(-15, -10, 5), Point3f(0, 20, 30)), e);
//}

//TEST(EqualArea, Randoms) {
//    for (Point2f u : Uniform2D(100)) {
//        Vector3f v = SampleUniformSphere(u);
//        Point2f c = EqualAreaSphereToSquare(v);
//        Vector3f vp = EqualAreaSquareToSphere(c);
//        EXPECT_TRUE(Length(vp) > 0.9999 && Length(vp) < 1.0001) << Length(vp);
//        EXPECT_GT(Dot(v, vp), 0.9999) << v;
//    }
//}
//
//TEST(EqualArea, RemapEdges) {
//    auto checkClose = [&](Point2f a, Point2f b) {
//        Vector3f av = EqualAreaSquareToSphere(a);
//        b = WrapEqualAreaSquare(b);
//        Vector3f bv = EqualAreaSquareToSphere(b);
//        EXPECT_GT(Dot(av, bv), .99);
//    };
//
//    checkClose(Point2f(.25, .01), Point2f(.25, -.01));
//    checkClose(Point2f(.89, .01), Point2f(.89, -.01));
//
//    checkClose(Point2f(.25, .99), Point2f(.25, 1.01));
//    checkClose(Point2f(.89, .99), Point2f(.89, 1.01));
//
//    checkClose(Point2f(.01, .66), Point2f(-.01, .66));
//    checkClose(Point2f(.01, .15), Point2f(-.01, .15));
//
//    checkClose(Point2f(.99, .66), Point2f(1.01, .66));
//    checkClose(Point2f(.99, .15), Point2f(1.01, .15));
//
//    checkClose(Point2f(.01, .01), Point2f(-.01, -.01));
//    checkClose(Point2f(.99, .01), Point2f(1.01, -.01));
//    checkClose(Point2f(.01, .99), Point2f(-.01, 1.01));
//    checkClose(Point2f(.99, .99), Point2f(1.01, 1.01));
//}
//
//DirectionCone RandomCone(RNG &rng) {
//    Vector3f w = SampleUniformSphere({rng.Uniform<Float>(), rng.Uniform<Float>()});
//    return DirectionCone(w, -1 + 2 * rng.Uniform<Float>());
//}
//
//TEST(DirectionCone, UnionBasics) {
//    // First encloses second
//    DirectionCone c = Union(DirectionCone(Vector3f(0, 0, 1), std::cos(Pi / 2)),
//                            DirectionCone(Vector3f(.1, .1, 1), std::cos(.1)));
//    EXPECT_EQ(c.w, Vector3f(0, 0, 1));
//    EXPECT_EQ(c.cosTheta, std::cos(Pi / 2));
//
//    // Second encloses first
//    c = Union(DirectionCone(Vector3f(.1, .1, 1), std::cos(.1)),
//              DirectionCone(Vector3f(0, 0, 1), std::cos(Pi / 2)));
//    EXPECT_EQ(c.w, Vector3f(0, 0, 1));
//    EXPECT_EQ(c.cosTheta, std::cos(Pi / 2));
//
//    // Same direction, first wider
//    Vector3f w(1, .5, -.25);
//    c = Union(DirectionCone(w, std::cos(.12)), DirectionCone(w, std::cos(.03)));
//    EXPECT_EQ(Normalize(w), c.w);
//    EXPECT_FLOAT_EQ(std::cos(.12), c.cosTheta);
//
//    // Same direction, second wider
//    c = Union(DirectionCone(w, std::cos(.1)), DirectionCone(w, std::cos(.2)));
//    EXPECT_EQ(Normalize(w), c.w);
//    EXPECT_FLOAT_EQ(std::cos(.2), c.cosTheta);
//
//    // Exactly pointing in opposite directions and covering the sphere when
//    // it's all said and done.
//    c = Union(DirectionCone(Vector3f(-1, -1, -1), std::cos(Pi / 2)),
//              DirectionCone(Vector3f(1, 1, 1), std::cos(Pi / 2)));
//    EXPECT_EQ(c.cosTheta, std::cos(Pi));
//
//    // Basically opposite and a bit more than pi/2: should also be the
//    // whole sphere.
//    c = Union(DirectionCone(Vector3f(-1, -1, -1), std::cos(1.01 * Pi / 2)),
//              DirectionCone(Vector3f(1.001, 1, 1), std::cos(1.01 * Pi / 2)));
//    EXPECT_EQ(c.cosTheta, std::cos(Pi));
//
//    // Narrow and at right angles; angle should be their midpoint
//    c = Union(DirectionCone(Vector3f(1, 0, 0), std::cos(1e-3)),
//              DirectionCone(Vector3f(0, 1, 0), std::cos(1e-3)));
//    EXPECT_FLOAT_EQ(1, Dot(c.w, Normalize(Vector3f(1, 1, 0))));
//    EXPECT_LT(std::abs(std::cos((Pi / 2 + 2e-3) / 2) - c.cosTheta), 1e-3);
//}
//
//TEST(DirectionCone, UnionRandoms) {
//    RNG rng(16);
//
//    for (int i = 0; i < 100; ++i) {
//        DirectionCone a = RandomCone(rng), b = RandomCone(rng);
//        DirectionCone c = Union(a, b);
//
//        for (int j = 0; j < 100; ++j) {
//            Vector3f w =
//                SampleUniformSphere({rng.Uniform<Float>(), rng.Uniform<Float>()});
//            if (Inside(a, w) || Inside(b, w))
//                EXPECT_TRUE(Inside(c, w))
//                    << "a: " << a << ", b: " << b << ", union: " << c << ", w: " << w;
//        }
//    }
//}
//
//TEST(DirectionCone, BoundBounds) {
//    Bounds3f b(Point3f(0, 0, 0), Point3f(1, 1, 1));
//
//    // Point inside the bbox
//    DirectionCone c = BoundSubtendedDirections(b, Point3f(.1, .2, .3));
//    EXPECT_EQ(std::cos(Pi), c.cosTheta);
//
//    // Outside, .5 units away in the middle (so using the direction to the
//    // center gives the best bound).
//    //
//    // tan theta = (sqrt(.5^2 + .5^2)) / .5
//    c = BoundSubtendedDirections(b, Point3f(-.5, .5, .5));
//    Float theta = std::acos(c.cosTheta);
//    Float precise = std::atan(std::sqrt(.5 * .5 + .5 * .5) / .5);
//    // Make sure the returned bound isn't too small.
//    EXPECT_GE(theta, 1.0001 * precise);
//    // It's fine for it to be a bit big (as it is in practice due to
//    // approximations for performance), but it shouldn't be too big.
//    EXPECT_LT(theta, 1.1 * precise);
//
//    RNG rng(512);
//    for (int i = 0; i < 1000; ++i) {
//        Bounds3f b(
//            Point3f(Lerp(rng.Uniform<Float>(), -1, 1), Lerp(rng.Uniform<Float>(), -1, 1),
//                    Lerp(rng.Uniform<Float>(), -1, 1)),
//            Point3f(Lerp(rng.Uniform<Float>(), -1, 1), Lerp(rng.Uniform<Float>(), -1, 1),
//                    Lerp(rng.Uniform<Float>(), -1, 1)));
//
//        Point3f p(Lerp(rng.Uniform<Float>(), -4, 4), Lerp(rng.Uniform<Float>(), -4, 4),
//                  Lerp(rng.Uniform<Float>(), -4, 4));
//
//        c = BoundSubtendedDirections(b, p);
//        if (Inside(p, b)) {
//            EXPECT_EQ(std::cos(Pi), c.cosTheta);
//        } else {
//            Vector3f wx, wy;
//            CoordinateSystem(c.w, &wx, &wy);
//            for (int j = 0; j < 1000; ++j) {
//                Point2f u(rng.Uniform<Float>(), rng.Uniform<Float>());
//                Vector3f w = SampleUniformSphere(u);
//                Ray r(p, w);
//                bool hit = b.IntersectP(r.o, r.d);
//                if (hit) {
//                    EXPECT_TRUE(Inside(c, w));
//                }
//                if (!Inside(c, w)) {
//                    EXPECT_FALSE(hit);
//                }
//            }
//        }
//    }
//}
//
//TEST(DirectionCone, VectorInCone) {
//    RNG rng;
//    for (int i = 0; i < 100; ++i) {
//        DirectionCone dc = RandomCone(rng);
//
//        for (int j = 0; j < 100; ++j) {
//            Vector3f wRandom =
//                SampleUniformSphere({rng.Uniform<Float>(), rng.Uniform<Float>()});
//            Vector3f wClosest = dc.ClosestVectorInCone(wRandom);
//
//            if (Inside(dc, wRandom))
//                EXPECT_GT(Dot(wClosest, wRandom), .999);
//            else {
//                // Uniformly sample the circle at the cone's boundary and
//                // keep the vector that's closest to wRandom.
//                Float sinTheta = SafeSqrt(1 - dc.cosTheta * dc.cosTheta);
//                Frame f = Frame::FromZ(dc.w);
//
//                Vector3f wBest;
//                Float bestDot = -1;
//                const int nk = 1000;
//                for (int k = 0; k < nk; ++k) {
//                    Float phi = (k + .5) / nk * 2 * Pi;
//                    Vector3f w = SphericalDirection(sinTheta, dc.cosTheta, phi);
//                    w = f.FromLocal(w);
//                    if (Dot(w, wRandom) > bestDot) {
//                        wBest = w;
//                        bestDot = Dot(w, wRandom);
//                    }
//                }
//                EXPECT_GT(Dot(wBest, wClosest), .999)
//                    << wBest << " vs " << wClosest << ", dot " << Dot(wBest, wClosest);
//            }
//        }
//    }
//}
//
//TEST(SphericalTriangleArea, Basics) {
//    {
//        Float a = SphericalTriangleArea(Vector3f(1, 0, 0), Vector3f(0, 1, 0),
//                                        Vector3f(0, 0, 1));
//        EXPECT_TRUE(a >= .9999 * Pi / 2 && a <= 1.00001 * Pi / 2) << a;
//    }
//
//    {
//        Float a = SphericalTriangleArea(Vector3f(1, 0, 0), Normalize(Vector3f(1, 1, 0)),
//                                        Vector3f(0, 0, 1));
//        EXPECT_TRUE(a >= .9999 * Pi / 4 && a <= 1.00001 * Pi / 4) << a;
//    }
//
//    // Random rotations
//    RNG rng;
//    for (int i = 0; i < 100; ++i) {
//        Vector3f axis = SampleUniformSphere({rng.Uniform<Float>(), rng.Uniform<Float>()});
//        Float theta = 2 * Pi * rng.Uniform<Float>();
//        Transform t = Rotate(theta, axis);
//        Vector3f va = t(Vector3f(1, 0, 0));
//        Vector3f vb = t(Vector3f(0, 1, 0));
//        Vector3f vc = t(Vector3f(0, 0, 1));
//        Float a = SphericalTriangleArea(va, vb, vc);
//        EXPECT_TRUE(a >= .9999 * Pi / 2 && a <= 1.0001 * Pi / 2) << a;
//    }
//
//    for (int i = 0; i < 100; ++i) {
//        Vector3f axis = SampleUniformSphere({rng.Uniform<Float>(), rng.Uniform<Float>()});
//        Float theta = 2 * Pi * rng.Uniform<Float>();
//        Transform t = Rotate(theta, axis);
//        Vector3f va = t(Vector3f(1, 0, 0));
//        Vector3f vb = t(Normalize(Vector3f(1, 1, 0)));
//        Vector3f vc = t(Vector3f(0, 0, 1));
//        Float a = SphericalTriangleArea(va, vb, vc);
//        EXPECT_TRUE(a >= .9999 * Pi / 4 && a <= 1.0001 * Pi / 4) << a;
//    }
//}
//
//TEST(SphericalTriangleArea, RandomSampling) {
//    for (int i = 0; i < 100; ++i) {
//        RNG rng(i);
//        Vector3f a = SampleUniformSphere({rng.Uniform<Float>(), rng.Uniform<Float>()});
//        Vector3f b = SampleUniformSphere({rng.Uniform<Float>(), rng.Uniform<Float>()});
//        Vector3f c = SampleUniformSphere({rng.Uniform<Float>(), rng.Uniform<Float>()});
//
//        Vector3f axis = Normalize(a + b + c);
//        Frame frame = Frame::FromZ(axis);
//        Float cosTheta = std::min({Dot(a, axis), Dot(b, axis), Dot(c, axis)});
//
//        Float area = SphericalTriangleArea(a, b, c);
//        bool sampleSphere = area > Pi;
//        int sqrtN = 200;
//        int count = 0;
//        for (Point2f u : Hammersley2D(sqrtN * sqrtN)) {
//            Vector3f v;
//            if (sampleSphere)
//                v = SampleUniformSphere(u);
//            else {
//                v = SampleUniformCone(u, cosTheta);
//                v = frame.FromLocal(v);
//            }
//
//            if (IntersectTriangle(Ray(Point3f(0, 0, 0), v), Infinity, Point3f(a),
//                                  Point3f(b), Point3f(c)))
//                ++count;
//        }
//
//        Float pdf = sampleSphere ? UniformSpherePDF() : UniformConePDF(cosTheta);
//        Float estA = Float(count) / (sqrtN * sqrtN * pdf);
//
//        Float error = std::abs((estA - area) / area);
//        EXPECT_LT(error, 0.035f) << "Area " << area << ", estimate " << estA
//                                 << ", va = " << a << ", vb = " << b << ", vc = " << c;
//    }
//}

using Point3fi = Theia::Point3Interval;
using Vector3fi = Theia::Vector3Interval;

TEST(PointVector, Interval) {
    // This is really just to make sure that various expected things
    // compile in the first place when using the interval variants of
    // these...
    Point3fi p(1, 2, 3), q(6, 9, 1);
    Vector3fi v(4, 5, 6);

    p += v;
    p = (p - v);
    p = p + 4 * v;
    Interval d = Dot(v, v);
    d = DistanceSquared(p, q);
    d = Distance(p, q);

#if 0
    v = Floor(v);
    p = Ceil(p);

    // These require a little more work since Min(Interval<T>, Interval<T>)
    // ends up returning T...
    v = Min(v, v);
    p = Max(p, q);
#endif

    Vector3fi vv = Cross(v, v);
}

//TEST(OctahedralVector, EncodeDecode) {
//    for (Point2 u : Hammersley2D(65535)) {
//        Vector3f v = SampleUniformSphere(u);
//
//        OctahedralVector ov(v);
//        Vector3f v2 = Vector3f(ov);
//
//        EXPECT_GT(Length(v2), .999f);
//        EXPECT_LT(Length(v2), 1.001f);
//        EXPECT_LT(std::abs(1 - Dot(v2, v)), .001f);
//    }
//}