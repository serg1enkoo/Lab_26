#include <iostream>
#include <string>
#include <algorithm>

bool isNumber(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

class IPAddress {
protected:
    std::string ip_value;

public:
    IPAddress(const std::string& ip) : ip_value(ip) {}
    IPAddress(const IPAddress& other) : ip_value(other.ip_value) {}
    virtual void print() const {
        std::cout << ip_value;
    }
    virtual ~IPAddress() {}
};

class IPAddressChecked : public IPAddress {
private:
    bool is_correct;

    bool validateIP(const std::string& ip) {
        int dots = 0;
        std::string current_segment;
        int segment_value;
        int segments_count = 0;

        std::string temp_ip = ip + ".";

        for (char c : temp_ip) {
            if (c == '.') {
                dots++;
                if (current_segment.empty() || !isNumber(current_segment)) {
                    return false;
                }
                try {
                    segment_value = std::stoi(current_segment);
                } catch (...) {
                    return false;
                }

                if (segment_value < 0 || segment_value > 255) {
                    return false;
                }
                segments_count++;
                current_segment = "";
            } else {
                current_segment += c;
            }
        }

        if (dots != 4 || segments_count != 4) {
            return false;
        }

        return true;
    }

public:
    IPAddressChecked(const std::string& ip) : IPAddress(ip) {
        is_correct = validateIP(ip_value);
    }
    IPAddressChecked(const IPAddressChecked& other) : IPAddress(other), is_correct(other.is_correct) {}

    void print() const override {
        IPAddress::print();
        if (is_correct) {
            std::cout << " Correct";
        } else {
            std::cout << " Not Correct";
        }
    }
    ~IPAddressChecked() {}
};

int main() {
    std::string ip_str1, ip_str2, ip_str3;

    std::cin >> ip_str1;
    std::cin >> ip_str2;
    std::cin >> ip_str3;

    IPAddress ip_obj1(ip_str1);
    IPAddressChecked ip_obj2_checked(ip_str2);
    IPAddressChecked ip_obj3_checked(ip_str3);

    ip_obj1.print();
    std::cout << std::endl;

    ip_obj2_checked.print();
    std::cout << std::endl;

    ip_obj3_checked.print();
    std::cout << std::endl;

    return 0;
}
