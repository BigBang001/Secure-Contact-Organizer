#include <iostream>
#include <regex>
#include <vector>

class Contact {
protected:
    std::string name;
    std::string phone;

public:
    Contact(const std::string& name, const std::string& phone)
        : name(name), phone(phone) {}

    virtual void display() const = 0;
    virtual void share() const = 0;
    virtual bool validate() const = 0;

    const std::string& getName() const {
        return name;
    }

    const std::string& getPhone() const {
        return phone;
    }
};

class PersonalContact : public Contact {
public:
    PersonalContact(const std::string& name, const std::string& phone)
        : Contact(name, phone) {}

    void display() const override {
        std::cout << "Personal Contact: Name - " << name << ", Phone - " << phone << std::endl;
    }

    void share() const override {
        std::cout << "Sharing Personal Contact: " << name << " via email..." << std::endl;
    }

    bool validate() const override {
        std::regex phoneRegex(R"(\d{10})");
        return std::regex_match(phone, phoneRegex);
    }
};

class BusinessContact : public Contact {
private:
    std::string company;

public:
    BusinessContact(const std::string& name, const std::string& phone, const std::string& company)
        : Contact(name, phone), company(company) {}

    void display() const override {
        std::cout << "Business Contact: Name - " << name << ", Phone - " << phone << ", Company - " << company << std::endl;
    }

    void share() const override {
        std::cout << "Sharing Business Contact: " << name << " via email..." << std::endl;
    }

    bool validate() const override {
        std::regex phoneRegex(R"(\d{10})");
        return (std::regex_match(phone, phoneRegex) && !company.empty());
    }
};

class FamilyContact : public Contact {
private:
    std::string relation;

public:
    FamilyContact(const std::string& name, const std::string& phone, const std::string& relation)
        : Contact(name, phone), relation(relation) {}

    void display() const override {
        std::cout << "Family Contact: Name - " << name << ", Phone - " << phone << ", Relation - " << relation << std::endl;
    }

    void share() const override {
        std::cout << "Sharing Family Contact: " << name << " via text message..." << std::endl;
    }

    bool validate() const override {
        std::regex phoneRegex(R"(\d{10})");
        return (std::regex_match(phone, phoneRegex) && !relation.empty());
    }
};
