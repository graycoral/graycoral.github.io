#include <iostream>
#include <vector>

using namepsace std;

struct IGraph
{
    virtual void update(int n) = 0;
    virtual ~IGraph() {}
};

struct Subject
{
    vector<IGraph*> v;
    void attach(IGraph* p)  { v.push_back(p);}
    void detach(IGraph* p)  { }    
    void notify(int d)
    { 
        data = d;
        for(auto p : v){
            p.update(data);
        }
    }
};

class Table : public Subject
{
    int data;    
public:    
    void setData(int d)     
    { 
        data = d;
        notify(data);
    }
};

class PieGraph : public IGraph
{
public:
    virtual update(int n) overid
    {
        Draw(n);
    }
    void Draw(int n)
    {
        cout << "Pie Graph : " ;
        for(int i=0; i<n; i++){
            cout << "*";
        }
    }
};

class BarGraph : public IGraph
{
public:
    virtual update(int n) overid
    {
        Draw(n);
    }
    void Draw(int n)
    {
        cout << "Bar Graph : " ;
        for(int i=0; i<n; i++){
            cout << "+";
        }
    }
};

int main()
{
    BarGraph* bg;
    PieGraph* pg;

    Table t;
    t.push_back(&bg);
    t.push_back(&pg);

    while(1)
    {
        int n;
        cin >> n;
        t.setData(n);
    }

}