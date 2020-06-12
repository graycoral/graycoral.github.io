#include <iostream>
#include <string>

using namespace std;

struct IGraph
{
    virtual ~IGraph() {}
    virtual void update(Subject* n) = 0;
};

struct Subject
{
    vector<IGraph*> v;
    void attach(IGraph* p)  {v.push_back(p);}
    void notify(int d) 
    {
        data = d;
        for(auto p: v){
            p->update(this);
        } 
    }
};

class Table : public Subject
{
    int data;
    // vector<IGraph*> v;
public:
    // void attach(IGraph* p)  {v.push_back(p);}
    void setData(int d) 
    {
        data = d;
        for(auto p: v){
            notify();
        } 
    }
    
};

class Table3D : public Subject
{
    int data;
    // vector<IGraph*> v;
public:
    // void attach(IGraph* p)  {v.push_back(p);}
    void setData(int d) 
    {
        data = d;
        for(auto p: v){
            notify();
        } 
    }
    
};

class Table3D
{

}

class PieGraph : public IGraph
{
public:
    virtual void update(Subject* p)
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
    virtual void update(int n)
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
    BarGraph bg;
    PieGraph pg;

    Table t;
    t.attach(&bg);
    t.attach(&pg);

    while(1) {
        int tmp;
        cint >> tmp;
        t.setData(tmp);
    }
}