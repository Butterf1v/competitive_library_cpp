#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <cassert>

class Random {
private:
    std::mt19937_64 mt;

public:
    // コンストラクタ：現在時刻でシードを自動初期化
    Random() {
        mt.seed(std::chrono::steady_clock::now().time_since_epoch().count());
    }

    // 任意のシード値を設定（デバッグ・再現性確保用）
    void seed(long long s) {
        mt.seed(s);
    }

    // randint(a, b): a 以上 b 以下のランダムな整数を返す
    long long randint(long long a, long long b) {
        assert(a <= b);
        std::uniform_int_distribution<long long> dist(a, b);
        return dist(mt);
    }

    // randrange(stop): 0 以上 stop 未満のランダムな整数を返す
    long long randrange(long long stop) {
        assert(stop > 0);
        return randint(0, stop - 1);
    }

    // randrange(start, stop): start 以上 stop 未満のランダムな整数を返す
    long long randrange(long long start, long long stop) {
        assert(start < stop);
        return randint(start, stop - 1);
    }

    // choice(v): 配列からランダムに1つの要素を選んで返す
    template<typename T>
    T choice(const std::vector<T>& v) {
        assert(!v.empty());
        return v[randint(0, (long long)v.size() - 1)];
    }

    // shuffle(v): 配列をインプレースでランダムにシャッフルする
    template<typename T>
    void shuffle(std::vector<T>& v) {
        std::shuffle(v.begin(), v.end(), mt);
    }

    // random(): 0.0 以上 1.0 未満の浮動小数点数を返す（確率の判定などに便利）
    double random() {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(mt);
    }

    // get(): mt19937_64 の生の値 (0 ~ 2^64-1) をそのまま返す
    unsigned long long get() {
        return mt();
    }
};

// グローバルに1つインスタンスを作っておくと便利です
Random rnd;
