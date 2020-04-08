//
//  ExpandableHashMap.h
//  GooberEats
//
//  Created by Fate End on 2020/3/6.
//  Copyright © 2020 Charles Zhou. All rights reserved.
//
#ifndef ExpandableHashMap_h
#define ExpandableHashMap_h
// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
    ExpandableHashMap(double maximumLoadFactor = 0.5):hashTable_size(8),m_size(0){
        if(maximumLoadFactor<0){
            m_loadFactor=0.5;
        }
        else{
            m_loadFactor=maximumLoadFactor;
        }
        hashTable=new std::list<Node>*[hashTable_size];
        for(int i=0;i<hashTable_size;i++){
            hashTable[i]=nullptr;
        }
    };
    ~ExpandableHashMap(){
        for(int i=0;i<hashTable_size;i++){
            if(hashTable[i]!=nullptr){
                delete hashTable[i];
            }
        }
        delete [] hashTable;
    };
    void reset(){
       for(int i=0;i<hashTable_size;i++){
           if(hashTable[i]!=nullptr){
               delete hashTable[i];
           }
       }
       delete [] hashTable;
       hashTable_size=8;
    };
    int size() const{
        return m_size;
    };
    void associate(const KeyType& key, const ValueType& value){
        int bucket=getBucketNumber(key);
        Node temp=Node(key,value);
        if(hashTable[bucket]==nullptr){
            hashTable[bucket]=new std::list<Node>;
            hashTable[bucket]->push_back(temp);
            m_size++;
        }
        else{
            typename std::list<Node>::iterator it;
            for(it = hashTable[bucket]->begin(); it!=hashTable[bucket]->end(); it++){
                if(it->key==key){
                    it->value=value;
                    return;
                }
            }
            hashTable[bucket]->push_back(temp);
            m_size++;
        }
        if(m_size>m_loadFactor*hashTable_size){
            hashTable_size=2*hashTable_size;
            std::list<Node> ** newHashTable=new std::list<Node>*[hashTable_size];
            for(int i=0;i<hashTable_size;i++){
                newHashTable[i]=nullptr;
            }
            for(int i=0;i<(hashTable_size/2);i++){
                if(hashTable[i]!=nullptr){
                    typename std::list<Node>::iterator it;
                    for(it = hashTable[i]->begin(); it!=hashTable[i]->end(); it++){
                        int bucketNum=getBucketNumber(it->key);
                        Node temp=Node(it->key,it->value);
                        if(newHashTable[bucketNum]==nullptr){
                            newHashTable[bucketNum]=new std::list<Node>;
                            newHashTable[bucketNum]->push_back(temp);
                        }
                        else{
                            newHashTable[bucketNum]->push_back(temp);
                        }
                        
                    }
                }
            }
            for(int i=0;i<hashTable_size/2;i++){
                if(hashTable[i]!=nullptr){
                    delete hashTable[i];
                }
            }
            delete [] hashTable;
            hashTable=newHashTable;
        }
    }

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const{
        int bucket=getBucketNumber(key);
        if(hashTable[bucket]==nullptr){
            return nullptr;
        }
        else{
            typename std::list<Node>::iterator it;
            for(it = hashTable[bucket]->begin(); it!=hashTable[bucket]->end(); it++){
                if(it->key==key){
                    return &(it->value);
                }
            }
            return nullptr;
        }
    }

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
        
    }

      // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    struct Node{
        Node(KeyType k,ValueType v):key(k),value(v){}
        KeyType key;
        ValueType value;
    };
    double m_loadFactor;
    int hashTable_size;
    int m_size;
    
    std::list<Node>**hashTable;
    unsigned int getBucketNumber(const KeyType& key) const
    {
    
        unsigned int hasher(const KeyType& k); // prototype
        unsigned int h = hasher(key);
        h=h%hashTable_size;
        return h;
        
    }
};

//typename std::list<Node>::iterator it;

//for(it = hashTable[i]->begin(); it!=hashTable[i]->end(); it++)



#endif /* ExpandableHashMap_h */
