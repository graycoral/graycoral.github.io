---
layout: post
title: "2. C++ Refactoring"
date: 2019-12-05 02:00:00 +0800
categories: C++ Refactoring
---

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

## **Decompose Conditional**
---

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
---

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

* Introduce Null Object(Null 검사를 널 객체에 위임)
  * null 값을 감사하는 코드가 계속나올 때 , null 값을 객체로 만들자

  ```cpp
  class Customer {
  public:
      string GetName();
      void SetName(string name);
      virtual bool IsNull() { return false; }
  private:
      string name_;
  };
  class NullCustomer : public Customer {
  public:
      static NullCustomer* NewNull();
      virtual bool IsNull() { return true; }
  };
  class Site {
  public:
      Site();
      Customer* GetCustomer();
  private:
      Customer* customer_;
  };
  ```

  ```cpp
  #include <iostream>
  #include <string>
  using namespace std;
  #include "sample1.h"

  string Customer::GetName() { return name_; }
  void Customer::SetName(string name) { name_ = name; }
  NullCustomer* NullCustomer::NewNull() {
    static NullCustomer null_item;
    return &null_item;
  }
  Site::Site() : customer_(0) {}
  Customer* Site::GetCustomer() {
    return (customer_ == 0) ? NullCustomer::NewNull() : customer_;
  }
  int main() {
    Customer* customer1 = new Customer();
    customer1->SetName("Refactoring");
    cout << customer1->GetName() << endl;

    // Customer* customer2;
    // cout << customer2->GetName() << endl;

    Customer* customer3 = new Customer();
    delete customer3;
    customer3 = NullCustomer::NewNull();
    cout << customer3->GetName() << endl;

    Customer* customer4 = NullCustomer::NewNull();
    customer4->SetName("singleton null customer");
    cout << customer4->GetName() << endl;

    Customer* customer5 = NullCustomer::NewNull();
    cout << customer5->GetName() << endl;

    Customer* customer6 = NullCustomer::NewNull();
    cout << customer6->IsNull() << endl;

    Customer* customer7 = new Customer();
    cout << customer7->IsNull() << endl;

    Site site;
    Customer* customer8 = site.GetCustomer();
    string customer_name;
    if (customer8->IsNull()) {
      customer_name = "null name";
    } else {
      customer_name = customer8->GetName();
    }
    cout << customer_name << endl;

    Customer* customer9 = new Customer();
    customer9->SetName("LG");
    if (customer9->IsNull()) {
      customer_name = "null name";
    } else {
      customer_name = customer9->GetName();
    }
    cout << customer_name << endl;

    Site site10;
    Customer* customer10 = site10.GetCustomer();
    cout << customer10->GetName() << endl;
    customer10 = new Customer();
    customer10->SetName("customer 10");
    cout << customer10->GetName() << endl;
  }
  ```

## **Change Function Declaration (함수 선언 변경하기)**
---

