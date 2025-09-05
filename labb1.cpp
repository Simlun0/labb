#include<cstdint>
#include <iostream>


struct Node{
    Node* left;
    Node* right;
    int32_t value;
    bool has_value;
    Node() : left(nullptr), right(nullptr), value(0) ,has_value(false){}
    Node(Node* l, Node* r) : left(l), right(r), value(0) ,has_value(false){}
    Node(Node* l, Node* r, int32_t v, bool hv): left(l), right(r),value(v), has_value(hv) {}
};


struct Array {
    Node* root;
    Array() : root(nullptr) {}
    Array(Node* r) : root(r) {}


    void destroy(uint32_t lastChangedIndex)
    {
        Node* cur = root;
        Node* temp;
        for (int bit = 31; bit >= 0; --bit)
        {
            Node* temp = cur;
            uint32_t b = (lastChangedIndex >> bit) & 1;
            cur = (b == 0) ? cur->left : cur->right;
            delete temp;
        }
        delete cur;
        int b;
    }


    static Array newArray() {return Array(nullptr);}


    static int32_t get(Array& arr, uint32_t index)
    {
        Node* cur = arr.root;
        for (int bit = 31; bit >= 0; --bit)
        {
            if (cur == nullptr) return 0;
            uint32_t b = (index >> bit) & 1;
            cur = (b == 0) ? cur->left : cur->right;
        }
        return cur->has_value ? cur->value : 0;
    }

    // set (arr, val, index)
    static Node* set_helper(Node* node, int32_t val,uint32_t index, int bit)
    {
        if (bit < 0)
        {
            return new Node(nullptr, nullptr,val,true);
        }
        uint32_t b = (index >> bit) & 1;
        Node* left = node ? node->left : nullptr;
        Node* right = node ? node->right : nullptr;

        if (b == 0)
        {
            Node* newLeft = set_helper(left,val,index,bit - 1);
            return new Node(newLeft,right);
        }
        else
        {
            Node* newRight = set_helper(right,val,index, bit - 1);
            return new Node(left,newRight);
        }

    }

    static Array set (const Array& arr, int32_t val, uint32_t index)
    {
        Node* newRoot = set_helper(arr.root, val,index, 31);
        return Array(newRoot); // returns the newest array
    }
};


struct LinkedListNode {
    Array arr;
    uint32_t lastChangedIndex;
    LinkedListNode* next;
    
    LinkedListNode(const Array& a, uint32_t i, LinkedListNode* n) : arr(a), lastChangedIndex(i), next(n) {}

    ~LinkedListNode()
    {
        arr.Array::destroy(lastChangedIndex);
    }
};


struct PersistentArray {
    LinkedListNode* root = nullptr;


    PersistentArray()
    {
        push(Array::newArray(),0);
    }


    void push(const Array& array, uint32_t i)
    {
        root = new LinkedListNode(array, i, root);
    }


    void pop()
    {
        if(root->next)
        {
            LinkedListNode* discard = root;
            root = root->next;
            delete discard;
        }
    }


    void set(uint32_t i, int32_t v)
    {
        Array next = Array::set(root->arr,v,i);
        push(next, i);
    }

    int32_t get(uint32_t i)
    {
        return Array::get(root->arr,i);
    }

    void unset()
    {
        pop();
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    PersistentArray arr;

    int Q;
    if (!(std::cin >> Q)) return 0;

    for (int q = 0; q < Q; ++q)
    {
        std::string op;
        std::cin >> op;
        if (op == "set")
        {
            uint32_t i;
            int32_t v;

            std::cin >> i >> v;
            arr.set(i,v);
        }
        else if (op == "get")
        {
            uint32_t i;
            std::cin >> i;
            std::cout << arr.get(i) << "\n";
        }
        else if(op == "unset")
        {
            arr.unset();
        }
        else // ignore unknown op
        {
            std::string rest ;
            std::getline(std::cin, rest);
        }
    }
}