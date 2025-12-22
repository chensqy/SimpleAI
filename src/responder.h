#ifndef RESPONDER_H
#define RESPONDER_H
#include <string>
#include <vector>
struct B {
    std::vector<std::string> a;
    B();
    std::string b();
    std::string c(const std::string& a);
    std::string d(const std::string& a);
};
#endif
