---
layout: post
title: "Refactoring : 객체 간의 이동"
date: 2019-12-05 01:00:00 +0800
categories: C++ Refactoring
---

# 객체 간의 기능 이동

## Move Method / Move Field / Extract Class

* Move Method(메소드 이동)
  * 메소드가 자신이 속한 클래스 보다 다른 클래스의 기능을 더 많이 이용할 떄,
  * 제일 많이 이용하는 클래스 안에 비슷한 내용의 새 매소드를 작성하자.
  * 기존 메소드는 간단한 대리 메소드로 전환 또는 삭제
  * Motivation
    * 클래스에 기능이 너무 많음
    * 클래스가다른 클래스와 과하게 연동 되어 의존성이 지나 칠때
    * 일부 속성을 옮길 때 이 작업을 하는기 좋다
  * **어렵다(마틴 파울러도 40년 동안 이짓을 했는데 어렵다....)**
* Move Field
  * 어떤필드가 자신이 속한 클래스 보다 다른 클래스에서 더많이 사용될 떄는 대상 클래스 안에새 필드 선언 하고 그 필드 참조 부분을전부 새 필드 참조로 수정하자

    ```cpp
    class Account {
    public:
        double OverdraftCharge();
        double BankCharge();
        double days_overdrawn;
        AccountType type;
    };
    ```

* Extract Class
  * 두개의 클래스가 처리해할 기능이 하나의 클래스에 들어 있을 때, 새 클래스 만들고 기존의 클래스의 관련 필드와 메서드를 분리하자
  * 클래스의 기능 분리 방법 정하기
  * 분리 가능한 클래스를 새로운 클래스로 작성
    * 원본이 이름이 변경 될 수 도 있다
    * 원본클래스에서 새 클래스로의 링크 만들기

    ```cpp
    class Person {
    public:
        string GetName();
        string GetTelephoneNumber();
        string GetOfficeAreaCode();
        void SetOfficeAreaCode(string arg);
        string GetOfficeNumber();
        void SetOfficeNumber(string arg);

    private:
        string name_;
        string office_area_code_;
        string office_number_;
    };

    #include <string>
    #include "sample1.h"

    using namespace std;

    string Person::GetName()
    {
        return name_;
    }

    string Person::GetTelephoneNumber()
    {
        return "(" + office_area_code_ + ") " + office_number_;
    }

    string Person::GetOfficeAreaCode()
    {
        return office_area_code_;
    }

    void Person::SetOfficeAreaCode(string arg)
    {
        office_area_code_ = arg;
    }

    string Person::GetOfficeNumber()
    {
        return office_number_;
    }

    void Person::SetOfficeNumber(string arg)
    {
        office_number_ = arg;
    }
    ```

    ```cpp
    #include <string>
    using namespace std;
    class TelphoneNumber {
    public:
        string GetTelephoneNumber();
        string GetAreaCode();
        void SetAreaCode(string arg);
        string GetNumber();
        void SetNumber(string arg);
        string area_code_;
        string number_;
    };
    class Person {
    public:
        string GetName();
        string GetTelephoneNumber();
        string GetOfficeAreaCode();
        void SetOfficeAreaCode(string arg);
        string GetOfficeNumber();
        void SetOfficeNumber(string arg);
    private:
        string name_;
        TelphoneNumber office_num;
    };

    #include <string>
    #include "sample1.h"

    using namespace std;
    string TelphoneNumber::GetTelephoneNumber()
    {
        return "(" + area_code_ + ") " + number_;
    }
    string TelphoneNumber::GetAreaCode()
    {
        return area_code_;
    }
    void TelphoneNumber::SetAreaCode(string arg)
    {
        area_code_ = arg;
    }
    string TelphoneNumber::GetNumber()
    {
        return number_;
    }
    void TelphoneNumber::SetNumber(string arg)
    {
        number_ = arg;
    }
    string Person::GetName()
    {
        return name_;
    }
    string Person::GetTelephoneNumber()
    {
        return office_num.GetTelephoneNumber();
    }
    string Person::GetOfficeAreaCode()
    {
        return office_num.GetAreaCode();
    }
    void Person::SetOfficeAreaCode(string arg)
    {
        office_num.SetAreaCode(arg);
    }
    string Person::GetOfficeNumber()
    {
        return office_num.GetNumber();
    }
    void Person::SetOfficeNumber(string arg)
    {
        office_num.SetNumber(arg);
    }
    ```

## Inline Class

* Inline Class(클래스 내용 직접 삽입)
  * 클래스의 내용이 너무 적을 떄 합치고 원래 클래스 삭제 → 가장 많이 사용 한 곳은 어딘가?


# 일반화 처리

## Pull up field, Method / Extract Subclass, Interface

* Pull up field(필드 상향)
  * 중복 된 경우 상위 클래스로 이동
* Pull up Method(메소드 상향)
  * 중복 된 경우 상위 클래스로 이동
* Extract Subclass(하위 클래스 추출)
  * 기능이 비슷한 두 클래스가 있으면 상위 클래스 옮기기
  * 중복 코드 제거 하는 효과가 있음
  * 나중에 발견해도 상속 구조를 만든다
  * 상위 클래스로 잘못 옮긴 경우 위임(delegation) 하자
* Extract Interface
  * 클래스 인터페이스의 같은 부분을 여러클라이언트가 사용하거나
  * 두 클래스 공통 부분을 추출