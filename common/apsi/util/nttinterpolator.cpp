#include "nttinterpolator.h"

#define log2(x) (__builtin_ctz(x))

namespace NTT {
    std::vector<uint64_t> w[C + 1];
    std::vector<uint64_t> rev_list[n + 1];
    void init_rev()
    {
        for (int i = 0; i <= n; i++) {
            uint64_t lim = 1 << i;
            rev_list[i].resize(lim);
            for (uint64_t j = 0; j < lim; j++)
                rev_list[i][j] = (rev_list[i][j >> 1] >> 1) | ((j & 1) * (lim >> 1));
        }
    }
    void init_w()
    {
        for (uint64_t i = 1; i <= C; i++)
            w[i].resize((1 << (i - 1)));
        uint64_t wn = fast_power(G, (mod - 1) / (1 << C));
        w[C][0] = 1;
        for (uint64_t i = 1; i < (1 << (C - 1)); i++)
            w[C][i] = mul(w[C][i - 1], wn);
        for (uint64_t i = C - 1; i; i--)
            for (uint64_t j = 0; j < (1 << (i - 1)); j++)
                w[i][j] = w[i + 1][j << 1];
    }

    // kd is the ntt direction, -1 for intt
    void ntt(std::vector<uint64_t> &f, uint64_t lim, uint64_t kd, uint64_t *rev)
    {
        for (uint64_t i = 0; i < lim; i++)
            if (i > rev[i])
                std::swap(f[i], f[rev[i]]);
        for (uint64_t mid = 1, l = 1; mid < lim; mid <<= 1, l++)
            for (uint64_t i = 0, a0, a1; i < lim; i += (mid << 1))
                for (uint64_t j = 0; j < mid; j++) {
                    a0 = f[i + j], a1 = mul(f[i + j + mid], w[l][j]);
                    f[i + j] = add(a0, a1), f[i + j + mid] = sub(a0, a1);
                }
        if (kd == -1) {
            std::reverse(f.begin() + 1, f.begin() + lim);
            for (uint64_t i = 0, inv = fast_power(lim, mod - 2); i < lim; i++)
                f[i] = mul(f[i], inv);
        }
    }

    inline std::vector<uint64_t> operator+(const std::vector<uint64_t> &a, const std::vector<uint64_t> &b)
    {
        std::vector<uint64_t> c(std::max(a.size(), b.size()), 0);
        for (uint64_t i = 0; i < c.size(); i++)
            c[i] = add(a[i], b[i]);
        return c;
    }
    inline std::vector<uint64_t> operator-(const std::vector<uint64_t> &a, const std::vector<uint64_t> &b)
    {
        std::vector<uint64_t> c(std::max(a.size(), b.size()), 0);
        for (uint64_t i = 0; i < c.size(); i++)
            c[i] = sub(a[i], b[i]);
        return c;
    }
    inline std::vector<uint64_t> operator*(std::vector<uint64_t> a, std::vector<uint64_t> b)
    {
        uint64_t deg = a.size() + b.size() - 1, lim = 1;
        if (deg <= 128) {
            std::vector<uint64_t> c(deg, 0);
            for (uint64_t i = 0; i < a.size(); i++)
                for (uint64_t j = 0; j < b.size(); j++)
                    c[i + j] = add(c[i + j], mul(a[i], b[j]));
            return c;
        }
        while (lim < deg)
            lim <<= 1;
        a.resize(lim), ntt(a, lim, 1, rev_list[log2(lim)].data());
        b.resize(lim), ntt(b, lim, 1, rev_list[log2(lim)].data());
        for (uint64_t i = 0; i < lim; i++)
            a[i] = mul(a[i], b[i]);
        ntt(a, lim, -1, rev_list[log2(lim)].data()), a.resize(deg);
        return a;
    }
    inline std::vector<uint64_t> Inv(std::vector<uint64_t> a, uint64_t deg)
    {
        std::vector<uint64_t> b, c(1, fast_power(a[0], mod - 2));
        for (uint64_t lim = 4; lim < (deg << 2); lim <<= 1) {
            b = a, b.resize(lim >> 1);
            b.resize(lim), ntt(b, lim, 1, rev_list[log2(lim)].data());
            c.resize(lim), ntt(c, lim, 1, rev_list[log2(lim)].data());
            for (uint64_t i = 0; i < lim; i++)
                c[i] = mul(c[i], sub(2, mul(b[i], c[i])));
            ntt(c, lim, -1, rev_list[log2(lim)].data()), c.resize(lim >> 1);
        }
        c.resize(deg);
        return c;
    }
    inline std::vector<uint64_t> operator/(std::vector<uint64_t> a, std::vector<uint64_t> b)
    {
        uint64_t lim = 1, deg = a.size() - b.size() + 1;
        std::reverse(a.begin(), a.end());
        std::reverse(b.begin(), b.end());
        while (lim < deg)
            lim <<= 1;
        b = Inv(b, lim), b.resize(deg);
        a = a * b, a.resize(deg);
        std::reverse(a.begin(), a.end());
        return a;
    }
    inline std::vector<uint64_t> operator%(std::vector<uint64_t> a, std::vector<uint64_t> b)
    {
        std::vector<uint64_t> c = a - (a / b) * b;
        c.resize(b.size() - 1);
        return c;
    }
    inline std::vector<uint64_t> deriv(std::vector<uint64_t> a)
    {
        for (uint64_t i = 0; i < a.size() - 1; i++)
            a[i] = mul(a[i + 1], i + 1);
        a.pop_back();
        return a;
    }
    uint64_t F(const std::vector<uint64_t> a, uint64_t x)
    {
        uint64_t p = 1, res = 0;
        for (uint64_t i = 0; i < a.size(); i++, p = mul(p, x))
            res = add(res, mul(a[i], p));
        return res;
    }
#define lc (u << 1)
#define rc ((u << 1) | 1)
#define mid ((l + r) >> 1)

