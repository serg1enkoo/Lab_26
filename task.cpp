#include <iostream> // Для операцій вводу-виводу (std::cout, std::cin, std::endl)
#include <string>   // Для використання рядків (std::string)
// #include <vector> // Більше не потрібен для validateIP у цій версії
// #include <sstream>  // Більше не потрібен для validateIP у цій версії
#include <algorithm> // Все ще може бути корисним, але спробуємо обійтися для validateIP

// Функція для перевірки, чи є рядок числом
bool isNumber(const std::string& s) {
    if (s.empty()) return false; // Порожній рядок не є числом
    for (char c : s) {           // Перебираємо кожен символ у рядку
        if (!isdigit(c)) {       // Якщо символ не є цифрою
            return false;        // Рядок не є числом
        }
    }
    return true;                 // Якщо всі символи - цифри, то рядок є числом
}

// Базовий клас для зберігання IP-адреси
class IPAddress {
protected:                       // Захищений член, доступний для похідних класів
    std::string ip_value;        // Рядок для зберігання IP-адреси

public:
    // Конструктор, що приймає рядок IP-адреси
    IPAddress(const std::string& ip) : ip_value(ip) {} // Ініціалізуємо ip_value

    // Конструктор копіювання
    IPAddress(const IPAddress& other) : ip_value(other.ip_value) {} // Копіюємо ip_value від іншого об'єкта

    // Віртуальний метод для виведення IP-адреси
    virtual void print() const {
        std::cout << ip_value; // Виводимо значення IP-адреси
    }

    // Віртуальний деструктор (хороша практика для базових класів з віртуальними методами)
    virtual ~IPAddress() {} // Порожній деструктор
};

// Похідний клас, що додає перевірку коректності IP-адреси
class IPAddressChecked : public IPAddress {
private:                        // Приватні члени, доступні тільки всередині цього класу
    bool is_correct;            // Логічне значення: true, якщо IP коректний, false - якщо ні

    // Спрощений приватний допоміжний метод для перевірки коректності IP-адреси
    bool validateIP(const std::string& ip) {
        int dots = 0;                     // Лічильник крапок
        std::string current_segment;      // Поточний сегмент IP-адреси (число між крапками)
        int segment_value;                // Числове значення поточного сегмента
        int segments_count = 0;           // Лічильник коректних сегментів

        // Додамо крапку в кінець IP, щоб обробити останній сегмент в циклі
        std::string temp_ip = ip + ".";

        for (char c : temp_ip) {          // Перебираємо кожен символ у тимчасовому рядку IP
            if (c == '.') {               // Якщо зустріли крапку
                dots++;                   // Збільшуємо лічильник крапок
                if (current_segment.empty() || !isNumber(current_segment)) { // Якщо сегмент порожній або не є числом
                    return false;         // IP некоректний
                }
                try {
                    segment_value = std::stoi(current_segment); // Конвертуємо рядок сегмента в число
                } catch (const std::out_of_range& oor) {
                    return false; // Число занадто велике/мале
                } catch (const std::invalid_argument& ia) {
                    return false; // Не вдалося конвертувати (хоча isNumber має це покрити)
                }

                if (segment_value < 0 || segment_value > 255) { // Якщо значення сегмента поза діапазоном [0, 255]
                    return false;         // IP некоректний
                }
                segments_count++;         // Збільшуємо лічильник коректних сегментів
                current_segment = "";     // Очищуємо поточний сегмент для наступного числа
            } else {
                current_segment += c;     // Додаємо символ до поточного сегмента
            }
        }

        // IP-адреса повинна мати рівно 4 сегменти (що означає 3 крапки у вихідному IP, або 4 крапки у temp_ip)
        // і ми маємо успішно розібрати 4 сегменти
        if (dots != 4 || segments_count != 4) {
            return false; // Якщо кількість крапок або розібраних сегментів не 4, IP некоректний
        }

        return true; // Якщо всі перевірки пройдені, IP-адреса коректна
    }

public:
    // Конструктор, що приймає рядок IP-адреси
    IPAddressChecked(const std::string& ip) : IPAddress(ip) { // Ініціалізуємо базовий клас
        is_correct = validateIP(ip_value); // Перевіряємо IP-адресу при створенні об'єкта
    }

    // Конструктор копіювання
    IPAddressChecked(const IPAddressChecked& other) : IPAddress(other), is_correct(other.is_correct) {
        // Копіюємо дані базового класу та поле is_correct
    }

    // Перевизначений метод print для виведення IP-адреси та її коректності
    void print() const override {
        IPAddress::print(); // Викликаємо метод print() базового класу для виведення IP-адреси
        if (is_correct) {   // Якщо IP-адреса коректна
            std::cout << " Correct"; // Додаємо " Correct"
        } else {            // Якщо IP-адреса некоректна
            std::cout << " Not Correct"; // Додаємо " Not Correct"
        }
    }

    // Деструктор
    ~IPAddressChecked() {} // Порожній деструктор
};

// Головна функція програми
int main() {
    std::string ip_str1, ip_str2, ip_str3; // Рядки для зберігання введених IP-адрес

    // Отримуємо три IP-адреси від користувача
    std::cin >> ip_str1;                    // Зчитуємо першу IP-адресу
    std::cin >> ip_str2;                    // Зчитуємо другу IP-адресу
    std::cin >> ip_str3;                    // Зчитуємо третю IP-адресу

    // Створюємо один об'єкт IPAddress
    IPAddress ip_obj1(ip_str1); // Створення об'єкта базового класу

    // Створюємо два об'єкти IPAddressChecked
    IPAddressChecked ip_obj2_checked(ip_str2); // Створення першого об'єкта похідного класу
    IPAddressChecked ip_obj3_checked(ip_str3); // Створення другого об'єкта похідного класу

    // Виводимо інформацію для всіх трьох об'єктів
    ip_obj1.print();                       // Викликаємо print() для першого об'єкта
    std::cout << std::endl;                // Перехід на новий рядок

    ip_obj2_checked.print();               // Викликаємо print() для другого об'єкта
    std::cout << std::endl;                // Перехід на новий рядок

    ip_obj3_checked.print();               // Викликаємо print() для третього об'єкта
    std::cout << std::endl;                // Перехід на новий рядок

    return 0; // Повертаємо 0, що означає успішне завершення програми
}
