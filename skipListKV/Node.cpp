//
// Created by sky on 2021/7/24.
//

#include "Node.h"

template<typename K,typename V>
Node<K,V>::Node(K key, V val, int level){
    //0
    this->key = key;
    this->val = val;
    this->level = level;
    this->next = new Node<K,V>*[level+1];
    memset(this->next,0,sizeof(Node<K,V>*)*(level+1));
}

template<typename K,typename V>
Node<K,V>::~Node(){
    delete [] next;
}

template<typename K,typename V>
K Node<K,V>::getKey() const {
    return key;
}

template<typename K,typename V>
V Node<K,V>::getVal() const {
    return val;
}

template<typename K,typename V>
void Node<K,V>::setVal(V v){
    this->val = v;
}