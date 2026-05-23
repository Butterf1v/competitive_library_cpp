#include <vector>
#include <cassert>
#include <algorithm>
#include <utility>

template <typename T>
struct Matrix {
    int h, w;
    std::vector<T> dat;

    // コンストラクタ
    Matrix() : h(0), w(0) {}
    Matrix(int h, int w, T val = T(0)) : h(h), w(w), dat(h * w, val) {}
    Matrix(const std::vector<std::vector<T>>& a) {
        h = a.size();
        w = (h > 0 ? a[0].size() : 0);
        dat.assign(h * w, T(0));
        for (int i = 0; i < h; ++i) {
            assert((int)a[i].size() == w); // 列数が揃っているか
            for (int j = 0; j < w; ++j) {
                (*this)[i][j] = a[i][j];
            }
        }
    }

    // 要素へのアクセス: 行列[i][j] の形でアクセスできるようにポインタを返す
    T* operator[](int i) { return dat.data() + i * w; }
    const T* operator[](int i) const { return dat.data() + i * w; }

    // 単位行列の生成
    static Matrix identity(int n) {
        Matrix res(n, n);
        for (int i = 0; i < n; ++i) res[i][i] = T(1);
        return res;
    }

    // 代入演算子と四則演算
    Matrix& operator+=(const Matrix& B) {
        assert(h == B.h && w == B.w);
        for (int i = 0; i < h * w; ++i) dat[i] += B.dat[i];
        return *this;
    }

    Matrix& operator-=(const Matrix& B) {
        assert(h == B.h && w == B.w);
        for (int i = 0; i < h * w; ++i) dat[i] -= B.dat[i];
        return *this;
    }

    Matrix& operator*=(const Matrix& B) {
        assert(w == B.h);
        Matrix C(h, B.w);
        // キャッシュ効率を考慮した i-k-j ループ
        for (int i = 0; i < h; ++i) {
            for (int k = 0; k < w; ++k) {
                T val = (*this)[i][k];
                if (val == T(0)) continue; // 定数倍最適化
                for (int j = 0; j < B.w; ++j) {
                    C[i][j] += val * B[k][j];
                }
            }
        }
        *this = std::move(C);
        return *this;
    }

    Matrix operator+(const Matrix& B) const { return Matrix(*this) += B; }
    Matrix operator-(const Matrix& B) const { return Matrix(*this) -= B; }
    Matrix operator*(const Matrix& B) const { return Matrix(*this) *= B; }

    // スカラー倍
    Matrix& operator*=(const T& val) {
        for (int i = 0; i < h * w; ++i) dat[i] *= val;
        return *this;
    }
    Matrix operator*(const T& val) const { return Matrix(*this) *= val; }

    // 比較演算子
    bool operator==(const Matrix& B) const {
        return h == B.h && w == B.w && dat == B.dat;
    }
    bool operator!=(const Matrix& B) const {
        return !(*this == B);
    }

    // 行列累乗 (自己書き換えではなく戻り値として返す)
    Matrix pow(long long k) const {
        assert(h == w); // 正方行列のみ
        Matrix res = identity(h);
        Matrix A = *this;
        while (k > 0) {
            if (k & 1) res *= A;
            A *= A;
            k >>= 1;
        }
        return res;
    }

    // 掃き出し法 (破壊的)
    // 戻り値は rank, is_extended = true の場合は拡大係数行列として扱い、一番右の列を掃き出しの軸にしない
    int sweep(bool is_extended = false) {
        int r = 0;
        int limit_w = w - (is_extended ? 1 : 0);
        for (int c = 0; c < limit_w && r < h; ++c) {
            int pivot = -1;
            // T が double 等の場合は std::abs を用いて絶対値最大のピボットを探す必要がありますが、
            // ModInt 等の場合は 0 でないものを見つければ十分です。
            for (int i = r; i < h; ++i) {
                if ((*this)[i][c] != T(0)) {
                    pivot = i;
                    break;
                }
            }
            if (pivot == -1) continue;

            if (pivot != r) {
                for (int j = 0; j < w; ++j) {
                    std::swap((*this)[r][j], (*this)[pivot][j]);
                }
            }

            T inv_val = T(1) / (*this)[r][c];
            for (int j = c; j < w; ++j) {
                (*this)[r][j] *= inv_val;
            }

            for (int i = 0; i < h; ++i) {
                if (i == r) continue;
                T factor = (*this)[i][c];
                if (factor != T(0)) {
                    for (int j = c; j < w; ++j) {
                        (*this)[i][j] -= factor * (*this)[r][j];
                    }
                }
            }
            ++r;
        }
        return r;
    }

