#pragma once
#include <iostream>
#include <string>

// ѕространство имЄн дл€ Unit-тестов
namespace unit_tests {

// ‘ункци€ сравнени€ объектов дл€ макроса сравнени€ выражений
template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const std::string& t_str, const std::string& u_str, const std::string& file, const std::string& func, unsigned line, const std::string& hint) {
    using namespace std;
    if (t != u) {
        cerr << boolalpha;
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cerr << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

// ћакрос сравнени€ выражений
#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

// ћакрос сравнени€ выражений со строковой подсказкой
#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

// ‘ункци€ сравнени€ (неравенства) объектов дл€ макроса сравнени€ выражений
template <typename T, typename U>
void AssertNotEqualImpl(const T& t, const U& u, const std::string& t_str, const std::string& u_str, const std::string& file, const std::string& func, unsigned line, const std::string& hint) {
    using namespace std;
    if (t == u) {
        cerr << boolalpha;
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT_NOT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cerr << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

// ћакрос сравнени€ (неравенства) выражений
#define ASSERT_NOT_EQUAL(a, b) AssertNotEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

// ћакрос сравнени€ (неравенства) выражений со строковой подсказкой
#define ASSERT_NOT_EQUAL_HINT(a, b, hint) AssertNotEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

// ‘ункци€ проверки логического значени€ дл€ макроса проверки логического выражени€
inline void AssertImpl(bool value, const std::string& expr_str, const std::string& file, const std::string& func, unsigned line, const std::string& hint) {
    using namespace std;
    if (!value) {
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

// ћакрос проверки логического выражени€
#define ASSERT(expr) AssertImpl((static_cast<bool>(expr)), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

// ћакрос проверки логического выражени€ со строковой подсказкой
#define ASSERT_HINT(expr, hint) AssertImpl((static_cast<bool>(expr)), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

// ‘ункци€ сравнени€ действительных чисел дл€ макроса сравнени€ выражений со значени€ми действительных чисел (с точностью до epsilon)
inline void AssertEqualEpsImpl(const double eps, const double& t, const double& u, const std::string& t_str, const std::string& u_str, const std::string& file, const std::string& func, unsigned line, const std::string& hint) {
    using namespace std;
    if (abs(t - u) >= eps) {
        cerr << boolalpha;
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT_EQUAL_EPS("s << t_str << ", "s << u_str << ", "s << eps << ") failed: "s;
        cerr << t << " != "s << u << " with eps = "s << eps << "."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

// ћакрос сравнени€ выражений со значени€ми действительных чисел (с точностью до epsilon)
#define ASSERT_EQUAL_EPS(a, b, eps) AssertEqualEpsImpl((eps), (a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

// ћакрос сравнени€ выражений со значени€ми действительных чисел (с точностью до epsilon) со строковой подсказкой
#define ASSERT_EQUAL_EPS_HINT(a, b, eps, hint) AssertEqualEpsImpl((eps), (a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

// ‘ункци€ запуска unit-теста дл€ макроса запуска unit-теста
template <typename TestFunction>
void RunTestImpl(TestFunction test_function, const std::string& test_function_name) {
    using namespace std;
    test_function();
    cerr << test_function_name << " OK"s << endl;
}

// ћакрос запуска unit-теста
#define RUN_TEST(func) RunTestImpl(func, #func)

}