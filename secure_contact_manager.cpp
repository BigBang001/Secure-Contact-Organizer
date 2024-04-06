#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <conio.h>
#include <regex>
#include <map>

using namespace std;

// Forward declarations
class Contact;
class PersonalContact;
class BusinessContact;
class FamilyContact;
class FileOperations;
class User;
class SecurityQuestion;

// Adding a SecurityQuestion class for user password reset
class SecurityQuestion {
public:
    string question;
    string answer;

    SecurityQuestion(const string& q, const string& a) : question(q), answer(a) {}
};

// Class to represent a user with login and password reset functionality
class User {
private:
    string username;
    string password;
    SecurityQuestion securityQuestion;

public:
    User(const string& uname, const string& pword, const SecurityQuestion& secQ)
        : username(uname), password(pword), securityQuestion(secQ) {}

    bool authenticate(const string& uname, const string& pword) const {
        return (username == uname && password == pword);
    }

    bool resetPassword(const string& answer, const string& newPass) {
        if (securityQuestion.answer == answer) {
            password = newPass; // Update the password with the new one
            return true;
        }
        return false;
    }

    const string& getSecurityQuestion() const {
        return securityQuestion.question;
    }
};

// Base class for different types of contacts
class Contact {
public:
    string name;
    string phone;

    Contact() {}

    Contact(const string& name, const string& phone) : name(name), phone(phone) {}

    virtual void display() = 0;
    virtual void share() = 0;
    virtual bool validate() const = 0;
};

// Personal contact class
class PersonalContact : public Contact {
public:
    PersonalContact() {}

    PersonalContact(const string& name, const string& phone) : Contact(name, phone) {}

    void display() override {
        cout << "Personal Contact: Name - " << name << ", Phone - " << phone << endl;
    }

    void share() override {
        cout << "Sharing Personal Contact: " << name << " via email..." << endl;
    }

    bool validate() const override {
        regex phoneRegex(R"(\d{10})");
        return regex_match(phone, phoneRegex);
    }
};

// Business contact class
class BusinessContact : public Contact {
public:
    string company;

    BusinessContact() {}

    BusinessContact(const string& name, const string& phone, const string& company)
        : Contact(name, phone), company(company) {}

    void display() override {
        cout << "Business Contact: Name - " << name << ", Phone - " << phone << ", Company - " << company << endl;
    }

    void share() override {
        cout << "Sharing Business Contact: " << name << " via email..." << endl;
    }

    bool validate() const override {
        regex phoneRegex(R"(\d{10})");
        return (regex_match(phone, phoneRegex) && !company.empty());
    }
};

// Family contact class
class FamilyContact : public Contact {
public:
    string relation;

    FamilyContact() {}

    FamilyContact(const string& name, const string& phone, const string& relation)
        : Contact(name, phone), relation(relation) {}

    void display() override {
        cout << "Family Contact: Name - " << name << ", Phone - " << phone << ", Relation - " << relation << endl;
    }

    void share() override {
        cout << "Sharing Family Contact: " << name << " via text message..." << endl;
    }

    bool validate() const override {
        regex phoneRegex(R"(\d{10})");
        return (regex_match(phone, phoneRegex) && !relation.empty());
    }
};

// File operations interface
class FileOperations {
public:
    virtual void saveToFile(const string& filename) = 0;
    virtual void loadFromFile(const string& filename) = 0;
    virtual vector<string> getFileHistory() = 0;
};

// Phonebook class with grouping contacts and file history
class Phonebook : public FileOperations {
public:
    vector<Contact*> contacts;
    map<string, vector<Contact*>> contactGroups;
    vector<string> fileHistory;
    stack<Contact*> deletedContacts;  // Stack to store deleted contacts for undo

public:
    const vector<Contact*>& getContacts() const {
        return contacts;
    }

    void addContact(Contact* contact, const string& group) {
        contacts.push_back(contact);
        contactGroups[group].push_back(contact);
    }

