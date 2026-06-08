#include <vector>

template <typename T>
class perarray {
private:
  struct node {
    node* child[32];
    T values[32];
    node() {
      for (int i = 0; i < 32; ++i) child[i] = nullptr;
    }
    node(const node& other) {
      for (int i = 0; i < 32; ++i) child[i] = other.child[i];
      for (int i = 0; i < 32; ++i) values[i] = other.values[i];
    }
  };

  std::vector<node*> roots;
  std::vector<node*> all_nodes;
  int max_shift;
  int size_;

  node* create_node() {
    node* n = new node();
    all_nodes.push_back(n);
    return n;
  }

  node* create_node(const node& other) {
    node* n = new node(other);
    all_nodes.push_back(n);
    return n;
  }

  node* build(int shift, int& idx, const std::vector<T>& vec) {
    if (idx >= vec.size()) return nullptr;
    node* res = create_node();
    if (shift == 0) {
      for (int i = 0; i < 32 && idx < vec.size(); ++i, ++idx) {
        res->values[i] = vec[idx];
      }
    } else {
      for (int i = 0; i < 32 && idx < vec.size(); ++i) {
        res->child[i] = build(shift - 5, idx, vec);
      }
    }
    return res;
  }

public:
  perarray(int n) {
    size_ = n;
    max_shift = 0;
    if (size_ > 0) {
      while ((size_ - 1) >> (max_shift + 5)) {
        max_shift += 5;
      }
    }
    std::vector<T> vec(size_);
    int idx = 0;
    roots.push_back(build(max_shift, idx, vec));
  }

  perarray(std::vector<T> init_vec) {
    size_ = init_vec.size();
    max_shift = 0;
    if (size_ > 0) {
      while ((size_ - 1) >> (max_shift + 5)) {
        max_shift += 5;
      }
    }
    int idx = 0;
    roots.push_back(build(max_shift, idx, init_vec));
  }

  ~perarray() {
    for (node* n : all_nodes) {
      delete n;
    }
  }

  T get(int version, int index) {
    node* curr = roots[version];
    int shift = max_shift;
    while (shift > 0) {
      curr = curr->child[(index >> shift) & 0x1f];
      shift -= 5;
    }
    return curr->values[index & 0x1f];
  }

  int set(int base_version, int index, T val) {
    node* curr = roots[base_version];
    node* new_root = create_node(*curr);
    node* p = new_root;
    int shift = max_shift;
    while (shift > 0) {
      int sub_idx = (index >> shift) & 0x1f;
      p->child[sub_idx] = create_node(*(curr->child[sub_idx]));
      p = p->child[sub_idx];
      curr = curr->child[sub_idx];
      shift -= 5;
    }
    p->values[index & 0x1f] = val;
    roots.push_back(new_root);
    return roots.size() - 1;
  }
};
