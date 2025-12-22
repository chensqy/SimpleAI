#include "util.h"
#include <algorithm>
#include <cctype>
#include <random>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <fstream>

namespace {
    void y(std::string& a, const std::string& b, const std::string& c) {
        size_t d = 0;
        while ((d = a.find(b, d)) != std::string::npos) {
            a.replace(d, b.size(), c);
            d += c.size();
        }
    }
    std::mt19937 g((unsigned)std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

namespace u {
    std::string l(const std::string& a) {
        std::string b = a;
        std::transform(b.begin(), b.end(), b.begin(), [](unsigned char c){ return std::tolower(c); });
        return b;
    }
    std::string p(const std::string& a) {
        size_t b = 0;
        size_t c = a.size();
        while (b < c && std::isspace((unsigned char)a[b])) b++;
        while (c > b && std::isspace((unsigned char)a[c - 1])) c--;
        return a.substr(b, c - b);
    }
    bool c(const std::string& a, const std::string& b) {
        return a.find(b) != std::string::npos;
    }
    std::string n() {
        auto a = std::chrono::system_clock::now();
        std::time_t b = std::chrono::system_clock::to_time_t(a);
        std::tm c = *std::localtime(&b);
        std::ostringstream d;
        d << std::put_time(&c, "%Y-%m-%d %H:%M:%S");
        return d.str();
    }
    int r(int a, int b) {
        std::uniform_int_distribution<int> c(a, b);
        return c(g);
    }
    std::string x(std::string a) {
        y(a, "你们", "@1@");
        y(a, "我们", "@2@");
        y(a, "你的", "@3@");
        y(a, "我的", "@4@");
        y(a, "你", "@5@");
        y(a, "我", "@6@");
        y(a, "@1@", "我们");
        y(a, "@2@", "你们");
        y(a, "@3@", "我的");
        y(a, "@4@", "你的");
        y(a, "@5@", "我");
        y(a, "@6@", "你");
        return a;
    }
    void q(const std::string& a) {
        std::ofstream b("chat.log", std::ios::app | std::ios::binary);
        if (b) b << n() << " " << a << "\n";
    }
}