    void displayContacts() {
        for (size_t i = 0; i < contacts.size(); ++i) {
            cout << "Contact " << i << ": ";
            contacts[i]->display();
        }
    }

    void shareContact(int index) {
        if (index >= 0 && static_cast<size_t>(index) < contacts.size()) {
            contacts[index]->share();
        } else {
            cout << "Invalid contact index." << endl;
        }
    }

    void displayGroup(const string& group) {
        if (contactGroups.find(group) != contactGroups.end()) {
            cout << "Contacts in Group - " << group << endl;
            for (Contact* contact : contactGroups[group]) {
                contact->display();
            }
        } else {
            cout << "Group not found." << endl;
        }
    }

    void saveToFile(const string& filename) override {
        ofstream file(filename);

        if (file.is_open()) {
            for (Contact* contact : contacts) {
                file << contact->name << "," << contact->phone << "\n";
            }
            file.close();
            cout << "Phonebook saved to " << filename << endl;

            fileHistory.push_back("Saved: " + filename);
        } else {
            cout << "Error: Unable to open the file for writing." << endl;
        }
    }

    void loadFromFile(const string& filename) override {
        ifstream file(filename);

        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                size_t pos = line.find(",");
                if (pos != string::npos) {
                    string name = line.substr(0, pos);
                    string phone = line.substr(pos + 1);
                    Contact* contact = new PersonalContact(name, phone);
                    contacts.push_back(contact);
                }
            }
            file.close();
            cout << "Phonebook loaded from " << filename << endl;
            fileHistory.push_back("Loaded: " + filename);
        } else {
            cout << "Error: Unable to open the file for reading." << endl;
        }
    }

    vector<string> getFileHistory() override {
        return fileHistory;
    }

    void deleteContact(int index) {
        if (index >= 0 && static_cast<size_t>(index) < contacts.size()) {
            // Get the contact from the vector
            Contact* contactToDelete = contacts[index];

            // Remove the contact from the vector
            contacts.erase(contacts.begin() + index);

            // Find the group(s) where this contact might exist and remove it from there
            for (auto& group : contactGroups) {
                group.second.erase(
                    std::remove(group.second.begin(), group.second.end(), contactToDelete),
                    group.second.end()
                );
            }

            // Push the contact to the deletedContacts stack for undo
            deletedContacts.push(contactToDelete);

            cout << "Contact deleted successfully. You can undo this action." << endl;
        } else {
            cout << "Invalid contact index." << endl;
        }
    }

    void undoDeleteContact() {
        if (!deletedContacts.empty()) {
            Contact* contactToUndo = deletedContacts.top();
            contacts.push_back(contactToUndo);
            deletedContacts.pop();

            cout << "Contact undeleted successfully." << endl;
        } else {
            cout << "No contact to undo." << endl;
        }
    }
};

