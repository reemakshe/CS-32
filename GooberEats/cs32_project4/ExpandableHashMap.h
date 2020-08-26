//
//  ExpandableHashMap.h
//  GooberEats
//
//  Created by Reema Kshetramade on 3/7/20.
//  Copyright © 2020 Reema Kshetramade. All rights reserved.
//
 
#ifndef ExpandableHashMap_h
#define ExpandableHashMap_h
 
#include <vector>
#include <list>
#include <string>
 
// ExpandableHashMap.h
 
// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.
 
template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
  public:
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    ~ExpandableHashMap();
    void reset();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);
 
      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;
 
      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }
    
     
      // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;
 
  private:
    struct Node
    {
        KeyType m_key;
        ValueType m_value;
        Node* next;
    };
    std::vector<Node*> hash_map;
    int m_buckets;
    double max_load;
    double m_num_items;
    double currentLoad();
    void rehash();
    
};
 
template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
    //Set all the starting values in the map to nullptr because they have no values associated with them
    for (int i = 0; i < 8; i++)
        hash_map.push_back(nullptr);
    
    //Set the maximum load factor based on what is passed in to the constructor
    if (maximumLoadFactor < 0)
        max_load = 0.5;
    else
        max_load = maximumLoadFactor;
    
    //Maps always start with 8 buckets and 0 items
    m_buckets = 8;
    m_num_items = 0;
}
 
template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
    //For each of the buckets in the map
    for (int i = 0; i < m_buckets; i++)
    {
        //If the location in the map is not nullptr, meaning it has a value associated with it
        if (hash_map[i] != nullptr)
        {
            //Delete all the nodes that are linked together starting at the Node pointed at by this bucket
            Node* p = hash_map[i];
            
            while (p != nullptr)
            {
                Node* temp = p->next;
                delete p;
                p = temp;
            }
        }
    }
}
 
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    //Delete all the old items in the buckets and set the values in the map to nullptr after
    for (int i = 0; i < m_buckets; i++)
    {
        if (hash_map[i] != nullptr)
        {
            Node* p = hash_map[i];
            
            while (p != nullptr)
            {
                Node* temp = p->next;
                delete p;
                p = temp;
            }
            
            hash_map[i] = nullptr;
        }
    }
    
    //Remove buckets from the vector until the size is 8
    while (hash_map.size() != 8)
        hash_map.pop_back();
}
 
template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    //Return number of items in the map
    return m_num_items;
}
 
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    //If adding one item to the map would exceed the maximum load factor, rehash the map
    if (((m_num_items + 1) / m_buckets) > max_load)
        rehash();
    
    //Check if there is any key value pair already associated in the map that matches this pair
    //and if there is reset what the value is to the value passed in
    ValueType* currVal = find(key);
    if (currVal != nullptr)
    {
        *currVal = value;
    }
    //If the key value is not already in the map
    else
    {
        //Generate location of key in the map by creating a hash number and then % with the number of buckets
        unsigned int hasher(const KeyType& k);
        unsigned int h = hasher(key);
        
        int loc = h % m_buckets;
        
        //If there is not already the address of a Node at that point, generate a new Node and set its values
        if (hash_map[loc] == nullptr)
        {
            hash_map[loc] = new Node;
            hash_map[loc]->m_key = key;
            hash_map[loc]->m_value = value;
            hash_map[loc]->next = nullptr;
        }
        //If there is already a Node at that address, go through the linked nodes until you reach the last one
        //(indicated by a Node with a next value of nullptr) and then add the Node there
        else
        {
            Node* p = hash_map[loc];
            
            while (p->next != nullptr)
                p = p->next;
            
            p->next = new Node;
            p = p->next;
            p->m_key = key;
            p->m_value = value;
            p->next = nullptr;
                
        }
        
        //Increment the number of items
        m_num_items++;
    }
    
    
}
 
template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    //Find the location of the key in the map using hash and %
    unsigned int hasher(const KeyType& key);
    unsigned int h = hasher(key);
    unsigned int loc = h % m_buckets;
    
    //If there is nothing at that position return nullptr
    if (hash_map[loc] == nullptr)
        return nullptr;
    
    //Otherwise, go through the Nodes linked at that point and if there is a matching key, return the address
    //of the value stored in the Node
    Node* p = hash_map[loc];
    
    while (p != nullptr)
    {
        if (p->m_key == key)
            return &(p->m_value);
        
        p = p->next;
    }
    
    //If the function has not returned yet, it means the key was not found, so return nullptr
    return nullptr;
    
}
 
template<typename KeyType, typename ValueType>
double ExpandableHashMap<KeyType, ValueType>::currentLoad()
{
    //Load is equal to the number of items divided by the number of buckets
    return m_num_items / m_buckets;
}
 
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::rehash()
{
    unsigned int hasher(const KeyType& key);
 
    //Create a new vector with twice the size of the original and set the values to nullptr
    std::vector<Node*> newHashMap;
    for (int i = 0; i < m_buckets * 2; i++)
        newHashMap.push_back(nullptr);
    
    //For each slot in the current map
    for (int i = 0; i < m_buckets; i++)
    {
        //If there is a value at that slot
        if (hash_map[i] != nullptr)
        {
            Node* p = hash_map[i];
            
            //For each node linked at that position
            while (p != nullptr)
            {
                //Generate a new location based on the new number of buckets
                unsigned int h = hasher(p->m_key);
                int newLoc = h % (m_buckets * 2);
                 
                //Add the key value pair to the map depending on whether there is something at that location
                if (newHashMap[newLoc] == nullptr)
                {
                    newHashMap[newLoc] = new Node;
                    (newHashMap[newLoc])->m_key = p->m_key;
                    (newHashMap[newLoc])->m_value = p->m_value;
                    (newHashMap[newLoc])->next = nullptr;
                }
                else
                {
                    Node* ptr = newHashMap[newLoc];
                    
                    while (ptr->next != nullptr)
                        ptr = ptr->next;
                    
                    ptr->next = new Node;
                    ptr = ptr->next;
                    ptr->m_key = p->m_key;
                    ptr->m_value = p->m_value;
                    ptr->next = nullptr;
                        
                }
                
                p = p->next;
            }
        }
    }
    
    //Delete the originally allocated values
    for (int i = 0; i < m_buckets; i++)
    {
        if (hash_map[i] != nullptr)
        {
            Node* p = hash_map[i];
            
            while (p != nullptr)
            {
                Node* temp = p->next;
                delete p;
                p = temp;
            }
        }
    }
    
    //Swap the contents of he old and new map and double the number of buckets
    hash_map.swap(newHashMap);
    m_buckets *= 2;
}
 
#endif /* ExpandableHashMap_h */


