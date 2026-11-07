#include <iostream>
#include <stdexcept>
#include <string>

// Власні класи винятків
class InvalidAmountException : public std::runtime_error {
public:
    explicit InvalidAmountException(const std::string& message)
        : std::runtime_error(message) {}
};

class InsufficientFundsException : public std::runtime_error {
public:
    explicit InsufficientFundsException(const std::string& message)
        : std::runtime_error(message) {}
};

class LimitExceededException : public std::runtime_error {
public:
    explicit LimitExceededException(const std::string& message)
        : std::runtime_error(message) {}
};

// Клас банкомату
class ATM {
private:
    int balance;
    int dailyLimit;
    int withdrawnToday;

public:
    ATM(int initialBalance, int limit)
        : balance(initialBalance), dailyLimit(limit), withdrawnToday(0) {
    }

    void withdraw(int amount) {
        if (amount % 100 != 0) {
            throw InvalidAmountException("Помилка: сума має бути кратною 100.");
        }

        if (amount > balance) {
            throw InsufficientFundsException("Помилка: недостатньо коштів на балансі.");
        }

        if (withdrawnToday + amount > dailyLimit) {
            throw LimitExceededException("Помилка: перевищено добовий ліміт зняття коштів.");
        }

        balance -= amount;
        withdrawnToday += amount;

        std::cout << "Операція успішна! Ви зняли: " << amount << " грн.\n";
        std::cout << "Залишок на балансі: " << balance << " грн.\n";
    }

    
    int getBalance() const { return balance; }
    int getWithdrawnToday() const { return withdrawnToday; }
    int getDailyLimit() const { return dailyLimit; }
};

int main() {
    
    std::setlocale(LC_ALL, "Ukrainian");

    ATM atm(10000, 5000);

    std::cout << "=== БАНКОМАТ ===\n";
    std::cout << "Баланс: " << atm.getBalance() << " грн\n";
    std::cout << "Добовий ліміт: " << atm.getDailyLimit() << " грн\n\n";

    int amount;
    std::cout << "Введіть суму для зняття: ";
    if (!(std::cin >> amount)) {
        std::cout << "Некоректне введення!" << std::endl;
        return 1;
    }

    try {
        atm.withdraw(amount);
    }
    catch (const InvalidAmountException& e) {
        std::cerr << "[InvalidAmountException] " << e.what() << '\n';
    }
    catch (const InsufficientFundsException& e) {
        std::cerr << "[InsufficientFundsException] " << e.what() << '\n';
    }
    catch (const LimitExceededException& e) {
        std::cerr << "[LimitExceededException] " << e.what() << '\n';
    }
    catch (const std::exception& e) {
        std::cerr << "Непередбачена помилка: " << e.what() << '\n';
    }

    return 0;
}
//31.10.26 practica
#include <string>
#include <stdexcept>
#include <cctype>


int toInt(const std::string& s) {
    if (s.empty()) {
        throw std::invalid_argument("Помилка: порожній рядок не може бути перетворений у число.");
    }

    size_t start = 0;
    if (s[0] == '+' || s[0] == '-') {
        if (s.length() == 1) {
            throw std::invalid_argument("Помилка: рядок містить лише знак.");
        }
        start = 1;
    }

    for (size_t i = start; i < s.length(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) {
            throw std::invalid_argument("Помилка: рядок містить нечислові символи: " + s);
        }
    }

    try {
        return std::stoi(s);
    }
    catch (const std::out_of_range&) {
        throw std::out_of_range("Помилка: число виходить за межі діапазону типу int.");
    }
}



void validatePassword(const std::string& password) {
    if (password.length() < 8) {
        throw std::invalid_argument("Помилка: пароль занадто короткий (мінімум 8 символів).");
    }

    bool hasDigit = false;
    for (char ch : password) {
        if (std::isdigit(static_cast<unsigned char>(ch))) {
            hasDigit = true;
            break;
        }
    }

    if (!hasDigit) {
        throw std::invalid_argument("Помилка: пароль повинен містити хоча б одну цифру.");
    }
}



class AgeException : public std::runtime_error {
private:
    int invalidAge;

public:
    explicit AgeException(int age)
        : std::runtime_error("Недостатній вік для голосування: " + std::to_string(age) + " років."),
        invalidAge(age) {
    }

