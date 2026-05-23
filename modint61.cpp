#include <iostream>

class modint61 {
private:
    // 法となる 2^61 - 1
    static constexpr long long MOD = (1LL << 61) - 1;
    long long v;

    // 2^61 - 1 の剰余をビット演算で高速に求める関数
    static inline long long reduce(__int128_t x) {
        // x = q * 2^61 + r としたとき、x ≡ q + r (mod 2^61 - 1)
        x = (x >> 61) + (x & MOD);
        if (x >= MOD) x -= MOD;
        return (long long)x;
    }

public:
    // デフォルトコンストラクタ
    modint61() : v(0) {}

    // intやlong longなどからの暗黙の変換を許可するコンストラクタ
    template <class T>
    modint61(T x) {
        long long y = (long long)(x % MOD);
        if (y < 0) y += MOD;
        v = y;
    }

    // 生の値を取得
    long long val() const { return v; }

    // 演算と代入を同時に行う演算子 (+=, -=, *=, /=)
    modint61& operator+=(const modint61& rhs) {
        v += rhs.v;
        if (v >= MOD) v -= MOD;
        return *this;
    }

    modint61& operator-=(const modint61& rhs) {
        v -= rhs.v;
        if (v < 0) v += MOD;
        return *this;
    }

    modint61& operator*=(const modint61& rhs) {
        // 乗算は 122bit になる可能性があるため __int128_t で計算してリダクション
        v = reduce((__int128_t)v * rhs.v);
        return *this;
    }

    modint61& operator/=(const modint61& rhs) {
        return *this *= rhs.inv();
    }

    // x.pow(n): 繰り返し二乗法で O(log n) で計算
    modint61 pow(long long n) const {
        modint61 res = 1;
        modint61 x = *this;
        while (n > 0) {
            if (n & 1) res *= x;
            x *= x;
            n >>= 1;
        }
        return res;
    }

    // 逆元: フェルマーの小定理 (a^(p-2) ≡ a^-1 mod p) を利用
    modint61 inv() const {
        return pow(MOD - 2);
    }

    // 二項演算子 (+, -, *, /)
    friend modint61 operator+(modint61 lhs, const modint61& rhs) { return lhs += rhs; }
    friend modint61 operator-(modint61 lhs, const modint61& rhs) { return lhs -= rhs; }
    friend modint61 operator*(modint61 lhs, const modint61& rhs) { return lhs *= rhs; }
    friend modint61 operator/(modint61 lhs, const modint61& rhs) { return lhs /= rhs; }

    // 比較演算子
    friend bool operator==(const modint61& lhs, const modint61& rhs) { return lhs.v == rhs.v; }
    friend bool operator!=(const modint61& lhs, const modint61& rhs) { return lhs.v != rhs.v; }

    // std::cout << で出力できるようにするオーバーライド
    friend std::ostream& operator<<(std::ostream& os, const modint61& m) {
        return os << m.v;
    }
};
