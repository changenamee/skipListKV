//
// Created by sky on 2021/7/24.
//

#include "skipList.h"
template<typename K,typename V>
skipList<K,V>::skipList(int level){
    this->maxLevel = level;
    this->curLevel = 0;
    this->nodeCnt = 0;
    K k;
    V v;
    this->head = new Node<K,V>(k,v,maxLevel);  //初始化头结点
}

template<typename K,typename V>
skipList<K,V>::~skipList(){
    if (outFile.is_open()) {
        outFile.close();
    }
    if (inFile.is_open()) {
        inFile.close();
    }
    delete head;
}

//随机分配到第几层
template<typename K,typename V>
int skipList<K,V>::getRandomLevel() {
    int level = 1;
    //百分之五十上一层
    while(rand()%2)
        ++level;
    level = (level<maxLevel)?level:maxLevel;
    return level;
}
//创建新节点
template<typename K,typename V>
Node<K,V>* skipList<K,V>::createNode(K k, V v, int level) {
    Node<K,V>* node = new Node<K,V>(k,v,level);
    return node;
}
template<typename K,typename V>
bool skipList<K,V>::insertNode(K k,V v){
    mtx.lock();
    Node<K,V>* cur = this->head;

    Node<K,V>* update[maxLevel+1];
    //.....乘层数，不是sizeof(层数)
    memset(update,0,sizeof(Node<K,V>*)*(maxLevel+1));

    //先查找比k小的节点
    //size_t......会造成死循环
    for(int i=curLevel;i>=0;--i){
        while(cur->next[i]!= nullptr && cur->next[i]->getKey()<k)
            cur = cur->next[i];
        update[i] = cur;
    }
    //下一个
    cur = cur->next[0];
    //判断是否存在
    if(cur!= nullptr && cur->getKey() == k){
        std::cout<<"the key has been there"<<std::endl;
        mtx.unlock();
        return false;
    }
    //不存在
    if(cur== nullptr || cur->getKey()!=k){
        int randomLevel = getRandomLevel();
        //初始化上层
        if(randomLevel>curLevel){
            for(int i=curLevel+1;i<=randomLevel;++i)
                update[i] = head;
            curLevel = randomLevel;
        }

        Node<K,V>* node = createNode(k,v,randomLevel);

        for(int i=0;i<=randomLevel;++i){
            node->next[i] = update[i]->next[i];
            update[i]->next[i] = node;
        }
        std::cout<<"insert:"<<k<<":"<<v<<std::endl;
        ++nodeCnt;
    }
    mtx.unlock();
    return true;
}

template<typename K,typename V>
bool skipList<K,V>::findNode(K k) {
    Node<K,V>* cur = head;

    for(int i=curLevel;i>=0;--i){
        while(cur->next[i]!=nullptr && cur->next[i]->getKey()<k)
            cur = cur->next[i];
    }
    cur =  cur->next[0];
    if(cur== nullptr || cur->getKey()!=k){
        std::cout<<k<<" is not there"<<std::endl;
        return false;
    }
    std::cout<<k <<" found:"<<cur->getVal()<<std::endl;
    return true;
}

template<typename K,typename V>
bool skipList<K,V>::deleteNode(K k) {
    mtx.lock();

    Node<K,V>* cur = this->head;
    Node<K,V>* update[maxLevel+1];
    memset(update,'\0',sizeof(Node<K,V>*)*(maxLevel+1));


    for(int i=curLevel;i>=0;--i){
        while(cur->next[i]!= nullptr && cur->next[i]->getKey()<k)
            cur = cur->next[i];
        update[i] = cur;
    }

    //check
    cur = cur->next[0];
    if(cur== nullptr || cur->getKey()!=k){
        std::cout<<k<<" is not there"<<std::endl;
        mtx.unlock();
        return false;
    }
    //delete
    for(int i=0;i<=curLevel;++i){
        if(update[i]->next[i]->getKey() != k)
            break;
        update[i]->next[i] = cur->next[i];
    }
    //
    while(curLevel>0 && head->next[curLevel]== nullptr)
        --curLevel;
    std::cout<<k<<" has been deleted"<<std::endl;
    --nodeCnt;
    mtx.unlock();
    return true;
}

template<typename K,typename V>
bool skipList<K,V>::saveFile() {
    outFile.open(DATAFILE);

    Node<K,V>* cur = this->head->next[0];
    while(cur!= nullptr){
        outFile<<cur->getKey()<<":"<<cur->getVal()<<"\n";
        std::cout<<cur->getKey()<<":"<<cur->getVal()<<"\n";
        cur = cur->next[0];
    }
    std::cout<<"save file\n";
    //outFile.flush();
    outFile.close();
    return true;
}

template<typename K,typename V>
bool skipList<K,V>::isValid(std::string s) {
    if(s.empty())   return false;
    if(s.find(flag)==std::string::npos)   return false;
    return true;
}

template<typename K,typename V>
bool skipList<K,V>::getKVfromFile(std::string* key, std::string* val, std::string& line) {
    if(!isValid(line))
        return false;
    *key = line.substr(0,line.find(flag));
    *val = line.substr(line.find(flag)+1);
}
template<typename K,typename V>
bool skipList<K,V>::loadFile() {
    inFile.open(DATAFILE);

    std::string line;
    std::string* key = new std::string();
    std::string* val = new std::string();
    while(getline(inFile,line)){
        getKVfromFile(key,val,line);
        if(key->empty() || val->empty())
            continue;
        insertNode(*key,*val);
    }
    inFile.close();
}

template<class K,class V>
void skipList<K,V>::display(){
    for(int i=0;i<curLevel;++i){
        Node<K,V>* cur = this->head->next[i];
        std::cout<<"line:"<<i<<"\n";
        while(cur!=nullptr){
            std::cout<<cur->getKey()<<":"<<cur->getVal()<<"\t";
            cur = cur->next[i];
        }
        std::cout<<"\n";
    }
}