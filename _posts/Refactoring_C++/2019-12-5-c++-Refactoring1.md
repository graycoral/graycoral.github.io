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

## **Replace Method with Method Object**
---
* Replace Method with Method Object(메소드를 메소드 객체로 전환)
  * 지역변수 떄문에 메소드 추출을 적용 할 수없는 긴 메소드가 있을 때, 그 메소드 자체를 객체로 전환 해서 지역변수를 객체의 필드로 만들자
  * 그런다음 그 메소드를 객체 안의 여로 메소드로 쪼개면 된다.
  * **메스드로 만들면 디펜던스가 생기기 떄문에 사용자가 판단 해야함**

  ```cpp
  #include <iostream>
  using namespace std;
  class Account {
  public:
    int delta() {
    return 0.1;
    }
    int gamma(int inputVal, int quantity, int yearToDate) {
      int importantValue1 = (inputVal * quantity) + delta();
      int importantValue2 = (inputVal * yearToDate) + 100;
      if ((yearToDate - importantValue1) > 100) {
        importantValue2 -= 20;
      }
      int importantValue3 = importantValue2 * 7;
      return importantValue3 - 2 * importantValue1;
    }
  };

  int main() {
    Account account;
    cout << account.gamma(1, 2, 3) << endl;
  }
  ```

  ```cpp
  #include <iostream>
  using namespace std;

  class Account {
  public:
  int delta() const {
    return 0.1;
  }
  int gamma(int inputVal, int quantity, int yearToDate);
  };

  class Gamma {
  public:
  Gamma(Account* account, int inputVal, int quantity, int yearToDate) {
    _account = account;
    this->inputVal = inputVal;
    this->quantity = quantity;
    this->yearToDate = yearToDate;
  }
  int compute() {
    int importantValue1 = (inputVal * quantity) + _account->delta();
    int importantValue2 = (inputVal * yearToDate) + 100;
    if ((yearToDate - importantValue1) > 100) {
    importantValue2 -= 20;
    }
    int importantValue3 = importantValue2 * 7;
    return importantValue3 - 2 * importantValue1;
  }
  private:
  const Account* _account;
  int inputVal;
  int quantity;
  int yearToDate;
  int importantValue1;
  int importantValue2;
  int importantValue3;
  };

  int Account::gamma(int inputVal, int quantity, int yearToDate) {
  Gamma g(this, inputVal, quantity, yearToDate);
  return g.compute();
  }

  int main() {
  Account account;
  cout << account.gamma(1, 2, 3) << endl;
  }
  ```

## ** Decompose Conditional**
* Decompose Conditional(조건문 쪼개기)
  * 복잡한 조건문이 있을 때, if, then. else 부분을 메소드로 뺴내자
  ```cpp
  double heatingCharge(int date, int quantity)
  {
  double charge = 0.0;
  if (date < SUMMER_START || date > SUMMER_END)
  {
    charge = quantity * winterRate + winterServiceCharge;
  } else {
    charge = quantity * summerRate;
  }
  return charge;
  }
  ```
  ```cpp
  int notSummer(int date) {
    return (date < SUMMER_START || date > SUMMER_END);
  }

  double NotSummerCharge(int quantity) {
      return quantity * winterRate + winterServiceCharge;
  }

  double SummerCharge(int quantity) {
      return quantity * summerRate;
  }

  double heatingCharge(int date, int quantity)
  {
      double charge = 0.0;
      if (notSummer(date)) {
          charge = NotSummerCharge(quantity);
      } else {
          charge = SummerCharge(quantity);
      }
      return charge;
  }
  ```

## **Consolidate Conditional Expression / Consolidate Duplicate Frangments / Remove Control Flag / Guard Clauses**

* Consolidate Conditional Expression(중복 조건식 통합)
  * 여러 조건 검사식의 결과가 같을 떄, 하나의 조건문을 합친 후 메소드를 빼낸다
* Consolidate Duplicate Frangments (조건문의 공통 실행 코드 빼내기)
  * 조건문의 모든 절에 같은 실행 코드가 있을 때, 같은 부분을 조건문 밖으로 뺴낸다.
* Remove Control Flag
  * 논리 연삭식의 제어 플래그 역할을 하는 변수가 있을 때, 그 변수를 Break or retunr 으로 변경

  ```cpp
  void sendAlert()
  {
  }

  int checkSecurity(char** people, int peopleLength)
  {
      int found = 0;
      for (int i = 0; i < peopleLength; ++i)
      {
          if (!found)
          {
              if (strcmp(people[i], "Don") == 0)
              {
                  sendAlert();
                  found = 1;
              }
              if (strcmp(people[i], "John") == 0)
              {
                  sendAlert();
                  found = 1;
              }
          }
      }
      return found;
  }
  ```

  ```cpp
  int checkSecurity(char** people, int peopleLength)
  {
      for (int i = 0; i < peopleLength; ++i)
      {
          if (strcmp(people[i], "Don") == 0)
          {
              sendAlert();
              return 1;
          }
          if (strcmp(people[i], "John") == 0)
          {
              sendAlert();
              return 1;
          }
      }
      return -1;
  }
  ```

* Replace Nested Conditional with Guard Clauses(여러겹의 조건문을 감시 절로 전환)

  ```cpp
  double getPayAmount(){
    double result;
    if(_isDead) result = deadAmount();
    else{
    if(_isSeparated)result = separatedAmount();
    else result = normalAmount();
    }
    return result;
  }
  ```

  ```cpp
  double getPayAmount(){
      if(_isDead)     return deadAmount();
      if(_isSeparated)return separatedAmount();
      if(_isRetired)  return retiredAmount();
      return normalAmount();
  }
  ```

  ```cpp
  double getAdjustedCapital(double capital, double intRate, double duration)
  {
      double result = 0.0;
      if (capital > 0.0)
      {
          if (intRate > 0.0 && duration > 0.0)
          {
              result = (income / duration) * ADJ_FACTOR;
          }
      }
      return result;
  }
  ```

  ```cpp
  double getAdjustedCapital(double capital, double intRate, double duration) {
      double result = 0.0;
    if (capital <= 0.0) {
      return result;
    }
    if (intRate <= 0.0 || duration <= 0.0) {
      return result;
    }
    return (income / duration) * ADJ_FACTOR;
  }
  ```

## **Introduce Null Object**
---
*