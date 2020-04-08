#include <iostream>
#include <vector>

using namespace std;

// 기반 클래스의 포인터

struct Object
{
    virtual ~Object() {}
};

class Point : public Object {};

struct Node
{
    // int   data;
    Object* data;
    Node* next;
    Node( int d, Node* n) : data(d), next(n) {}
};

class slist
{
    Node* head = 0;
public:
    void push_front(int n) { head = new Node(n, head);}
    int  front()           { return head->data;}
};

int main()
{
    slist s;

    // s.push_front(10);
    // s.push_front(20);
    // s.push_front(30);
    // s.push_front(40);
    // s.push_front(50);

    // int n = s.front();
    s.push_front(new Point);
    s.push_front(new Point);

    Point* p = static_cast<Point*> (s.front());
}

//template----------------------------

template<typename T>
struct Node
{
    // int   data;
    T* data;
    Node* next;
    Node( int d, Node* n) : data(d), next(n) {}
};

template<typename T>
class slist
{
    Node* head = 0;
public:
    void push_front(int n) { head = new Node<T>(n, head);}
    T  front()           { return head->data;}
};

int main()
{
    slist<int> s;

    s.push_front(10);
    s.push_front(20);
    s.push_front(30);
    s.push_front(40);

    int n = s.front();
}


//thin template------------------------------

struct Node
{
    void*   data;
    Node* next;
    Node( void* d, Node* n) : data(d), next(n) {}
};

class slistImp
{
    Node* head = 0;
public:
    void push_front(void* n) { head = new Node(n, head);}
    void*  front()           { return head->data;}
};

template<typename T>
class slist : public slistImp
{
public:
    inline void push_front(T n) { slistImp::push_front((void*)n);}
    inline T  front()           { return (T)(slistImp::front());}
}

int main()
{
    slist<int> s;

    s.push_front(10);
    s.push_front(20);
    s.push_front(30);
    s.push_front(40);
    s.push_front(50);

    int n = s.front();
}
