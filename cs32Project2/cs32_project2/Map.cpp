#include "Map.h"
#include <iostream>

using namespace std;

Map::Map()
{
    //Create a new dummy node and set the head pointer equal to the address of that node
    head = new Node;
    //Set the next and previous pointers of the node equal to head itself because there are no other items
    head->next = head;
    head->prev = head;
    //Set the size of the list equal to 0 because there are no relevant items
    m_size = 0;
}

Map::~Map()
{
    //Create a Node pointer to traverse the list, starting at the node after head (dummy node)
    Node* p = head->next;
    //Create another Node pointer to store the pointer to the next item in the list
    Node* n;
    //While the pointer is not equal to head (the end of the list has been reached)
    while (p != head)
    {
        //Store the pointer to the next item in the list
        n = p->next;
        //Delete the current item in the list
        delete p;
        //Set the pointer to the current item equal to the next item in the list
        p = n;
    }
    //After all the other elements have been deleted, delete the dummy node
    delete head;
}

Map::Map(const Map& other)
{
    //Create a new head node and set its next and prev pointer
    head = new Node;
    head->next = head;
    head->prev = head;
    m_size = 0;
    
    //Traverse each item in the list and insert a new node into this map with the corresponding values for
    //each item in the other map
    Node* o = (other.head)->next;
    while (o != other.head)
    {
        this->insert(o->m_key, o->m_value);
        o = o->next;
    }

}

Map& Map::operator=(const Map& other)
{
    //If you are setting a variable equal to itself, you don't need to do anything
    if (&other == this)
        return *this;
    
    //Delete all the items currently stored in the list
    Node* p = head->next;
    Node* n;
    while (p != head)
    {
        n = p->next;
        delete p;
        p = n;
    }
    delete head;

    //Create a new dummy node and set head equal to it
    head = new Node;
    head->next = head;
    head->prev = head;
    m_size = 0;

    //Insert a new node into the list with the corresponding values in the other map
    Node* o = (other.head)->next;
    while (o != other.head)
    {
        this->insert(o->m_key, o->m_value);
        o = o->next;
    }

    //Return a reference to this map
    return *this;
}

bool Map::empty() const
{
    //Return whether the size of the map is 0
    return m_size == 0;
}

int Map::size() const
{
    //Return the size of the map
    return m_size;
}

//Adds pair to the end of the list
bool Map::insert(const KeyType& key, const ValueType& value)
{
    //Return false if the map already contains this key
    if (this->contains(key))
        return false;
    //Otherwise create a new node and set its data members equal to the parameters
    Node* p = new Node;
    p->m_key = key;
    p->m_value = value;
    //Set the new node's next and prev pointers which fit with it being added to the end of the list
    p->next = head;
    p->prev = head->prev;
    //Set head's prev (the previous last item in the list) next pointer equal to the address of the new node
    head->prev->next = p;
    //Set head's prev value to the address of new node (the new last item of the list)
    head->prev = p;
    //Increment size and return true because the value was successfully added
    m_size++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    //Check the key data member of each value in the list and if there is a key that matches
    //set its value data member equal to the value parameter and return true
    Node* p = head->next;
    while (p != head)
    {
        if (p->m_key == key)
        {
            p->m_value = value;
            return true;
        }
        p = p->next;
    }
    //Return false if no node was found with that key value
    return false;
    
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    //If the key exists already, it can be updated
    if (this->update(key, value))
        return true;
    //Otherwise, the key does not already exist and should be inserted
    else
        return this->insert(key, value);
}

bool Map::erase(const KeyType& key)
{
    //Traverse the list and break out of the list if a matching key data member is found
    Node* p = head->next;
    while (p != head)
    {
        if (p->m_key == key)
            break;
        p = p->next;
    }
    
    //If the pointer to the node is not equal to head, meaning a node with the key data member was found and
    //it broke out of the while loop before reaching head again
    if (p != head)
    {
        Node* delNode = p;
        //Set the prev pointer of the next node after the node to be deleted equal to the address of the
        //previous value to the node about to be deleted
        p->next->prev = delNode->prev;
        //Set the next pointer of the value previous to the node to be deleted equal to the node after the to
        //be deleted node
        p->prev->next = delNode->next;
        //Delete the matching node, decrement size, and return true
        delete delNode;
        m_size--;
        return true;
    }
    //Return false if a matching key data member was not found
    return false;
}

