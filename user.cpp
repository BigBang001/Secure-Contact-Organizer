#include <iostream>
#include <vector>
#include <string>

class SecurityQuestion {
public:
    std::string question;
    std::string answer;

    SecurityQuestion(const std::string& q, const std::string& a)
        : question(q), answer(a) {}
};

class User {
private:
    std::string username;
    std::string password;
    SecurityQuestion securityQuestion;

public:
    static const std::vector<SecurityQuestion> securityQuestions;

    User(const std::string& uname, const std::string& pword, const SecurityQuestion& secQ)
        : username(uname), password(pword), securityQuestion(secQ) {}

    bool authenticate(const std::string& uname, const std::string& pword) const {
        return (uname == username && pword == password);
    }

    bool resetPassword(const std::string& answer, const std::string& newPass) {
        if (answer == securityQuestion.answer) {
            password = newPass;
            return true;
        }
        return false;
    }

    const std::string& getSecurityQuestion() const {
        return securityQuestion.question;
    }
};

const std::vector<SecurityQuestion> User::securityQuestions = {
    {"What is your favorite book?", "book"},
    {"What is your mother's maiden name?", "mother"},
    {"Where did you go for your studies?", "studies"},
    {"In which city were you born?", "city"}
};
