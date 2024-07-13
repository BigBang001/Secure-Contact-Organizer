#include "user.h"

// Security questions list (static)
const std::vector<SecurityQuestion> User::securityQuestions = {
    {"What is your favorite book?", "book"},
    {"What is your mother's maiden name?", "mother"},
    {"Where did you go for your studies?", "studies"},
    {"In which city were you born?", "city"}
};

User::User(const std::string& uname, const std::string& pword, const SecurityQuestion& secQ)
    : username(uname), password(pword), securityQuestion(secQ) {}

bool User::authenticate(const std::string& uname, const std::string& pword) const {
    return (uname == username && pword == password);
}

bool User::resetPassword(const std::string& answer, const std::string& newPass) {
    if (answer == securityQuestion.answer) {
        password = newPass;
        return true;
    }
    return false;
}

const std::string& User::getSecurityQuestion() const {
    return securityQuestion.question;
}
