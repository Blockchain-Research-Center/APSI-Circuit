#pragma once
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <vector>

namespace NTT {
    const uint64_t mod = 1179649, G = 19;
    inline std::uint64_t add(std::uint64_t a, std::uint64_t b)
    {
        return a + b >= mod ? a + b - mod : a + b;
    }
    inline std::uint64_t sub(std::uint64_t a, std::uint64_t b)
    {
        return a >= b ? a - b : a - b + mod;
    }
    inline std::uint64_t mul(std::uint64_t a, std::uint64_t b)
    {
        return 1ll * a * b >= mod ? 1ll * a * b % mod : a * b;
    }
    inline std::uint64_t fast_power(std::uint64_t a, std::uint64_t b, std::uint64_t res = 1)
    {
        for (; b; b >>= 1, a = mul(a, a))
            (b & 1) ? (res = mul(res, a)) : 0;
        return res;
    }
    // N: poly degree limit
    // C: power limit
    const std::uint64_t N = (1 << 17) + 1, C = 14;
    // Init all powers
    void init_w();
    // Evaluate polynomial with coefficients a at x
    std::uint64_t F(const std::vector<std::uint64_t> a, std::uint64_t x);
    class NTTInterpolator {
    public:
        static void init();
        void init_with_params(std::uint64_t n, std::vector<std::uint64_t> &x);
        std::vector<std::vector<std::uint64_t>> fast_lagrange(std::vector<std::vector<std::uint64_t>> y);

    private:
        void build_inner(std::uint64_t u, std::uint64_t l, std::uint64_t r);
        void calc_inner(
            std::uint64_t u,
            std::uint64_t l,
            std::uint64_t r,
            std::vector<std::uint64_t> res,
            std::vector<std::vector<std::uint64_t>> &y);
        std::vector<std::uint64_t> get_ans_inner(
            std::uint64_t u, std::uint64_t l, std::uint64_t r, std::vector<std::uint64_t> &g);
        void build();
        void calc(std::vector<std::vector<std::uint64_t>> &y);
        std::vector<std::vector<std::uint64_t>> get_ans();
        std::uint64_t n;
        std::uint64_t poly_count;
        std::vector<std::uint64_t> x;
        std::vector<std::uint64_t> f[N << 2];
        std::vector<std::uint64_t> deriv_f;
        std::vector<std::vector<std::uint64_t>> g;
    };
} // namespace NTT
