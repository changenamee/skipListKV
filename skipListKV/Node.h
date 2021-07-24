//
// Created by sky on 2021/7/24.
//

#ifndef SKIPLISTKV_NODE_H
#define SKIPLISTKV_NODE_H
#include <memory>
#include <mutex>
std::mutex mtx;
template<typename K,typename V>
class Node {
public:
    //根据随机层数构造
    Node(K,V,int);
    ~Node();
    K getKey() const;
    V getVal() const;
    void setVal(V val);
    //每一层的下一个节点
    Node<K,V> **next;
    int level;
private:
    K key;
    V val;
};


#endif //SKIPLISTKV_NODE_H
