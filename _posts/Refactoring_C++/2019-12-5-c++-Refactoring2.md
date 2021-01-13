---
layout: post
title: "Refactoring : 데이터 체계화"
date: 2019-12-05 01:00:00 +0800
categories: C++ Refactoring
---


# 데이터 체계화

## Replace Array with object(배열을 객체로 전환)

* Replace Array with object(배열을 객체로 전환)
  * 베열을 구성하는 특정 원소가 별의별 의미를 지닐 떄, 각 원소별 객체로 전환 하자

    ```cpp
    char row[3][50];

    int setScore(char* name, char* wins, char* losses)
    {
        strcpy(row[0], name);
        strcpy(row[1], wins);
        strcpy(row[2], losses);
        return atoi(row[1]) + atoi(row[2]);
    }
    ```

    ```cpp
    char row[3][50];

    struct Score {
        char name[50];
        int wins;
        int losses;
    } new_row;

    int setScore(char* name, char* wins, char* losses)
    {
        strcpy(new_row.name, name);
        new_row.wins = atoi(wins);
        new_row.losses = atoi(losses);
        return new_row.wins + new_row.losses;
    }
    ```

## Replace Magic Number with Symbolic Constant / Replace Type Code with class

* Replace Magic Number with Symbolic Constant
  * 특수 의미를 지닌 리터럴 숫자가 있을때 상수로 교체 하자

* Replace Type Code with class(분류 부호를클래스로 전환)
  * 기능에 영향을 미치는 수자형 분류 부호가 든 클래스가 있을 때, 그 숫자를 새로운 클래스로 생성

아래의 예제에서 Enum class를 쓰면 좋은 예/ → ENUM class를 쓰자

    ```cpp
    class Person {
    public:
        static const int O = 0;
        static const int A = 1;
        static const int B = 2;
        static const int AB = 3;
    private:
        int _bloodGroup;
    public:
        Person(int bloodGroup) {
            _bloodGroup = bloodGroup;
        }
        void setBloodGroup(int arg) {
            _bloodGroup = arg;
        }
        int getBloodGroup() {
            return _bloodGroup;
        }
    };

    TEST(A, B)
    {
        Person* person = new Person(Person::O);
        EXPECT_EQ((int)Person::O, person->getBloodGroup());
        person->setBloodGroup(Person::A);
        EXPECT_EQ((int)Person::A, person->getBloodGroup());
        person->setBloodGroup(Person::B);
        EXPECT_EQ((int)Person::B, person->getBloodGroup());
        person->setBloodGroup(Person::AB);
        EXPECT_EQ((int)Person::AB, person->getBloodGroup());
        delete person;
    }
    ```

    ```cpp
    /*
    class BloodGroup {
    public:
        static const BloodGroup* O;
        static const BloodGroup* A;
        static const BloodGroup* B;
        static const BloodGroup* AB;
        int getCode() const {
            return _code;
        }
        static const BloodGroup* code(int arg) {
            return _values[arg];
        }
        bool operator==(const BloodGroup& op) const {
            return getCode() == op.getCode();
        }
    private:
        static const BloodGroup* _values[];
        const int _code;
        BloodGroup(int code) : _code(code) {}
    };
    const BloodGroup* BloodGroup::O = new BloodGroup(0);
    const BloodGroup* BloodGroup::A = new BloodGroup(1);
    const BloodGroup* BloodGroup::B = new BloodGroup(2);
    const BloodGroup* BloodGroup::AB = new BloodGroup(3);
    const BloodGroup* BloodGroup::_values[] = {O, A, B, AB};
    */
    enum class BloodGroup {
        O, A, B, AB
    };
    class Person {
    private:
        BloodGroup _bloodGroup;
    public:
        Person(const BloodGroup bloodGroup) {
            _bloodGroup = bloodGroup;
        }
        void setBloodGroup(const BloodGroup arg) {
            _bloodGroup = arg;
        }
        const BloodGroup getBloodGroup() {
            return _bloodGroup;
        }
    };
    TEST(A, B)
    {
        Person* person = new Person(BloodGroup::O);
        EXPECT_EQ(BloodGroup::O, person->getBloodGroup());
        person->setBloodGroup(BloodGroup::A);
        EXPECT_EQ(BloodGroup::A, person->getBloodGroup());
        person->setBloodGroup(BloodGroup::B);
        EXPECT_EQ(BloodGroup::B, person->getBloodGroup());
        person->setBloodGroup(BloodGroup::AB);
        EXPECT_EQ(BloodGroup::AB, person->getBloodGroup());
        delete person;
    }
    ```
## Replace Type Code with Subclass

* Replace Type Code with Subclass(분류 부호를 하위 클래스로 전환)
  * 클래스 기능에 영향을 주는 변경 불가 분류 부호가 있을 때 분류 부홀 하위 클래스로 만들자

    ```cpp
    class Employee {
    private:
        int _type;
    public:
        static const int ENGINEER = 0;
        static const int SALESMAN = 1;
        static const int MANAGER = 2;
        Employee(int type) {
            _type = type;
        }
    };
    ```

    ```cpp
    class Employee {
    public:
        static const int ENGINEER = 0;
        static const int SALESMAN = 1;
        static const int MANAGER = 2;
        virtual int getType() = 0;
        Employee* create(int type);

    protected:
        Employee() {}
        Employee(int type) {
            _type = type;
        }
    private:
        int _type;
    };
    class Engineer : public Employee {
    public:
        virtual int getType() {
            return Employee::ENGINEER;
        }
    };
    class Salesman : public Employee {
    public:
        virtual int getType() {
            return Employee::SALESMAN;
        }
    };
    class Manager : public Employee {
    public:
        virtual int getType() {
            return Employee::MANAGER;
        }
    };
    Employee* Employee::create(int type) {
        switch (type) {
            case ENGINEER:
                return new Engineer();
            case SALESMAN:
                return new Salesman();
            case MANAGER:
                return new Manager();
        }
    }
    TEST(A, B)
    {
    }
    ```