    int getAge() const {
        return invalidAge;
    }
};

void vote(const std::string& candidate, int age) {
    if (age < 18) {
        throw AgeException(age);
    }
    std::cout << "Голос за кандидата '" << candidate << "' успішно зараховано! (Вік виборця: " << age << ")\n";
}



int main() {
    std::setlocale(LC_ALL, "Ukrainian");

    std::cout << "=== ТЕСТУВАННЯ ЗАВДАННЯ 1 (toInt) ===\n";
    try {
        std::string testStr = "12345";
        std::cout << "Рядок \"" << testStr << "\" -> Число: " << toInt(testStr) << "\n";

        
        std::string invalidStr = "123a45";
        std::cout << "Спроба перетворити \"" << invalidStr << "\":\n";
        toInt(invalidStr);
    }
    catch (const std::exception& e) {
        std::cerr << "[Спіймано виняток] " << e.what() << "\n\n";
    }


    std::cout << "=== ТЕСТУВАННЯ ЗАВДАННЯ 2 (validatePassword) ===\n";
    try {
        std::string pwd = "abc"; 
        std::cout << "Перевірка пароля \"" << pwd << "\":\n";
        validatePassword(pwd);
    }
    catch (const std::exception& e) {
        std::cerr << "[Спіймано виняток] " << e.what() << "\n\n";
    }


    std::cout << "=== ТЕСТУВАННЯ ЗАВДАННЯ 3 (vote) ===\n";
    try {
        std::string candidate = "Кандидат А";
        int age = 16; // Менше 18
        std::cout << "Спроба проголосувати (вік " << age << "):\n";
        vote(candidate, age);
    }
    catch (const AgeException& e) {
        std::cerr << "[Спіймано AgeException] " << e.what() << " (Переданий вік: " << e.getAge() << ")\n";
    }
    catch (const std::exception& e) {
        std::cerr << "[Спіймано загальний виняток] " << e.what() << "\n";
    }

    return 0;
}
//01.11.26 samoctiuna 
//1
#include <iostream>
#include <typeinfo>


template <typename T>
void printTypeName(const T& value) {
    std::cout << "Type name: " << typeid(T).name() << std::endl;
}

int main() {
    int a = 10;
    double b = 5.5;
    const char* c = "Hello";

    printTypeName(a); 
    printTypeName(b); 
    printTypeName(c); 

    return 0;
}
//2
#include <iostream>

void printCharFromInt(int charCode) {

    char* charPtr = reinterpret_cast<char*>(static_cast<uintptr_t>(charCode));

  
    char symbol = *charPtr;

    std::cout << "Code " << charCode << " corresponds to character: " << symbol << std::endl;
}

int main() {
    
    printCharFromInt(65);
    printCharFromInt(97);
    printCharFromInt(36);

    return 0;
}
//01.11.26 practica 
#include <iostream>

int convertDoubleToInt(double value) {
    return static_cast<int>(value);
}

int main() {
    double num = 45.89;
    int result = convertDoubleToInt(num);

    std::cout << "Original double: " << num << "\n";
    std::cout << "Converted int: " << result << "\n"; // Дробова частина відкинеться

    return 0;
}
#include <iostream>
#include <cmath>

class Shape {
public:
    virtual double getPerimeter() const = 0; // Чисто віртуальний метод
    virtual ~Shape() = default;
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}

    double getPerimeter() const override {
        return 2 * 3.1415926535 * radius;
    }
};

int main() {
    Circle circle(5.0);

    Shape* shapePtr = static_cast<Shape*>(&circle);
    std::cout << "Perimeter via Shape* (Upcast): " << shapePtr->getPerimeter() << "\n";


    Circle* circlePtr = static_cast<Circle*>(shapePtr);
    std::cout << "Perimeter via Circle* (Downcast): " << circlePtr->getPerimeter() << "\n";

    return 0;
}
#include <iostream>

void convertVoidToChar(void* ptr) {
    char* charPtr = static_cast<char*>(ptr);
    std::cout << "Converted char pointer value: " << *charPtr << "\n";
}

int main() {
    char letter = 'Z';
    void* voidPtr = &letter;

    convertVoidToChar(voidPtr);

    return 0;
}