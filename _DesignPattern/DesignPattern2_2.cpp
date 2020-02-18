#include <iostream>
#include <algorithm>
using namespace std;
#define TEST    (3)


#if TEST == 1
// Application Framework
/*
    * 모든 것을 객체로 하자
        * c/c++은 main 은 일반 함수로 해야한다
    * 특정분야의 프로그램은 전체적인 흐름이 항상 유사하다
        * GUI/스마트폰 앱 게임등
    * main 함수에 전체적인 흐름을 담아서 라이브러리 내부에 감추자
        * Appliction Framework
        * MFC QT IOS Android
*/

class CWinApp; // 클래스 전방 선언 : 포인터는 사용가능

CWinApp* g_app = 0;

class CWinApp
{
public:
    CWinApp()   {g_app = this;}
    virtual bool InitInstance() { return false; }
    virtual int ExitInstance() { return false; }
    virtual int Run() { return 0;}
};

/*
    라이브러리 사용자
    1. CWinApp 의 파생 클래스를 만들어야 한다.
    2. 사용자 클래스 전역객체 생성
*/
int main()
{
    if(g_app->InitInstance() == true)   g_app->Run();
    g_app->ExitInstance();
    return 0;
}

class MyApp : public CWinApp
{
public:
    virtual bool InitInstance() override
    {
        cout << "init" << endl;
        return true;
    }
    virtual int ExitInstance() override
    {
        cout << "finish" << endl;
        return 0;
    }
};

MyApp theApp; // ??? 왜???

/* 실행 순서
 // 1. 전역변수 생성자. 기반 클래스 생성자

*/
#elif TEST == 2
// 함수와 정책 : 일반함수와 가성변
/*
    * 멤버 함수에서 변하는 것
        * 변하는 것을 가상함수로 분리
        * 변하는 것을 다른 클래스로 분리
    * 멤버 아닌 일반 함수에서 변하는 것
        * 변해야 하는(정책)것을 **함수 인자화**
*/

// void Sort(int* x, int sz)
// {
//     for(int i = 0; i <sz-1; i++){
//         for(int j=i+1; j<sz; j++ ){
//             if(x[i] > x[j]) swap(x[i], x[j]);
//         }
//     }
// }

void Sort(int* x, int sz, bool(*cmp)(int ,int )){
    for(int i = 0; i <sz-1; i++){
        for(int j=i+1; j<sz; j++ ){
            // if(x[i] > x[j]) swap(x[i], x[j]);
            if(cmp(x[i], x[j])){
                swap(x[i], x[j]);
            }
        }
    }
}

bool cmp1(int a, int b) {return a < b;}
bool cmp2(int a, int b) {return a > b;}

int main()
{
    int x[10] = {1,3,5,7,9,2,4,6,8,10};
    // Sort(x, 10);
    Sort(x, 10, &cmp2);

    for(auto n : x) {
        cout << n << ",";
    }
}

#elif TEST == 3
/*
    함수 인자로 정채을 담은 코드를 전달하는 방법
    * 함수 포인터 : 코드 메모리 증가하지 않음 , 인라인 치환 안됨
    * 함수 객체 람다 표현식을 전달하고 템플릿을 받는다. : 인란인 치환 가능 , 코드 메모리 증가
*/

//void Sort(int* x, int sz, bool(*cmp)(int ,int )){
template<typename T>
void Sort(int* x, int sz, T cmp) {
    for(int i = 0; i <sz-1; i++){
        for(int j=i+1; j<sz; j++ ){
            // if(x[i] > x[j]) swap(x[i], x[j]);
            if(cmp(x[i], x[j])){
                swap(x[i], x[j]);
            }
        }
    }
}

bool cmp1(int a, int b) {return a < b;}
bool cmp2(int a, int b) {return a > b;}

int main()
{
    int x[10] = {1,3,5,7,9,2,4,6,8,10};
    // Sort(x, 10);
    Sort(x, 10, [](int a, int b){return a > b;}); // 인라이 치환 안되기 때문에 성능 저하 우려 있다

    for(auto n : x) {
        cout << n << ",";
    }
}

#endif