#include <iostream>
#include <vector>

using namespace std;

#if 0
template<typename T> struct Node
{
    T   data;
    Node* next;
    Node( const T& d, Node* n) : data(d), next(n) {}
};

/*
// 반복자의 규칙- C#1.0
struct IEnumerator
{
    virtual ~IEnumerator() {}
    virtual bool MoveNext() = 0;
    virtual Object*    GetObject() = 0;
};
*/
// C# 2.0
template<typename T> struct IEnumerator
{
    virtual ~IEnumerator() {}
    virtual bool MoveNext() = 0;
    virtual T& GetObject() = 0; // 중요
};

// slist 반복자
template<typename T> class SlistEnumerator : public IEnumerator<T>
{
    Node<T>* curret = 0;
public:
    SlistEnumerator(Node<T>*p = 0) : curret(p) {}
    virtual bool MoveNext()
    {
        curret = curret->next;
        return curret;
    }
    virtual T&  GetObject() { return current->data; }
};

// 모든 컨테이너에서는 반복자를 꺼낼수 있어야 한다.
// 컨테이너가 지켜야 하는 인터페이스
template<typename T>struct IEnumerable
{
    virtual ~IEnumerable() {}
    virtual IEnumerable<T>* GetEunmerator() = 0;
};

template<typename T> class slist : public IEnumerable<T>
{
    Node<T>* head = 0;
public:
    virtual IEnumerable<T>* GetEunmerator()
    {
        return new SlistEnumerator<T>(head);
    }
    void push_front(const T& n) { head = new Node<T>(n, head);}
    T  front()                  { return head->data;}
};

int main()
{
    slist<int> s;

    s.push_front(10);
    s.push_front(20);
    s.push_front(30);
    s.push_front(40);

    // SlistEnumerator<int>* p = s.GetEunmerator(); 내부 자료 구조에상관 없이 자료를 열거 하고 싶다.
    IEnumerator<int>* p = s.GetEunmerator();

    cout << p->GetObject() << endl; // 40
    p->MoveNext();
    cout << p->GetObject() << endl; // 30
}
#endif
template<typename T> struct Node
{
    T   data;
    Node* next;
    Node( const T& d, Node* n) : data(d), next(n) {}
};

// 반복자 인터페이스 

template<typename T>
struct IEnumerator
{
    virtual ~IEnumerator() {}
    virtual T& GetObject() = 0;
    virtual bool MoveNext()= 0;
};

// sList 반복자
template<typename T> class SlistEnumerator : public IEnumerator<T>
{
    Node<T> curret = 0;
public:
    SlistEnumerator(Node<T>)
    virtual T& GetObject(){
        return curret;
    }
    virtual bool MoveNext() {
        curret = curret->next;
        return curret;
    }
};

// container의 인터페이스
template<typename T>
struct IEmunrable
{
    virtual ~IEmunrable() {}
    virtual IEnumerator<T> GetEunmerator() {} // 반복자를 리턴해한다. 인터페이스 ㅡ 모양으로     
};


template<typename T> class slist : public IEmunrable<T>
{
    Node<T> head =0;
public:
    void push_front(const T& n) { haed = new Node<T>(n,head);}
    T  front()                  { return head->data;}
    virtual IEnumerator<T> GetEunmerator()
    {
        return new SlistEnumerator<T>(head);
    }

};


int main()
{
    slist<int> s;

    s.push_front(10);
    s.push_front(20);
    s.push_front(30);
    s.push_front(40);

    // SlistEnumerator<int>* p = s.GetEunmerator(); 내부 자료 구조에상관 없이 자료를 열거 하고 싶다.
    IEnumerator<int>* p = s.GetEunmerator();

    cout << p->GetObject() << endl; // 40
    p->MoveNext();
    cout << p->GetObject() << endl; // 30
}