* Change Function Declaration(함수 선언 변경하기)
  * 함수 선언은 이조각 들이 어떻게 연결 되는지 나타낸다 → 관절 같은...
  * 좋은 관절은 시스템에 새로운 조각을 추가 할 수 있다
  * 나뿐 관절은 끊임 없이 어려움의 원인이 되어 수정 할 때, 더 어렵게 한다
  * 함수 이름이 좋아야한다... 그러나 어렵다
  * 새 이름으로 새메서드를 선언하자 코드의 원래 내용을 새매서등로 복사 하고 적절히 수정하자
  * 수정 할 때마다 실시

  ```cpp
  class Person {
  public:
      string _officeAreaCode;
      string _officeNumber;
      Person() : _officeAreaCode("02"), _officeNumber("6950-3246") {
      }
      string getTelephoneNumber() {
          return "(" + _officeAreaCode + ")" + _officeNumber;
      }
  };

  TEST(Refactoring, RenameMethod)
  {
      Person person;
      EXPECT_EQ("(02)6950-3246", person.getTelephoneNumber());
  }
  ```

  ```cpp
  class Person {
  public:
      string _officeAreaCode;
      string _officeNumber;
      Person() : _officeAreaCode("02"), _officeNumber("6950-3246") {
      }
      string getOfficeTelephoneNumber() {
          return "(" + _officeAreaCode + ")" + _officeNumber;
      }
  };

  TEST(Refactoring, RenameMethod)
  {
      Person person;
      EXPECT_EQ("(02)6950-3246", person.getOfficeTelephoneNumber());
      EXPECT_EQ("(02)6950-3246", person.getOfficeTelephoneNumber());
      EXPECT_EQ("(02)6950-3246", person.getOfficeTelephoneNumber());
      EXPECT_EQ("(02)6950-3246", person.getOfficeTelephoneNumber());
      EXPECT_EQ("(02)6950-3246", person.getOfficeTelephoneNumber());
      EXPECT_EQ("(02)6950-3246", person.getOfficeTelephoneNumber());
      EXPECT_EQ("(02)6950-3246", person.getOfficeTelephoneNumber());
      EXPECT_EQ("(02)6950-3246", person.getOfficeTelephoneNumber());
  }
  ```

## **Replace Parameter with Explicit Methods**
---

* Replace Parameter with Explicit Methods(매개변수를 메소드로 전환)
  * 매개변수로 전달 된 값에 따라 메소드가 다른 코드를 실행 할 때 그 값에 대응 하는 메소드를 각각 작성하자
  * 조건문의 각 절 마다 해당 되는 새 메서드를 호출을 넣자
  * 매개 변수 값이 많이 변할 가능성이 있을 때는 적용하면 안됨
  ```cpp
  int getEmployeeSw(int type)
  {
      switch (type)
      {
      case ENGINEER:
          return VISUALSTUDIO;
      case SALESMAN:
          return EXCEL;
      case MANAGER:
          return POWERPOINT;
      default:
          return NO_DEFINED;
      }
  }
  int createEmployee(int type)
  {
      int employeeSw = getEmployeeSw(type);

      return employeeSw;
  }
  ```

  ```cpp
  int engineerSw() {
    return VISUALSTUDIO;
  }
  int salesmanSw() {
      return EXCEL;
  }
  int managerSw() {
      return POWERPOINT;
  }
  int noDefined() {
      return NO_DEFINED;
  }
  int getEmployeeSw(int type)
  {
      switch (type)
      {
      case ENGINEER:
          return engineerSw();
      case SALESMAN:
          return salesmanSw();
      case MANAGER:
          return managerSw();
      default:
          break;
      }
      return noDefined();
  }
  int createEmployee(int type)
  {
      int employeeSw = getEmployeeSw(type);
      return employeeSw;
  }
  int createEngineerSw() {
      int engineerSw2 = engineerSw();
      return engineerSw2;
  }
  ```

## **Preserve Whole Object(객체를 전부 전달)**
---
* Preserve Whole Object(객체를 전부 전달)
  * 객체에서 가져온 여러 갑을 메소드 호출에서 매개 변수로 전달 할 때, 그 객체를 통째로 전달 하게 수정하자.
  * 매개 변수 새로 작성 후 컴파일/테스트
  * 통 객체를 가져올 값이 하나인 경우
    * 코드 명로성 입장에서는 통으로 가져오나 값으로 가져오나 차이 없다
    * 성능은 약간 떨어질수 있으나 무시 가능
    * 불필요한 의존성 생김
  * 대안으로 Move Method

  ```cpp
  bool HeatingPlan::WithinRange(int low, int high)
  {
      return low >= range.GetLow() && high <= range.GetHigh();
  }

  bool Room::WithinPlan(HeatingPlan plan)
  {
      int low = DaysTempRange().GetLow();
      int high = DaysTempRange().GetHigh();
      return plan.WithinRange(low, high);
  }

  TempRange Room::DaysTempRange()
  {
      return range;
  }
  ```

  ```cpp
  bool HeatingPlan::WithinRange(int low, int high)
  {
      return low >= range.GetLow() && high <= range.GetHigh();
  }
  bool HeatingPlan::WithinRange(TempRange roomRange)
  {
      return range.Includes(roomRange);
  }
  bool Room::WithinPlan(HeatingPlan plan)
  {
      int low = DaysTempRange().GetLow();
      int high = DaysTempRange().GetHigh();
      return plan.WithinRange(DaysTempRange());
  }
  TempRange Room::DaysTempRange()
  {
      return range;
  }
  bool TempRange::Includes(TempRange arg) {
      return arg.GetLow() >= GetLow() && arg.GetHigh() <= GetHigh();
  }
  ```

