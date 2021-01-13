---
layout: post
title: "Refactoring"
date: 2019-12-05 01:00:00 +0800
categories: Refactoring c++
------

# Refactoring


## **Extract Function(함수 추출하기)**
---
  * 함수의 목적(what)을 하는지 위주로 파악 → What으로 함수 이름 추출
  * 함수를 짧게 생각하기 → 6줄 이상이면 Bad Smells 의심해보기
  * 함수가 더 짧아지면 캐쉬가 쉬워져서 컴파일러 최적화가 더 잘 됨

    ```cpp
    // Extract Function Before
    void PrintOwing()
    {
    double outstanding(0.0);

    // init orders
    for (int i = 0; i < 5; ++i)
    {
        orders.push_back(i);
    }

    // print banner
    cout << "**************************" << endl;
    cout << "***** Customer Owes ******" << endl;
    cout << "**************************" << endl;

    // calculate outstanding
    vector<double>::const_iterator orders_it = orders.begin();
    while (++orders_it != orders.end())
    {
        outstanding += *orders_it;
    }

    // print details
    cout << "name:" + name << endl;
    cout << "amount:" << outstanding << endl;
    }
    ```

    ```cpp
    /// Extract Function After
    void PrintBanner() {
        cout << "**************************" << endl;
        cout << "***** Customer Owes ******" << endl;
        cout << "**************************" << endl;
    }
    void PrintDetails(double outstanding) {
    cout << "name:" + name << endl;
    cout << "amount:" << outstanding << endl;
    }
    void InitOrder() {
    for (int i = 0; i < 5; ++i) {
        orders.push_back(i);
    }
    }
    double CalculateOutstanding()
    {
    double outstanding(0.0);
    // calculate outstanding
    vector<double>::const_iterator orders_it = orders.begin();
    while (++orders_it != orders.end()) {
        outstanding += *orders_it;
    }
    return outstanding;
    }

    void PrintOwing() {
        InitOrder();
        PrintBanner();
        PrintDetails(CalculateOutstanding());
    }
    ```

## **Inline Method / Inline Temp / Replace Temp with Query / Introduce Explanning Variable**
---

* **Inline Method**
  * 메소드 기능이너무 간단해서 메소드명만 봐도 너무 뻔할 때.
  * 그 메소드의 기능을 호출하는 메소드를 넣어버리고 그 메소드를 삭제
  ```cpp
  /// Inline Method Before
  int geRating(){
    return (moreThanFiveLateDeliveries()) ? 2 : 1;
  bool moreThanFiveLateDeliveries(){
    return _numberofLateDeliveries > 5;
  ```

  ```cpp
  int geRating(){
    return (_numberofLateDeliveries > 5) ? 2 : 1;
  ```

* **Inline Temp**
  * 간단한 수식을 대입을 받는 임시 변수로 인해 다른 리팩토링이 힘들 때, 참조하는 부부을 수식으로 치환
* Replace Temp with Query
  * 수식의 결과를 저장하는 임시변수는 메소드로 변환하여 사용 → 한번만 사용 되는 경우 **(const를 붙여보면 알수 있음)**
  ```cpp
  /// Replace Temp with Query Before
  int quantity = 10; int itemPrice = 15000;

    double getPrice()
    {
        int basePrice = quantity * itemPrice;  // - (1)
        // getBasePrice() 여기에 왜 안넣었을까 ??
        // -> 기존의 코드와 변경점을 확인 할 수 있다(점진적 리페토링)
        double discountFactor;
        if (basePrice > 1000) // - (2) 정답
        {
            discountFactor = 0.95;
        }
        else
        {
            discountFactor = 0.98;
        }
        return basePrice * discountFactor; // - (3)
    }
  ```

  ```cpp
  /// Replace Temp with Query After
  int quantity = 10; int itemPrice = 15000;
    double getBasePrice() {
    return quantity * itemPrice;
    }
    double getDiscountFactor() {
    if (getBasePrice() > 1000) {  return 0.95; }
    else               {  return 0.98; }
    }
    double getPrice() {
        return getBasePrice() * getDiscountFactor();
    }
  ```

* Introduce Explanning Variable
  * 사용된 수식이 복잡 할 때, 수식의 결과나 수식의 일부분을 용도에 부합하는 직관적인 이름의 임시변수 대입
  * **상황마다 다르므로 잘 판단해서 쓰자**

## **Split Temporary Variable**
---

* Split Temporary Variable(임시변수 분리)
  * 루프나 변수나 값 누적용 임시변수가 아닌 임시 변수에 여러 번 값이 대입 될 때, 각 대입 마다 다른 임시변수를 사용하지 말자
  ```cpp
  double GetDistanceTravelled(int time)
  {
      double result;
      double acc = primary_force / mass;
      int primary_time = min(time, delay);
      result = 0.5 * acc * primary_time * primary_time;
      int secondary_time = time - delay;
      if (secondary_time > 0)
      {
          double primary_vel = acc * delay;
          acc = (primary_force + secondary_force) / mass;
          result += primary_vel * secondary_time + 0.5 * acc * secondary_time * secondary_time;
      }
      return result;
  }
  ```

  ```cpp
  double getPrimaryAcc() {
    return primary_force / mass;
  }
  double GetDistanceTravelled(int time) {
      double result;
      int primary_time = min(time, delay);
      result = 0.5 * getPrimaryAcc() * primary_time * primary_time;
      int secondary_time = time - delay;
      if (secondary_time > 0) {
          double primary_vel = getPrimaryAcc() * delay;
          const double secondary_acc = (primary_force + secondary_force) / mass;
          result += primary_vel * secondary_time + 0.5 * secondary_acc * secondary_time * secondary_time;
      }
      return result;
  }
  ```