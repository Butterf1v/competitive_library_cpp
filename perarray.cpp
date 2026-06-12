#include <vector>
#include <algorithm>

template <typename T>
class perarray {
private:
  struct Node {
    int child[4];
    T val;
    Node() {
      child[0] = child[1] = child[2] = child[3] = -1;
      val = T();
    }
  };
  std::vector<Node> nodes;
  std::vector<int> roots;
  int n;
  int max_shift;

  int build(int shift, int idx_base, const std::vector<T>& v) {
    if (idx_base >= (int)v.size()) return -1;
    int cur = nodes.size();
    nodes.emplace_back();
    if (shift < 0) {
      nodes[cur].val = v[idx_base];
      return cur;
    }
    for (int i = 0; i < 4; ++i) {
      nodes[cur].child[i] = build(shift - 2, idx_base | (i << shift), v);
    }
    return cur;
  }

  int update(int cur, int shift, int idx, const T& value) {
    int res = nodes.size();
    nodes.emplace_back();
    if (cur != -1) {
      nodes[res] = nodes[cur];
    }
    if (shift < 0) {
      nodes[res].val = value;
      return res;
    }
    int i = (idx >> shift) & 3;
    nodes[res].child[i] = update(nodes[res].child[i], shift - 2, idx, value);
    return res;
  }

  const T& get_internal(int ver, int idx) const {
    int cur = roots[ver];
    int shift = max_shift;
    while (shift >= 0 && cur != -1) {
      int i = (idx >> shift) & 3;
      cur = nodes[cur].child[i];
      shift -= 2;
    }
    if (cur == -1) {
      static const T default_value = T();
      return default_value;
    }
    return nodes[cur].val;
  }

public:
  class VersionProxy {
  private:
    const perarray& pa;
    int ver;
  public:
    VersionProxy(const perarray& pa, int ver) : pa(pa), ver(ver) {}
    const T& operator[](int idx) const {
      return pa.get_internal(ver, idx);
    }
  };

  perarray(int elements_count) : n(elements_count) {
    int shift = 0;
    while ((1 << shift) < n) {
      shift += 2;
    }
    max_shift = shift;
    nodes.emplace_back();
    roots.push_back(0);
  }

  perarray(const std::vector<T>& v) : n(v.size()) {
    int shift = 0;
    while ((1 << shift) < n) {
      shift += 2;
    }
    max_shift = shift;
    if (n == 0) {
      nodes.emplace_back();
      roots.push_back(0);
    } else {
      int rt = build(max_shift, 0, v);
      roots.push_back(rt);
    }
  }

  int set(int base_version, int index, const T& value) {
    int old_rt = roots[base_version];
    int new_rt = update(old_rt, max_shift, index, value);
    roots.push_back(new_rt);
    return roots.size() - 1;
  }

  VersionProxy operator[](int ver) const {
    return VersionProxy(*this, ver);
  }

  void reserve(size_t capacity) {
    nodes.reserve(capacity);
  }
};
