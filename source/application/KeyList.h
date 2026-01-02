// KeyList.h
#pragma once
#include <list>
#include <algorithm>

struct KeyPair {
    int senderId;
    int keyId;
};

class KeyList {
public:
    // 插入（可选：去重）
    bool add(int senderId, int keyId, bool dedup=true) {
        if (dedup && contains(senderId, keyId)) return false;
        data_.push_back({senderId, keyId});
        return true;
    }
    // 查找
    bool contains(int senderId, int keyId) const {
        return std::any_of(data_.begin(), data_.end(),
            [&](const KeyPair& p){ return p.senderId==senderId && p.keyId==keyId; });
    }
    // 删除一个
    bool erase(int senderId, int keyId) {
        auto n = data_.size();
        data_.remove_if([&](const KeyPair& p){ return p.senderId==senderId && p.keyId==keyId; });
        return data_.size() != n;
    }
    // 按 sender 删除
    size_t eraseBySender(int senderId) {
        auto n = data_.size();
        data_.remove_if([&](const KeyPair& p){ return p.senderId==senderId; });
        return n - data_.size();
    }
    // 其它工具
    size_t size() const { return data_.size(); }
    void clear() { data_.clear(); }

    // 迭代访问
    const std::list<KeyPair>& items() const { return data_; }

private:
    std::list<KeyPair> data_;  // 双向链表
};
