#include <iostream>
#include <string>

using namespace std;

template<typename T> struct Node
{
    T   data;
    Node* next;
    Node( const T& d, Node* n) : data(d), next(n) {}
};

template<typename T>
class slist_iterator
{
    Node<T>* current = 0;
public:
    slist_iterator(Node<T>* p = 0)  : current(p) {}
   inline slist_iterator& operator++()
    {
        current = current->next;
        return *this;
    }
    inline T&  operator*() { return current->data; }
};

template<typename T> class slist
{
    Node<T>* head = 0;
public:
    void push_front(const T& n) { head = new Node<T>(n, head);}
    T  front()                  { return head->data;}
    slist_iterator<T>* begin()
    {
         return slist_iterator<T>( head);
    }
};

int main()
{
    slist<int> s;

    s.push_front(10);
    s.push_front(20);
    s.push_front(30);
    s.push_front(40);

    // SlisEnumerator<int>* p = s.GetEunmerator(); 내부 자료 구조에상관 없이 자료를 열거 하고 싶다.
    int* p = s.begin();



}