int main() {
    // Create a user with a security question
    SecurityQuestion secQ("What's your pet's name?", "Fluffy");
    User user("user123", "password123", secQ);

    string enteredUsername, enteredPassword;
    bool loggedIn = false;

    while (!loggedIn) {
        system("cls");
        cout << "Login\n";
        cout << "Enter Username: ";
        cin >> enteredUsername;
        cout << "Enter Password: ";
        cin >> enteredPassword;
        loggedIn = user.authenticate(enteredUsername, enteredPassword);

        if (!loggedIn) {
            cout << "Login failed!!" << "\n Please Try Again";
        }
    }

    Phonebook phonebook;
    string filename = "phonebook.txt";

    phonebook.loadFromFile(filename);

    int choice;
    do {
        cout << "\t\t\t||\n";
        cout << "\t\t\t|           PHONE BOOK APPLICATION           | \n";
        cout << "\t\t\t**\n\n";
        cout << "\t\t\t::::::::::::::: PROGRAM  MENU :::::::::::::::\n" << endl;
        cout << "\t\t\t**" << endl;
        cout << "\t\t\t|             [1] Add Personal Contact                   |" << endl;
        cout << "\t\t\t|             [2] Add Business Contact                   |" << endl;
        cout << "\t\t\t|             [3] Add Family Contact                     |" << endl;
        cout << "\t\t\t|             [4] Display Contacts                       |" << endl;
        cout << "\t\t\t|             [5] Share Contact                          |" << endl;
        cout << "\t\t\t|             [6] Save Phonebook                         | " << endl;
        cout << "\t\t\t|             [7] Create Contact Group                   |" << endl;
        cout << "\t\t\t|             [8] Display Group                          |" << endl;
        cout << "\t\t\t|             [9] Password Reset                         |" << endl;
        cout << "\t\t\t|             [10] View File History                     |" << endl;
        cout << "\t\t\t|             [11] Delete contact                         |" << endl;
        cout << "\t\t\t|             [12] Undo Delete Contact                    |" << endl;
        cout << "\t\t\t|             [13] Quit                                   |" << endl;
        cout << "\t\t\t**" << endl;

        cout << "Enter Your Choice  ";
        cin >> choice;

        switch (choice) {
            case 1: {
                system("cls");
                cout << "\t\t\t||\n";
                cout << "\t\t\t|           PHONE BOOK APPLICATION           | \n";
                cout << "\t\t\t**\n\n";
                string name, phone;
                cin.ignore();
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Phone (10 digits): ";
                getline(cin, phone);
                if (!regex_match(phone, regex(R"(\d{10})"))) {
                    cout << "Invalid phone number format. Please enter 10 digits." << endl;
                } else {
                    Contact* contact = new PersonalContact(name, phone);
                    phonebook.addContact(contact, "Personal");
                }
                break;
            }
            case 2: {
                system("cls");
                cout << "\t\t\t||\n";
                cout << "\t\t\t|           PHONE BOOK APPLICATION           | \n";
                cout << "\t\t\t**\n\n";
                string name, phone, company;
                cin.ignore();
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Phone (10 digits): ";
                getline(cin, phone);
                if (!regex_match(phone, regex(R"(\d{10})"))) {
                    cout << "Invalid phone number format. Please enter 10 digits." << endl;
                } else {
                    cout << "Enter Company: ";
                    getline(cin, company);
                    if (company.empty()) {
                        cout << "Company name cannot be empty. Please try again." << endl;
                    } else {
                        Contact* contact = new BusinessContact(name, phone, company);
                        phonebook.addContact(contact, "Business");
                    }
                }
                break;
            }
            case 3: {
                system("cls");
                cout << "\t\t\t||\n";
                cout << "\t\t\t|           PHONE BOOK APPLICATION           | \n";
                cout << "\t\t\t**\n\n";
                string name, phone, relation;
                cin.ignore();
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Phone (10 digits): ";
                getline(cin, phone);
                if (!regex_match(phone, regex(R"(\d{10})"))) {
                    cout << "Invalid phone number format. Please enter 10 digits." << endl;
                } else {
                    cout << "Enter Relation: ";
                    getline(cin, relation);
                    if (relation.empty()) {
                        cout << "Relation cannot be empty. Please try again." << endl;
                    } else {
                        Contact* contact = new FamilyContact(name, phone, relation);
                        phonebook.addContact(contact, "Family");
                    }
                }
                break;
            }
            case 4: {
                system("cls");
                cout << "\t\t\t||\n";
                cout << "\t\t\t|           PHONE BOOK APPLICATION           | \n";
                cout << "\t\t\t**\n\n";
                phonebook.displayContacts();
                getch();
                break;
            }
            case 5: {
                system("cls");
                cout << "\t\t\t||\n";
                cout << "\t\t\t|           PHONE BOOK APPLICATION           | \n";
                cout << "\t\t\t**\n\n";
                int index;
                cout << "Enter the index of the contact to share: ";
                cin >> index;
                phonebook.shareContact(index);
                getch();
                break;
            }
            case 6: {
                system("cls");
                cout << "\t\t\t||\n";
                cout << "\t\t\t|           PHONE BOOK APPLICATION           | \n";
                cout << "\t\t\t**\n\n";
                phonebook.saveToFile(filename);
                getch();
                break;
            }
            case 7: {
                system("cls");
                cout << "\t\t\t||\n";
                cout << "\t\t\t|           PHONE BOOK APPLICATION           | \n";
                cout << "\t\t\t**\n\n";
                cin.ignore();
                string group;
                cout << "Enter the name of the new contact group: ";
                getline(cin, group);

                if (!group.empty()) {
                    phonebook.contactGroups[group] = vector<Contact*>();
                    cout << "Contact group '" << group << "' created." << endl;

                    // Allow the user to add contacts to the created group
                    int addMore;
                    do {
                        cout << "Do you want to add contacts to this group? (1 for yes, 0 for no): ";
                        cin >> addMore;

                        if (addMore) {
                            int contactIndex;
                            cout << "Enter the index of the contact to add to group '" << group << "': ";
                            cin >> contactIndex;

                            if (contactIndex >= 0 && static_cast<size_t>(contactIndex) < phonebook.contacts.size()) {
                                phonebook.contactGroups[group].push_back(phonebook.contacts[contactIndex]);
                                cout << "Contact added to group '" << group << "'." << endl;
                            } else {
                                cout << "Invalid contact index." << endl;
                            }
                        }
                    } while (addMore);
                } else {
                    cout << "Group name cannot be empty. Please try again." << endl;
                }
                getch();
                break;
            }
            case 8: {
                system("cls");
                cout << "\t\t\t||\n";
                cout << "\t\t\t|           PHONE BOOK APPLICATION           | \n";
                cout << "\t\t\t**\n\n";
                cin.ignore();
                string group;
                cout << "Enter the name of the contact group to display: ";
                getline(cin, group);
                phonebook.displayGroup(group);
                getch();
                break;
            }
            case 9: {
                system("cls");
                cout << "\t\t\t||\n";
                cout << "\t\t\t|           PHONE BOOK APPLICATION           | \n";
                cout << "\t\t\t**\n\n";

                cin.ignore();
                string answer, newPassword;
                cout << "Security Question: " << user.getSecurityQuestion() << endl;
                cout << "Enter the answer to reset your password: ";
                getline(cin, answer);
                cout << "Enter your new password: ";
                getline(cin, newPassword);
                if (user.resetPassword(answer, newPassword)) {
                    cout << "Password reset successfully." << endl;
                } else {
                    cout << "Password reset failed. Security answer is incorrect." << endl;
                }
                getch();
                break;
            }
            case 10: {
                system("cls");
                cout << "\t\t\t||\n";
                cout << "\t\t\t|           PHONE BOOK APPLICATION           | \n";
                cout << "\t\t\t**\n\n";
                vector<string> history = phonebook.getFileHistory();
                cout << "File History: " << endl;
                for (const string& entry : history) {
                    cout << entry << endl;
                }
                getch();
                break;
            }
            case 11: {
                int index;
                cout << "Enter the index of the contact to delete: ";
                cin >> index;
                phonebook.deleteContact(index);
                getch();
                break;
            }
            case 12: {
                system("cls");
                cout << "\t\t\t||\n";
                cout << "\t\t\t|           PHONE BOOK APPLICATION           | \n";
                cout << "\t\t\t**\n\n";
                phonebook.undoDeleteContact();
                getch();
                break;
            }
            case 13: {
                system("cls");
                cout << "\t\t\t||\n";
                cout << "\t\t\t|           PHONE BOOK APPLICATION           | \n";
                cout << "\t\t\t**\n\n";
                cout << "Goodbye!" << endl;
                break;
            }
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 13);

    // Clean up dynamic memory
    for (Contact* contact : phonebook.getContacts()) {
        delete contact;
    }

    return 0;
}
