#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stack>
#include "contact.cpp"

class Phonebook {
private:
    std::vector<Contact*> contacts;
    std::unordered_map<std::string, std::vector<Contact*>> contactGroups;
    std::vector<std::string> fileHistory;
    std::stack<Contact*> deletedContacts;

public:
    const std::vector<Contact*>& getContacts() const {
        return contacts;
    }

    void addContact(Contact* contact, const std::string& group) {
        contacts.push_back(contact);
        contactGroups[group].push_back(contact);
    }

    void displayContacts() const {
        for (size_t i = 0; i < contacts.size(); ++i) {
            std::cout << "Contact " << i << ": ";
            contacts[i]->display();
        }
    }

    void shareContact(int index) const {
        if (index >= 0 && static_cast<size_t>(index) < contacts.size()) {
            contacts[index]->share();
        } else {
            std::cout << "Invalid contact index." << std::endl;
        }
    }

    void displayGroup(const std::string& group) const {
        if (contactGroups.find(group) != contactGroups.end()) {
            std::cout << "Contacts in Group - " << group << std::endl;
            for (Contact* contact : contactGroups[group]) {
                contact->display();
            }
        } else {
            std::cout << "Group not found." << std::endl;
        }
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);

        if (file.is_open()) {
            for (Contact* contact : contacts) {
                file << contact->getName() << "," << contact->getPhone() << "\n";
            }
            file.close();
            std::cout << "Phonebook saved to " << filename << std::endl;

            fileHistory.push_back("Saved: " + filename);
        } else {
            std::cout << "Error: Unable to open the file for writing." << std::endl;
        }
    }

    void loadFromFile(const std::string& filename) {
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

    std::vector<std::string> getFileHistory() const {
        return fileHistory;
    }

    void deleteContact(int index) {
        if (index >= 0 && static_cast<size_t>(index) < contacts.size()) {
            deletedContacts.push(contacts[index]);
            contacts.erase(contacts.begin() + index);
            std::cout << "Contact deleted successfully." << std::endl;
        } else {
            std::cout << "Invalid contact index." << std::endl;
        }
    }

    void undoDeleteContact() {
        if (!deletedContacts.empty()) {
            Contact* contactToRestore = deletedContacts.top();
            deletedContacts.pop();
            contacts.push_back(contactToRestore);
            std::cout << "Contact restored successfully." << std::endl;
        } else {
            std::cout << "No contacts to restore." << std::endl;
        }
    }

    ~Phonebook() {
        for (Contact* contact : contacts) {
            delete contact;
        }
        contacts.clear();
    }
};
