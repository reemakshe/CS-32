

#ifndef Map_h
#define Map_h

#include <string>

using KeyType = std::string;
using ValueType = double;

class Map
{
  public:
    //Default constructor
    Map();
    
    //Destructor
    ~Map();
    
    //Copy constructor
    Map(const Map& other);
    
    //Equal assignment operator
    Map& operator=(const Map& other);

    //Returns whether the list is empty
    bool empty() const;

    //Returns the size of the list
    int size() const;

    //Attempts to insert key value pair into list, and returns whether insertion was successful
    bool insert(const KeyType& key, const ValueType& value);

    //Attempts to update value data member corresponding to the key and returns whether update was succesful
    bool update(const KeyType& key, const ValueType& value);

    //Either inserts the key value pair into the list, if the key is not already in the list
    //If the key is already in the list, update the corresponding value
    bool insertOrUpdate(const KeyType& key, const ValueType& value);

    //Attempts to erase item from list with that key value, and returns whether erasure was successful
    bool erase(const KeyType& key);

    //Returns whether the list contains an item with that key
    bool contains(const KeyType& key) const;

    //Sets the value parameter equal to the corresponding value data member for that key and returns true
    //If key is not present in list, does not change value parameter and returns false
    bool get(const KeyType& key, ValueType& value) const;

    //Sets the key and value parameters equal to the key and value data members of an item in the list,
    //if i is between 0 and the size of the list; otherwise, returns false and doesn't change the parameters
    bool get(int i, KeyType& key, ValueType& value) const;

    //Swap the contents and sizes of the two maps
    void swap(Map& other);
    
    //Prints all the key and value members of the items in the list
    void dump() const;

  private:
    //Struct that contains a key and value data member, as well as pointers to the next and previous values
    //in the list
    struct Node
    {
        KeyType m_key;
        ValueType m_value;
        Node* next;
        Node* prev;
    };
    Node* head;
    int m_size;

};

//Combines the two maps, adding each unique pair in both maps into the result map
//If there is a duplicate key with nonmatching values, do not add that key to the result map and return false
//If there is a duplicate key with matching values, add that key to the result map
bool combine(const Map& m1, const Map& m2, Map& result);

//In result, add each key from m, each with a value different from the value it had in m
void reassign(const Map& m, Map& result);

#endif
