#include "primer/trie.h"
#include <memory>
#include <stack>
#include <string_view>
#include "common/exception.h"

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  // throw NotImplementedException("Trie::Get is not implemented.");

  // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
  // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
  // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
  // Otherwise, return the value.

  auto p = root_;
  for (const auto &c : key) {
    if (p == nullptr) {
      return nullptr;
    }
    p = p->GetChild(c);
  }
  if (p == nullptr) {
    return nullptr;
  }
  auto t = dynamic_cast<const TrieNodeWithValue<T> *>(p.get());
  if (t == nullptr) {
    return nullptr;
  }
  return t->value_.get();
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  // throw NotImplementedException("Trie::Put is not implemented.");

  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should create a new `TrieNodeWithValue`.
  size_t i = 0;
  std::shared_ptr<TrieNode> root = (root_ != nullptr) ? (root_->Clone()) : (std::make_shared<TrieNode>());
  auto p = root;
  std::shared_ptr<TrieNode> q = root;
  auto key_len = key.size();
  for (; i < key_len; i++) {
    auto c = key[i];
    if (p != nullptr) {
      if (auto t = p->GetChild(key[i]); t != nullptr) {
        if (i == key_len - 1 && !t->is_value_node_) {
          q->PutChild(c, std::make_unique<TrieNodeWithValue<T>>(t->children_, std::make_shared<T>(value)));
        } else {
          q->PutChild(c, t->Clone());
        }
        p = std::move(t);
      } else {
        if (i == key_len - 1) {
          q->PutChild(c, std::make_unique<TrieNode>());
        } else {
          q->PutChild(c, std::make_unique<TrieNodeWithValue<T>>(std::make_shared<T>(value)));
        }
        p = nullptr;
      }
    } else {
      if (i == key_len - 1) {
        q->PutChild(c, std::make_unique<TrieNode>());
      } else {
        q->PutChild(c, std::make_unique<TrieNodeWithValue<T>>(std::make_shared<T>(value)));
      }
    }
    if (i == 0) {
      root = q;
    }
    q = std::const_pointer_cast<TrieNode>(q->children_[c]);
  }
  q->is_value_node_ = true;
  return Trie(root);
}

auto Trie::Remove(std::string_view key) const -> Trie {
  throw NotImplementedException("Trie::Remove is not implemented.");

  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
  if (this->Get<class T>(key) == nullptr) {
    return *this;
  }
  std::shared_ptr<TrieNode> root = root_->Clone();
  auto p = root;
  std::stack<std::shared_ptr<TrieNode>> stk;
  for (const auto &c : key) {
    stk.push(p);
    p = p->GetChild(c);
  }
  p = p->Clone();
  p->is_value_node_ = false;
  std::dynamic_pointer_cast<TrieNodeWithValue<class T>>(p)->value_ = nullptr;
  if (p->children_.empty()) {
    p = nullptr;
  }
  while (!stk.empty()) {
    auto q = stk.top()->Clone();
    auto c = key[stk.size() - 1];
    if (p == nullptr) {
      if (q->children_.size() == 1 && !q->is_value_node_) {
        q = nullptr;
      } else {
        q->children_.erase(c);
      }
    } else {
      q->children_[c] = std::move(p);
    }
    p = std::shared_ptr<TrieNode>(std::move(q));
    stk.pop();
  }
  root = std::move(p);
  return Trie(root);
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
