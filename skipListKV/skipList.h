//
// Created by sky on 2021/7/24.
//

#ifndef SKIPLISTKV_SKIPLIST_H
#define SKIPLISTKV_SKIPLIST_H
#include "Node.h"
#include <iostream>
#include <string>
#include <fstream>
const char* DATAFILE = "data";
const char* flag=":";
template<typename K,typename V>
class skipList {
public:
    skipList(int level);
    ~skipList();
    int getRandomLevel();
    Node<K,V>* createNode(K,V,int);
    bool insertNode(K,V);
    bool findNode(K);
    bool deleteNode(K);
    bool saveFile();
    bool loadFile();
    void display();
private:
    bool getKVfromFile(std::string*,std::string*,std::string&);
    bool isValid(std::string);
    Node<K,V>* head;    //头结点
    int maxLevel;
    int curLevel;
    int nodeCnt;

    std::ofstream outFile;
    std::ifstream inFile;
};


#endif //SKIPLISTKV_SKIPLIST_H
