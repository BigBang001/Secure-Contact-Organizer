#include "phonebook.h"
#include <iostream>
#include <fstream>
#include <algorithm>

// Get all contacts
const std::vector<Contact*>& Phonebook::getContacts() const {
    return contacts;
}

// Add contact to phonebook
void Phonebook::addContact(Contact* contact, const std::string& group) {
    contacts.push_back(contact);
    contactGroups[group].push_back(contact);
}

// Display all contacts
void Phonebook::displayContacts() const {
    for (size_t i = 0; i < contacts.size(); ++i) {
        std::cout << "Contact " << i << ": ";
        contacts[i]->display();
    }
}

// Share contact at given index
void Phonebook::shareContact(int index) const {
    if (index >= 0 && static_cast<size_t>(index) < contacts.size()) {
        contacts[index]->share();
    } else {
        std::cout << "Invalid contact index." << std::endl;
    }
}

// Display contacts in a specific group
void Phonebook::displayGroup(const std::string& group) const {
    if (contactGroups.find(group) != contactGroups.end()) {
        std::cout << "Contacts in Group - " << group << std::endl;
        for (Contact* contact : contactGroups[group]) {
            contact->display();
        }
    } else {
        std::cout << "Group not found." << std::endl;
    }
}

// Save phonebook to file
void Phonebook::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);

    if (file.is_open()) {
        for (Contact* contact : contacts) {
            file << contact->name << "," << contact->phone << "\n";
        }
        file.close();
        std::cout << "Phonebook saved to " << filename << std::endl;

        fileHistory.push_back("Saved: " + filename);
    } else {
        std::cout << "Error: Unable to open the file for writing." << std::endl;
    }
}

// Load phonebook from file
void Phonebook::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            size_t pos = line.find(",");
            if (pos != std::string::npos) {
                std::string name = line.substr(0, pos);
                std::string phone = line.substr(pos + 1);
                Contact* contact = new PersonalContact(name, phone); // Assume personal contact for simplicity
                contacts.push_back(contact);
            }
        }
        file.close();
        std::cout << "Phonebook loaded from " << filename << std::endl;

        fileHistory.push_back("Loaded: " + filename);
    } else {
        std::cout << "Error: Unable to open the file for reading." << std::endl;
    }
}

// Retrieve file operation history
std::vector<std::string> Phonebook::getFileHistory() const {
    return fileHistory;
}

// Delete contact at given index
void Phonebook::deleteContact(int index) {
    if (index >= 0 && static_cast<size_t>(index) < contacts.size()) {
        deletedContacts.push(contacts[index]);
        contacts.erase(contacts.begin() + index);
        std::cout << "Contact deleted successfully." << std::endl;
    } else {
        std::cout << "Invalid contact index." << std::endl;
    }
}

// Undo last delete operation
void Phonebook::undoDeleteContact() {
    if (!deletedContacts.empty()) {
        Contact* contactToRestore = deletedContacts.top();
        deletedContacts.pop();
        contacts.push_back(contactToRestore);
        std::cout << "Contact restored successfully." << std::endl;
    } else {
        std::cout << "No contacts to restore." << std::endl;
    }
}

// Phonebook destructor to clean up memory
Phonebook::~Phonebook() {
    for (Contact* contact : contacts) {
        delete contact;
    }
    contacts.clear();
}
