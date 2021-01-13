---
layout: post
title: "1. C++ Refactoring Overview"
date: 2019-12-05 01:00:00 +0800
categories: C++ Refactoring
---


# Refactoring Overview


## Bad Smells in Code

**1. 리팩토링 정의**
* 겉으로 들어나는 기능은 그대로 둔채, 알아보기 쉽고 수정하기 간편하게 소프트웨어내부를수정하는 작업

**2. Code Coverage**
* Decision Coverage
  + 전체적인 결과가 참/거짓이면 충족된다.
  + 프로그램의 모든 시작점과 끝점을 적어도 한 번 이상 방문하고 프로그램 내 모든 Decision Point Decision Point Decision Point Decision PointDecision PointDecision Point Decision Point는 적어도 한 번 이상 모든 가능한 결과값을 가져야 함
* Condition Coverage
  + Decision Point 내 모든 Condition Condition 은 한 번 이상 모든 가능한 결과값을 가 져야 함
* Modified Condition / Decision Coverage
  + 시작점 과 끝점을 적어도 한번 이상 방문
  + 모든 Decision Point는적어도 한번이상 모든 가능한 결과 갑을 가져야 함
  + 모든 Condition은 모든 가능한 결과값을 가져야 함
  + 각 condition이 다른 컨디션에 영향 받지 않고 디지션 포인트 결과에 독립적으로 영향을 줘야 한다.
  ```cpp
    /*
    TEST CASE 고려해보기
    */
    {
        int z = 0;
        if ((x > 0) && (y > 0))
        {
            z = x;
        }
        return z;
    }
  ```

**3. Bad Smells in Code**

* **Long function(매소드가 길다)**
  * 최근 CPU에서는 함수 호출로 인한 오버해드가 거의 발생하지 않아 짧은 코드가 가독성이 좋음
    * 길다는 의미는 일과 일을 처리하는 방법 사이의 거리(semantic distance)
  * 이름을 잘 만들어야함(동의어 사전 활용)
  * **주석이 필요할 경우 함수 새로 만들기(공격적으로, 재사용을 고려하지 말고)**
  * 함수 이름은 How 보다는 What으로
* **Large Class(인스턴스 변수가 많고 코드가 길다)**
  * 의미있게 변수를묶고 서브 클래스 생성
  * 중복 코드 제거
  * Extract class && Extract subclass
* **Primitive Obsession(적절한 타입을 사용하지 않고 기본 자료형만 사용한다)**
* **Long Parameter List( 파라민터가 많고 자주 변경 됨)**
  * OOP 에서는 객체에서 직접 데이터를 전달하기 보다 필요한 것만 남기고 메소드를
  * 포함하고 있는 클래스에서 구함
  * 파라미터 리스트는 짧게
  * replace parameter with method
  * preserver whole object
* **Data Clumps(뭉쳐 다녀야 할 데이터가 하나의 객체로 정리 되어 있지 않다.)**
  * 덩어리를 객체로 변경 → Extract class 사용
* **OOA(Object-Orientation Abusers, 객체지향 남용**
* **Switch Statement**
    * OOP의 특징은 Switch 경우가 적개 발생함
    * OPP의 개념중 다향성(polymorphism)으로 문제 해결
* **Temporary Field**
  * 객체 안에인스터스 변수가 특정 상황에서만 할당 된다.
      * 사용되지 않는 것처러 보이는 변수가 왜 있는지 이해하기 어려움 → 직관적으로 코드를 짜야함
* 고아변수들을 위한 Extract Class 사용
* **Divergent Change(클래스가 다양한 이우로 수정된다.)**
  * 변경 시 명확히 한곳을 변경 할 수 있어야 한다.
  * Shotgun Surgery, Divergent Change
  * 특정 원인 찾은 후 Extract Class
* **Shotgun surgery(수정 할 때마다, 자잘 한 부분을 고쳐야 한다.)**
  * 하나를 변경 했을 때, 많은 클래스를 고쳐야 하는 경우
  * 변경해야 할 곳은 맣지만 찾기 어렵고 중요한 사항을 빼먹기 쉬움
* **Parallel Inheritance Hierarchies**
  * 하위 클래스를 만들 때마다 다른 서브 클래스를 만들어 주는 경우
  * Move method 와 Move field 를 사용
* **Comment**
  * 코드로 코멘트를 대신 하자!!
* **Duplicated Code**
  * 무조건 제거!!!!!
* **Lazy Code**
  * 리팩토링 이후 기능이 축소 되거나 쓸모 없어진 클래스가 있다 → 제거
* **Data Class(데이터를 보관만 담당하는 클래스가 있다)**
  * 멍청하게 데이터만 저장하고 거의 대부분 클래스 조작
  * Encapsulate filed 사용
* **Speculative Generality**
  * 아직 필요없는 기능이 구현 된 경우 → 삭제
  * 불필요한 위임은 Inline Class로 제거