#include <iostream>
#include <regex>
#include "contact.h"

// Contact base class constructor
Contact::Contact(const std::string& name, const std::string& phone)
    : name(name), phone(phone) {}

// PersonalContact constructor
PersonalContact::PersonalContact(const std::string& name, const std::string& phone)
    : Contact(name, phone) {}

// Display personal contact information
void PersonalContact::display() const {
    std::cout << "Personal Contact: Name - " << name << ", Phone - " << phone << std::endl;
}

// Share personal contact information
void PersonalContact::share() const {
    std::cout << "Sharing Personal Contact: " << name << " via email..." << std::endl;
}

// Validate personal contact information
bool PersonalContact::validate() const {
    std::regex phoneRegex(R"(\d{10})");
    return std::regex_match(phone, phoneRegex);
}

// BusinessContact constructor
BusinessContact::BusinessContact(const std::string& name, const std::string& phone, const std::string& company)
    : Contact(name, phone), company(company) {}

// Display business contact information
void BusinessContact::display() const {
    std::cout << "Business Contact: Name - " << name << ", Phone - " << phone << ", Company - " << company << std::endl;
}

// Share business contact information
void BusinessContact::share() const {
    std::cout << "Sharing Business Contact: " << name << " via email..." << std::endl;
}

// Validate business contact information
bool BusinessContact::validate() const {
    std::regex phoneRegex(R"(\d{10})");
    return (std::regex_match(phone, phoneRegex) && !company.empty());
}

// FamilyContact constructor
FamilyContact::FamilyContact(const std::string& name, const std::string& phone, const std::string& relation)
    : Contact(name, phone), relation(relation) {}

// Display family contact information
void FamilyContact::display() const {
    std::cout << "Family Contact: Name - " << name << ", Phone - " << phone << ", Relation - " << relation << std::endl;
}

// Share family contact information
void FamilyContact::share() const {
    std::cout << "Sharing Family Contact: " << name << " via text message..." << std::endl;
}

// Validate family contact information
bool FamilyContact::validate() const {
    std::regex phoneRegex(R"(\d{10})");
    return (std::regex_match(phone, phoneRegex) && !relation.empty());
}