## **Replace Parameter with Method**
---
* Replace Parameter with Method(매개변수 세트를 메서드로 전환)
* 객체가 A 메소드를 호출해서 그 결과를 B 메소드로 전달하는데 B가 A를 직접 호출 할 수 있을때, 매개 변수 삭제 후 B에서 직접 A 호출
  ```cpp
  #include "gtest/gtest.h"

  using namespace std;

  class Order {
  public:
      int _quantity;
      int _itemPrice;
      Order() : _quantity(0), _itemPrice(0) {
      }
      double getPrice() {
          int basePrice = _quantity * _itemPrice;
          int discountLevel;
          if (_quantity > 100) discountLevel = 2;
          else discountLevel = 1;
          double finalPrice = discountedPrice(basePrice, discountLevel);
          return finalPrice;
      }

      double discountedPrice(int basePrice, int discountLevel) {
          if (discountLevel == 2) return basePrice * 0.1;
          else return basePrice * 0.05;
      }
  };

  TEST(Refactoring, RenameMethod)
  {
      Order order;
      order._quantity = 100;
      order._itemPrice = 500;

      int basePrice = order._quantity * order._itemPrice;
      int discountLevel;
      if (order._quantity > 100) discountLevel = 2;
      else discountLevel = 1;
      EXPECT_EQ(2500, order.discountedPrice(basePrice, discountLevel));

      order._quantity = 101;
      basePrice = order._quantity * order._itemPrice;
      if (order._quantity > 100) discountLevel = 2;
      else discountLevel = 1;
      EXPECT_EQ(5050, order.discountedPrice(basePrice, discountLevel));
  }
  ```

  ```cpp
  class Order {
  public:
      int _quantity;
      int _itemPrice;
      Order() : _quantity(0), _itemPrice(0) {
      }
      int getDiscountLevel() {
          if (_quantity > 100) return 2;
          return 1;
      }
      int getBasePrice()  {
          return _quantity * _itemPrice;
      }
      double getPrice() {
          double finalPrice = discountedPrice();
          return finalPrice;
      }
      double discountedPrice() {
          if (getDiscountLevel() == 2) return getBasePrice() * 0.1;
          else return getBasePrice() * 0.05;
      }
  };

  TEST(Refactoring, ReplaceParameterWithMethod )
  {
      Order order;
      order._quantity = 100;
      order._itemPrice = 500;
      EXPECT_EQ(2500, order.discountedPrice());
      order._quantity = 101;
      EXPECT_EQ(5050, order.discountedPrice());
  }
  ```

## **Introduce Parameter Object / Remove Setting Method**
---
* Introduce Parameter Object (매개변수 세트를 객체로 전환)
  * 여러 개의 매개 변수가 항상 붙어 다닐 때, 그 매개변수들을 객체로 바꾸자
* Remove Setting Method(쓰기 메소드 제거)
  * 생성 할 때 지정한 필드 값이 절대로 변경 되지 말아야 할 떄, 그 필드설정하는 모든 쓰기 메소드 삭제