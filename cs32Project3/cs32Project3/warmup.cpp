#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
using namespace std;

  // Remove the odd integers from li.
  // It is acceptable if the order of the remaining even integers is not
  // the same as in the original list.
void removeOdds(list<int>& li)
{
    list<int>::iterator it = li.begin();

    while (it != li.end())
    {
        if ((*it) % 2 != 0)
        {
            li.erase(it);
            it--;
        }
        it++;
    }
}

void removeOdds(vector<int>& v)
{
    vector<int>::iterator it = v.begin();

    while (it != v.end())
    {
        if ((*it) % 2 != 0)
        {
            v.erase(it);
            it--;
        }
        else
            it++;
    }
}


void test()
{
    int a[8] = { 2, 8, 5, 6, 7, 3, 4, 1 };
    list<int> x(a, a+8);  // construct x from the array
    assert(x.size() == 8 && x.front() == 2 && x.back() == 1);
    removeOdds(x);
    assert(x.size() == 4);
    vector<int> v(x.begin(), x.end());  // construct v from x
    sort(v.begin(), v.end());
    int expect[4] = { 2, 4, 6, 8 };
    for (int k = 0; k < 4; k++)
        assert(v[k] == expect[k]);
}

//vector<int> destroyedOnes;
//
//class Movie
//{
//  public:
//    Movie(int r) : m_rating(r) {}
//    ~Movie() { destroyedOnes.push_back(m_rating); }
//    int rating() const { return m_rating; }
//  private:
//    int m_rating;
//};


//void removeBad(list<Movie*>& li)
//{
//    list<Movie*>::iterator it = li.begin();
//
//    while (it != li.end())
//    {
//        if ((*it)->rating() < 50)
//        {
//            Movie* temp = *it;
//            li.erase(it);
//            delete temp;
//            it = li.begin();
//        }
//        else
//            it++;
//    }
//}
//
//void test()
//{
//    int a[8] = { 85, 80, 30, 70, 20, 15, 90, 10 };
//    list<Movie*> x;
//    for (int k = 0; k < 8; k++)
//        x.push_back(new Movie(a[k]));
//    assert(x.size() == 8 && x.front()->rating() == 85 && x.back()->rating() == 10);
//    removeBad(x);
//    assert(x.size() == 4 && destroyedOnes.size() == 4);
//    vector<int> v;
//    for (list<Movie*>::iterator p = x.begin(); p != x.end(); p++)
//    {
//        Movie* mp = *p;
//        v.push_back(mp->rating());
//    }
//      // Aside:  In C++11, the above loop could be
//      //     for (auto p = x.begin(); p != x.end(); p++)
//      //     {
//      //         Movie* mp = *p;
//      //         v.push_back(mp->rating());
//      //     }
//      // or
//      //     for (auto p = x.begin(); p != x.end(); p++)
//      //     {
//      //         auto mp = *p;
//      //         v.push_back(mp->rating());
//      //     }
//      // or
//      //     for (Movie* mp : x)
//      //         v.push_back(mp->rating());
//      // or
//      //     for (auto mp : x)
//      //         v.push_back(mp->rating());
//    sort(v.begin(), v.end());
//    int expect[4] = { 70, 80, 85, 90 };
//    for (int k = 0; k < 4; k++)
//        assert(v[k] == expect[k]);
//    sort(destroyedOnes.begin(), destroyedOnes.end());
//    int expectGone[4] = { 10, 15, 20, 30 };
//    for (int k = 0; k < 4; k++)
//        assert(destroyedOnes[k] == expectGone[k]);
//    for (list<Movie*>::iterator p = x.begin(); p != x.end(); p++)
//        delete *p;
//}

//
//void removeBad(vector<Movie*>& v)
//{
//    vector<Movie*>::iterator it = v.begin();
//
//    while (it != v.end())
//    {
//        if ((*it)->rating() < 50)
//        {
//            Movie* temp = *it;
//            v.erase(it);
//            delete temp;
//            it = v.begin();
//        }
//        else
//            it++;
//    }
//}
//
//void test()
//{
//    int a[8] = { 85, 80, 30, 70, 20, 15, 90, 10 };
//    vector<Movie*> x;
//    for (int k = 0; k < 8; k++)
//        x.push_back(new Movie(a[k]));
//    assert(x.size() == 8 && x.front()->rating() == 85 && x.back()->rating() == 10);
//    removeBad(x);
//    assert(x.size() == 4 && destroyedOnes.size() == 4);
//    vector<int> v;
//    for (int k = 0; k < 4; k++)
//        v.push_back(x[k]->rating());
//    sort(v.begin(), v.end());
//    int expect[4] = { 70, 80, 85, 90 };
//    for (int k = 0; k < 4; k++)
//        assert(v[k] == expect[k]);
//    sort(destroyedOnes.begin(), destroyedOnes.end());
//    int expectGone[4] = { 10, 15, 20, 30 };
//    for (int k = 0; k < 4; k++)
//        assert(destroyedOnes[k] == expectGone[k]);
//    for (vector<Movie*>::iterator p = x.begin(); p != x.end(); p++)
//        delete *p;
//}

//
//const int MAGIC = 11223344;
//
//void test()
//{
//    bool allValid = true;
//
//    vector<int> v1(5, MAGIC);
//    int k = 0;
//    for ( ; k != v1.size(); k++)
//    {
//        if (v1[k] != MAGIC)
//        {
//            cout << "v1[" << k << "] is " << v1[k] << ", not " << MAGIC <<"!" << endl;
//            allValid = false;
//        }
//        if (k == 2)
//        {
//            for (int i = 0; i < 5; i++)
//                v1.push_back(MAGIC);
//        }
//    }
//    if (allValid  &&  k == 10)
//        cout << "Passed test 1" << endl;
//    else
//        cout << "Failed test 1" << endl;
//
//    allValid = true;
//    list<int> l1(5, MAGIC);
//    k = 0;
//    for (list<int>::iterator p = l1.begin(); p != l1.end(); p++, k++)
//    {
//        if (*p != MAGIC)
//        {
//            cout << "Item# " << k << " is " << *p << ", not " << MAGIC <<"!" << endl;
//            allValid = false;
//        }
//        if (k == 2)
//        {
//            for (int i = 0; i < 5; i++)
//                l1.push_back(MAGIC);
//        }
//    }
//    if (allValid  &&  k == 10)
//        cout << "Passed test 2" << endl;
//    else
//        cout << "Failed test 2" << endl;
//
//    allValid = true;
//    vector<int> v2(5, MAGIC);
//    k = 0;
//    for (vector<int>::iterator p = v2.begin(); p != v2.end(); p++, k++)
//    {
//        if (k >= 20)  // prevent infinite loop
//            break;
//        if (*p != MAGIC)
//        {
//            cout << "Item# " << k << " is " << *p << ", not " << MAGIC <<"!" << endl;
//            allValid = false;
//        }
//        if (k == 2)
//        {
//            for (int i = 0; i < 5; i++)
//                v2.push_back(MAGIC);
//        }
//    }
//    if (allValid  &&  k == 10)
//        cout << "Passed test 3" << endl;
//    else
//        cout << "Failed test 3" << endl;
//}


int main()
{
    test();
    cout << "Passed" << endl;
}