    void NTTInterpolator::build()
    {
        build_inner(1, 1, n);
    }

    void NTTInterpolator::build_inner(uint64_t u, uint64_t l, uint64_t r)
    {
        if (l == r) {
            f[u].push_back(mod - x[l]), f[u].push_back(1);
            return;
        }
        build_inner(lc, l, mid), build_inner(rc, mid + 1, r);
        f[u] = f[lc] * f[rc];
    }

    void NTTInterpolator::calc(std::vector<std::vector<uint64_t>> &y)
    {
        return calc_inner(1, 1, n, deriv_f, y);
    }
    void NTTInterpolator::calc_inner(
        uint64_t u, uint64_t l, uint64_t r, std::vector<uint64_t> res, std::vector<std::vector<uint64_t>> &y)
    {
        if (l == r) {
            for (uint64_t i = 0; i < poly_count; i++)
                g[i][l] = mul(fast_power(F(res, x[l]), mod - 2), y[i][l]);
            return;
        }
        calc_inner(lc, l, mid, res % f[lc], y), calc_inner(rc, mid + 1, r, res % f[rc], y);
    }
    std::vector<std::vector<uint64_t>> NTTInterpolator::get_ans()
    {
        std::vector<std::vector<uint64_t>> res;
        for (uint64_t i = 0; i < poly_count; i++) {
            res.push_back(get_ans_inner(1, 1, n, g[i]));
        }
        return res;
    }

    std::vector<uint64_t> NTTInterpolator::get_ans_inner(uint64_t u, uint64_t l, uint64_t r, std::vector<uint64_t> &g)
    {
        if (l == r)
            return std::vector<uint64_t>(1, g[l]);
        std::vector<uint64_t> ansl = get_ans_inner(lc, l, mid, g), ansr = get_ans_inner(rc, mid + 1, r, g);
        return ansl * f[rc] + ansr * f[lc];
    }

    void NTTInterpolator::init()
    {
        init_w();
        init_rev();
    }

    void NTTInterpolator::init_with_params(uint64_t n, std::vector<uint64_t> &x)
    {
        for (auto i = 0; i < (N << 1); i++)
            this->f[i].clear();
        this->n = n;
        this->x = x;
        this->build();
        this->deriv_f = deriv(f[1]);
    }

    std::vector<std::vector<uint64_t>> NTTInterpolator::fast_lagrange(std::vector<std::vector<uint64_t>> y)
    {
        poly_count = y.size();
        g = std::vector<std::vector<uint64_t>>(poly_count, std::vector<uint64_t>(n + 1, 0));
        calc(y);
        return get_ans();
    }

} // namespace NTT
