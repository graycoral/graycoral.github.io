---
layout: post
title: "1. Template Basic"
date: 2019-12-31 01:00:00 +0800
categories: C++ template
---

## Template 이란 ?

* Code 를 생성하는 툴
* 개발자가 함수의 틀을 제공하고 컴파일러가 컴파일 할 떄, 소스 코드를  분석해서 필요한 타입의 함수(클래스)를 자동으로 생성함.
* Template은 선언부와 구현부가 동일(eg. test.h 에 선언한 경우 헤더파일에 구현)
* 함수 템플릿을 사용 할  때, 템플릿 인자의 타입을 명시적으로 전달 해도 되고 생략 해도 됨.(type deduction)

```cpp
    template<typename T> T square(T a)
    {
        return a * a;
    }
    template<class T> T square1(T a)
    {
    }

    int main()
    {
        square(3); // asm 확인 -> int square(int a)
        square(3.3); // asm 확인 -> double square(double a)
        return 0;
    }
```
## Template Instantiation(템플릿 인스턴스화)
* 컴파일 시간에 실제의 함수를 생성하는 과정을 template instantiation이라고 한다. (Explicit instantiation , Implicit instantiation)
* Explicit instantiation
    ```cpp
    template<typename T>class Test
    {
    public:
        void foo() {}
        void goo() {}
    };

    //template class Test<int>; // foo, goo 모두 초기화
    template void Test<int>::foo(); // foo 만 초기화 위의 모두 초기화와 중복 안 됨

    template<typename T>T square(T a)
    {
        return a*a;
    };
    // Explicit instantiation
    //template int square<int>(int);
    //template int square<>(int);
    template int square(int);

    int main()
    {
        int n1 = square(3);
        int n2 = square<int>(3);
        return 0;
    }
    ```
* Implicit instantiation
  + 소스코드에서 템플릿을 사용하는 코드가 있으면 암시적으로 인스턴스화가 발생
    ```cpp
    template<typename T> T square(T a)
    {
        return a * a;
    }
    int main()
    {
        square<int>(3); // 사용자가 템플릿을 전달하는 경우
        square<>(3);    // complie을 통해 type deduction 하는 경우
        square(3);      // 문맥을 통해서 인자의 타입을 추론 할 수 있는 경우
        return 0;
    }
    ```
* class template type deduction C++17
  + 클래스 템플릿은 C++ 14까지는 type deduction을 지원하지 않음
    ```cpp
    template<typename T> class Vector
    {
        T* buff;
    public:
        Vector() {}; // 생성자 1
        Vector(int sz, T init) {}; // 생성자 2

    }

    /*user define deduction */
    vector()->vector<int>;

    int main()
    {
        Vector<int> v1; // ok
        Vector  v2; // Errro ? why?

        Vector<int>(10,3);    // ok
        Vector(10,3);   // ok

        /* C++ 17 스타일 */
        pair<int, double> p1(1,3.3);
        pair p1(1,3.3);

        vector<int> v1(10,3);
        vector v2(10,3);

        return 0;
    }
    ```
  + 함수 템플릿 : 사용자가 템플릿 인자의 타입을 전달 가능/ 컴파일러가 함수의 인자를 통해서 추론 가능
  + 클래스 템플릿
    + 사용자가 템플릿을 인자를 전달해야함 / 전달하지 않으면 추론 가능함(c++14)
    + C++17부터는 함수 템플릿과 동일하게 전달 가능
+ 컴파일러에 의한 타입 추론을 막기 위한 방법
  + 컴파일러의 인자 추론을 막아 명확히 전달하기 위해 사용
    ```cpp
    template<typename T> struct identity
    {
        typedef T type;
    }
    template<typename T> void foo(T a)
    {
    }
    // 컴파일러에 의한 타입 추론을 막기 위해 indentity 사용
    template<typename T> void goo(typename identity<T>::type a)
    {
    }

    int main()
    {
        goo<int>(3); // T는 int -> identity<T>::type : int
        goo(3);      // identity 는 class template 이므로 타입을 직접 전달 해야함
        return 0;
    }
    ```
  + Object Generator
    ```cpp
    int main()
    {
        auto p1 = make_pair(10, 1);
        auto p1 = std::pair<int>(10, 1); // original
    }
    ```
+ lazy instantiation
  + Template로 만든 모든 함수가 모두 코드로 생성 되지 않음 → 사용되지 않은 템플릿은 인스턴스화 되지 않는다.
    ```cpp
    template<typename T> T square(T a)
    {
        return a * a;
    }
    int main()
    {
        // square을 사용 한적이 없어 코드가 생성 되지 않음
        return 0;
    }
    ```
    ```cpp
    template<typename T> T square(T a)
    {
        return a * a;
    }

    struct Resource1
    {
        Resource1() { cout << "Resource1() "<< endl}
        ~Resource1() { cout << "~Resource1() "<< endl}
    };

    struct Resource2
    {
        Resource2() { cout << "Resource2() "<< endl}
        ~Resource2() { cout << "~Resource2() "<< endl}
    };

    /*
    struct Test
    {
        Resource1 res1;
        static Resource2 res2;
    }

    Resource2 Test::res2;
    */

    template<typename T> struct Test
    {
        Resource1 res1;
        static Resource2 res2;
    };

    int main()
    {
        /* lazy instantiation */
        A<int> a;
        //a.foo(0); // error

        //Test t; // Resource2() -> main -> Resource1()
        Test<int> t; // main -> Resource1()

        return 0;
    }
    ```
+ Static if - C++ 17
    ```cpp
    template<typename T> void goo(typename identity::type a){}

    int main()
    {
        /* if constexpr c+=17 */  // 컴파일 시간에 조사
        if constexpr (false)
            goo(3);
        return 0;
    }
    ```
+ template type deduction
  + C++ typeid() 이용하면 어떤 타입인지 알수 있느나, const, volatile, reference는 출력하지 않음
  + boost.type_index 사용 하면  const, volatile, reference는 출력 함
  + type deduction 이란 ?
    + 사용자가 함수 템플릿 인자를 전달하지 않으면 컴파일러가 함수의 인자를 보고 템플릇 인자의 타입을 결정
  + 규칙
    + 값(T)을 받을 때 : const, volatile, reference 제거 후 타입 결정
    + 참조(T&)을 받을 때 : reference 속성만 제거 const, volatile, 유지
    + forwarding reference(T&&) : l value 인 경우 l value reference, r value 인경우 T는 값타임

    ```cpp
    template<typename T> void foo(T  a) {}
    template<typename T> void goo(T& a) {}

    int main()
    {
        const char* const s = "hello";
        foo(s); // cont char*  인자가 가진 const 속성만 제거 됨

        int x[10] = { 0 };
        foo(x); // int *
        goo(x); // int[10]
    }
    ```