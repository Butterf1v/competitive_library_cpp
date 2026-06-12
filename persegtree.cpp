#include <vector>

template <class S, S (*op)(S, S), S (*e)()> struct persegtree {
private:
  struct Node {
    S val;
    int lc, rc;
  };
  int _n;
  std::vector<Node> nodes;
  std::vector<int> roots;

  int new_node(S val, int lc = -1, int rc = -1) {
    nodes.push_back({val, lc, rc});
    return (int)nodes.size() - 1;
  }

  int build_init(int l, int r, const std::vector<S>& v) {
    if (l + 1 == r) return new_node(v[l]);
    int m = (l + r) / 2;
    int lc = build_init(l, m, v);
    int rc = build_init(m, r, v);
    return new_node(op(nodes[lc].val, nodes[rc].val), lc, rc);
  }

  int build_init(int l, int r) {
    if (l + 1 == r) return new_node(e());
    int m = (l + r) / 2;
    int lc = build_init(l, m);
    int rc = build_init(m, r);
    return new_node(op(nodes[lc].val, nodes[rc].val), lc, rc);
  }

  int set_internal(int t, int l, int r, int p, S x) {
    if (l + 1 == r) return new_node(x);
    int m = (l + r) / 2;
    int lc = nodes[t].lc;
    int rc = nodes[t].rc;
    if (p < m) lc = set_internal(lc, l, m, p, x);
    else rc = set_internal(rc, m, r, p, x);
    return new_node(op(nodes[lc].val, nodes[rc].val), lc, rc);
  }

  S get_internal(int t, int l, int r, int p) const {
    if (l + 1 == r) return nodes[t].val;
    int m = (l + r) / 2;
    if (p < m) return get_internal(nodes[t].lc, l, m, p);
    else return get_internal(nodes[t].rc, m, r, p);
  }

  S prod_internal(int t, int l, int r, int ql, int qr) const {
    if (qr <= l || r <= ql) return e();
    if (ql <= l && r <= qr) return nodes[t].val;
    int m = (l + r) / 2;
    return op(prod_internal(nodes[t].lc, l, m, ql, qr), prod_internal(nodes[t].rc, m, r, ql, qr));
  }

  template <class F> int max_right_internal(int t, int l, int r, int ql, S& sm, F f) const {
    if (r <= ql) return r;
    if (ql <= l) {
      if (f(op(sm, nodes[t].val))) {
        sm = op(sm, nodes[t].val);
        return r;
      }
      if (l + 1 == r) return l;
    }
    int m = (l + r) / 2;
    int res = max_right_internal(nodes[t].lc, l, m, ql, sm, f);
    if (res < m) return res;
    return max_right_internal(nodes[t].rc, m, r, ql, sm, f);
  }

  template <class F> int min_left_internal(int t, int l, int r, int qr, S& sm, F f) const {
    if (qr <= l) return l;
    if (r <= qr) {
      if (f(op(nodes[t].val, sm))) {
        sm = op(nodes[t].val, sm);
        return l;
      }
      if (l + 1 == r) return r;
    }
    int m = (l + r) / 2;
    int res = min_left_internal(nodes[t].rc, m, r, qr, sm, f);
    if (res > m) return res;
    return min_left_internal(nodes[t].lc, l, m, qr, sm, f);
  }

public:
  persegtree() : _n(0) {
    roots.push_back(-1);
  }
  explicit persegtree(int n) : _n(n) {
    if (n > 0) {
      roots.push_back(build_init(0, n));
    } else {
      roots.push_back(-1);
    }
  }
  explicit persegtree(const std::vector<S>& v) : _n(v.size()) {
    if (_n > 0) {
      roots.push_back(build_init(0, _n, v));
    } else {
      roots.push_back(-1);
    }
  }

  int set(int v, int p, S x) {
    int root = set_internal(roots[v], 0, _n, p, x);
    roots.push_back(root);
    return (int)roots.size() - 1;
  }

  S get(int v, int p) const {
    return get_internal(roots[v], 0, _n, p);
  }

  S prod(int v, int l, int r) const {
    if (l == r) return e();
    return prod_internal(roots[v], 0, _n, l, r);
  }

  S all_prod(int v) const {
    return _n > 0 ? nodes[roots[v]].val : e();
  }

  template <class F> int max_right(int v, int l, F f) const {
    if (l == _n) return _n;
    S sm = e();
    return max_right_internal(roots[v], 0, _n, l, sm, f);
  }

  template <bool (*f)(S)> int max_right(int v, int l) const {
    return max_right(v, l, [](S x) { return f(x); });
  }

  template <class F> int min_left(int v, int r, F f) const {
    if (r == 0) return 0;
    S sm = e();
    return min_left_internal(roots[v], 0, _n, r, sm, f);
  }

  template <bool (*f)(S)> int min_left(int v, int r) const {
    return min_left(v, r, [](S x) { return f(x); });
  }
};
