// =====================================================================================
// PerfectForwarding.cpp // Perfect Forwarding
// =====================================================================================


#include <functional>
#include <iostream>
#include <map>
#include <print>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

namespace PerfectForwarding {

    static void overloaded(int& arg) {
        std::println("By lvalue: {}", arg);
    }

    static void overloaded(int&& arg) {
        std::println("By rvalue: {}", arg);
    }

    /*
     * Note: "T&&" with "T" being template parameter is special: Universal Reference
     */

    template <typename T>
    void forwarding(T&& arg) {
        overloaded(arg);
    }





    template <typename T>
    void forwardingPerfect(T&& arg) {
        overloaded(std::forward<T>(arg));
    }

    static void test_forwarding() {

        int n{ 123 };

        forwarding(n);

        forwarding(456);
    }

    static void test_forwardingPerfect() {

        int n{ 123 };

        forwardingPerfect(n);

        forwardingPerfect(456);
    }

    // =================================================================================

    template <typename T, typename U>
    void foo(T&& arg1, U&& arg2)
    {
        // Beobachte den Inhalt der beiden Parameter 'arg1' und 'arg2'

        // T obj1 = std::forward<T>(arg1);
        // vs
        T obj1 = std::forward<T>(arg1);
        std::println("[{}]", arg1);

        U obj2 = std::forward<U>(arg2);
        std::println("[{}]", arg2);

      //  arg2 += "XYZ";
    }

    static void test_example()
    {
        const std::string s{ "DEF" };

        foo(std::string{ "ABC" }, s);

        std::println("[{}]", s);
    }
}

void main_perfect_forwarding()
{
    using namespace PerfectForwarding;
    test_forwarding();
    test_forwardingPerfect();
    test_example();
}

// =====================================================================================
// End-of-File
// =====================================================================================
