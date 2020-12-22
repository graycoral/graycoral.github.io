#include <iostream>
#include <vector>

using namespace std;


// 방문자 인터페이스 
template<typename T> struct IVisitor
{
	virtual ~IVisitor() {}
	virtual	void visit(T& elem) = 0;
};

template<typename T>
class TwiceVisitor : public IVisitor
{
public:
	virtual void visit(T& elem)	{		elem  *= 2;	}
};

template<typename T>
class ShowVisitor : public IVisitor
{
public:
	virtual void visit(T& elem)	{		cout << elem << endl;}
}

// 방문자의 대상 인터페이스 
template<typename T>
struct IAccept
{
	virtual void accept(IAccept<T*> p) = 0;
	virtual ~IAccept() {}
};


template<typename T> class List : public list<T>, public IAcceptor<T>
{
public:
    using list<T>::list;
    virtual void accept(IVisitor<T>* p)
    {
        // 모든 요소를 방문자에게 전달.
        for(auto& e: *this){
            p->visit(e);
        }
    }
};

int main()
{
	List<int> s= {1,2,3,4,5,6,7,8,9,10};

    TwiceVisitor<int> tv;// 방문자
    s.accept(&tv);

    ShowVisitor<int> sv; // 방문자
    s.accept(&tv);
}