    // 行列式 (破壊的にならないようにコピーして計算)
    T det() const {
        assert(h == w);
        Matrix A = *this;
        T res = 1;
        for (int i = 0; i < h; ++i) {
            int pivot = -1;
            for (int j = i; j < h; ++j) {
                if (A[j][i] != T(0)) {
                    pivot = j;
                    break;
                }
            }
            if (pivot == -1) return T(0); // 正則でない
            if (pivot != i) {
                res = res * T(-1); // 行の入れ替えで符号反転
                for (int j = i; j < w; ++j) {
                    std::swap(A[i][j], A[pivot][j]);
                }
            }
            res *= A[i][i];
            T inv_val = T(1) / A[i][i];
            for (int j = i; j < w; ++j) A[i][j] *= inv_val;
            for (int j = i + 1; j < h; ++j) {
                T factor = A[j][i];
                if (factor != T(0)) {
                    for (int k = i; k < w; ++k) {
                        A[j][k] -= factor * A[i][k];
                    }
                }
            }
        }
        return res;
    }

    // ランク (階数)
    int rank() const {
        Matrix A = *this;
        return A.sweep();
    }

    // 逆行列 (存在しない場合は assert または bool を返すなど用途に合わせて変更)
    // 戻り値: {正則か否か, 逆行列}
    std::pair<bool, Matrix> inv() const {
        assert(h == w);
        Matrix A(h, h * 2);
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < h; ++j) {
                A[i][j] = (*this)[i][j];
            }
            A[i][h + i] = T(1);
        }
        int r = A.sweep();
        if (r < h) return {false, Matrix(h, h)}; // 逆行列を持たない
        Matrix res(h, h);
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < h; ++j) {
                res[i][j] = A[i][h + j];
            }
        }
        return {true, res};
    }

    // 連立一次方程式 Ax = B を解く
    // B は列ベクトルを想定 (h × 1 行列など)
    // 戻り値: {解が存在するか否か, 特殊解} (一般解・カーネルを求める必要がある場合は適宜拡張してください)
    std::pair<bool, Matrix> linear_equations(const Matrix& B) const {
        assert(h == B.h);
        Matrix A(h, w + B.w);
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) A[i][j] = (*this)[i][j];
            for (int j = 0; j < B.w; ++j) A[i][w + j] = B[i][j];
        }
        int r = A.sweep(true); // 拡大係数行列として掃き出す

        // 解の存在判定 (Ax = 0 の左辺が 0 なのに右辺が非 0 の行があったら解なし)
        for (int i = r; i < h; ++i) {
            for (int j = 0; j < B.w; ++j) {
                if (A[i][w + j] != T(0)) return {false, Matrix(w, B.w)};
            }
        }

        Matrix res(w, B.w); // 特殊解 (任意変数はすべて 0 とする)
        for (int i = 0; i < r; ++i) {
            // ピボットを見つける
            int pivot_col = -1;
            for (int j = 0; j < w; ++j) {
                if (A[i][j] != T(0)) {
                    pivot_col = j;
                    break;
                }
            }
            assert(pivot_col != -1);
            for (int j = 0; j < B.w; ++j) {
                res[pivot_col][j] = A[i][w + j];
            }
        }
        return {true, res};
    }

    // 転置行列
    Matrix transpose() const {
        Matrix res(w, h);
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                res[j][i] = (*this)[i][j];
            }
        }
        return res;
    }
};