bool Map::contains(const KeyType& key) const
{
    //Traverse the list and return true if a node has a matching key data member
    Node* p = head->next;
    while (p != head)
    {
        if (p->m_key == key)
            return true;
        p = p->next;
    }
    //Return false if a matching data member was not found
    return false;
}

 bool Map::get(const KeyType& key, ValueType& value) const
{
    //Traverse the list and if a matching key data member is found, set the value parameter equal to the value
    //data member of that node and return true
    Node* p = head->next;
    while (p != head)
    {
        if (p->m_key == key)
        {
            value = p->m_value;
            return true;
        }
        p = p->next;
    }
    //Return false if a matching key was not found
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    //If i is out of the bounds of the size of the list return false
    if (i < 0 || i >= m_size)
        return false;
    
    //Traverse the list until reaching the ith member of the list
    Node* p = head->next;
    for (int j = 0; j < i; j++)
        p = p->next;
    
    //Set the key and value parameter equal to the key and value data members of this node and return true
    key = p->m_key;
    value = p->m_value;
    return true;
}

void Map::swap(Map& other)
{
    //Swap the pointers to the head dummy node for each list
    Node* tempHead = head;
    head = other.head;
    other.head = tempHead;
    
    //Swap the sizes of the lists
    int tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
}

void Map::dump() const
{
    //Traverse the list and print out the key and value data members of each node
    Node* p = head->next;
    while (p != head)
    {
        cerr << p->m_key << " ";
        cerr << p->m_value << endl;
        
        p = p->next;
    }
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
    //Set the default return value equal to true
    bool returnValue = true;
    //Create a new map that will store the resulting combination
    Map resultMap;
    
    //For each item in m1, insert the key value pair into resultMap
    for (int i = 0; i < m1.size(); i++)
    {
        KeyType k;
        ValueType v;
        m1.get(i, k, v);
        
        resultMap.insert(k, v);
    }

    //For each item in m2
    for (int j = 0; j < m2.size(); j++)
    {
        KeyType k;
        ValueType v;
        ValueType vRes;
        m2.get(j, k, v);
        
        //If m1 already has this item and the value in m1 does not equal the value in m2
        if (m1.get(k, vRes) && vRes != v)
        {
            //Set the return value to false because there are nonmatching duplicate keys
            returnValue = false;
            //Erase the key value pair from resultMap
            resultMap.erase(k);
        }
        //Otherwise, insert the key value pair into resultMap
        else
            resultMap.insert(k, v);
    }
    
    //Set result equal to resultMap and return the returnValue
    result = resultMap;
    return returnValue;
}

void reassign(const Map& m, Map& result)
{
    //Store the key and value data for the first item in the list
    KeyType firstKey;
    ValueType tempFirstVal;
    m.get(0, firstKey, tempFirstVal);
    
    //For each item in the list, except the last item, insert an item into result with the current key and
    //the value of the next item in the list
    for (int i = 0; i < m.size() - 1; i++)
    {
        int j = i + 1;
        
        KeyType k;
        ValueType v;
        m.get(i, k, v);
        
        KeyType k1;
        ValueType newVal;
        m.get(j, k1, newVal);
        
        result.insertOrUpdate(k, newVal);
    }
    
    //For the last item in the list, insert into result the key of the last item and the original value of
    //the first item in the list
    KeyType lastKey;
    ValueType lastVal;
    m.get(m.size() - 1, lastKey, lastVal);
    result.insertOrUpdate(lastKey, tempFirstVal);
    
    //Go through result and erase any item in result that has a key that is not in m
    for (int r = 0; r < result.size(); r++)
    {
        KeyType k;
        ValueType v;
        result.get(r, k, v);
        
        if (!m.contains(k))
            result.erase(k);
    }
}
