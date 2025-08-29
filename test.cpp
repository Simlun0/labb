#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

class Vector
{
    int* root= nullptr;
    int m_Size = 0;
    int m_Capacity = 0;

    public:
    void reallocate(int newCapacity)
    {
        int* temp = new int [newCapacity];
        m_Capacity *= 2;
        // for (int i = 0; i < m_Size; i++) 
        // {
        //     temp[i] = root[i];
        // }
        temp = std::move(root);
        delete[] root;
    }

        int Size() {return m_Size;}
        int Capacity() {return m_Capacity;}
        int operator[](int index) {return *(root+index);}
        void operator=(int value) {}

};

struct bestVektor;
bestVektor set(bestVektor &a, size_t i, size_t value);

struct twoInts {
    int index; 
    int value;

    twoInts(int i, int val) :index(i),value(val){}
};

struct bestVektor
{
    std::vector<int> Vec;
    std::vector<twoInts> Changes;

    void unset()
    {
        if (Changes.size())
        {
            twoInts lastChange = Changes[Changes.size()-1];
            Changes.pop_back();
            set(*this, lastChange.index, lastChange.value);
        }
    }

};



bestVektor set(bestVektor &a, size_t i, size_t value)
{
    if (a.Vec.size() >= i)
    {
        int oldSize = a.Vec.size();
        a.Vec.reserve(i - a.Vec.size());
        for(int i = oldSize; i < a.Vec.size(); i++) {a.Vec[i]=0;}
    }
    a.Vec[i] = value;
    a.Changes.push_back(twoInts(i,value));
    return a;
}

int get(bestVektor &a, size_t i)
{
    if (i >= a.Vec.size()) {return 0;}
    return a.Vec[i];
}




int main()
{
    bestVektor a;
    set(a, 0, 1);
    set(a, 1, 2);
    set(a, 2, 3);
    std::cout << get(a,1) << std::endl;
    set(a, 1, 10);
    a.unset();
    std::cout << get(a,1) << std::endl;
}

