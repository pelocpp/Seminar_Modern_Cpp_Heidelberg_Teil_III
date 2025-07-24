// =====================================================================================
// Folding.cpp // Variadic Templates // Folding
// =====================================================================================

#include "ScopedTimer.h"

#include <functional>
#include <iostream>
#include <map>
#include <print>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

namespace Folding_Seminar {

    // C++ 17
    //template <typename ... TArgs>
    //auto addierer(TArgs ... args)
    //{
    //    auto result = ( ... + args ) ;

    //    return 0;
    //}

    // C++ 20
    auto addierer(auto ... args)
    {
        return (... + args);
    }

    auto subtrahierer (auto ... args)
    {
        // 1 - 2 - 3:
        // Ergebnis: 1 - (-1) = +2  // von rechts nach links
        // Ergebnis: (-1) -3 = -4   // von links nach rechts

        return (args - ...);
    }

    void printer (auto first, auto ... args)
    {
        // std::cout << args1 << args2 << args3 << ...
    
        // (std::cout << ... << args  << " = ");

        std::cout << first;

        ( ...  ,  ( std::cout << " = " << args ) );
    }

    void printerExtended(auto ... first, auto ... args)
    {
        // std::cout << args1 << args2 << args3 << ...

        // (std::cout << ... << args  << " = ");

        (..., (std::cout << " = " << first));

        std::cout << std::endl;

        (..., (std::cout << " = " << args));
    }


    void test_folding() {

        // auto result = addierer(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    
        // auto result = subtrahierer(1, 2, 3);

     //   printer(123, "ABC", std::string("XYZ"), 123.4657, '!');
    
        printerExtended(123, "ABC", std::string("XYZ"), 123.4657, '!', "Last");


        int a, b, c;

        a = 1;
        b = 2;

        // Jedes Statement hat einen Wert !!!
        // Am Ende eines Statements steht im Akku ein Wert

       // c = (b = (a = 1));

       // c = (a = 1 , b = 2);  // Komma:  Sequenz-Operator
    
    }

}

namespace Folding {

    /* folding examples: introduction
    */

    template<typename... TArgs>
    static auto anotherAdder(TArgs... args) {
        return (... + args);  // unary left fold
    }

    static void test_01() {
        int sum = anotherAdder(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        std::cout << "Sum from 1 up to 10: " << sum << std::endl;
    }

    template<typename... TArgs>
    static void printer(TArgs... args) {
        // binary left fold (init == ostream)
        (std::cout << ... << args) << std::endl;
    }

    // demonstrating fold expressions
    static void test_02() {
        printer(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // =================================================================================
    /* demonstrating all four versions of folding expressions
    */

    template<typename... TArgs>
    static auto anotherSubtracterBRF(TArgs... args) {
        return (args - ... - 0);  // binary right fold (init == 0)
    }

    static void test_03a() {
        // binary right fold: 1 - (2 - (3 - (4 - ( 5 - 0)))) = 3
        int result = anotherSubtracterBRF(1, 2, 3, 4, 5);
        std::cout << "BRF: 1 - (2 - (3 - (4 - ( 5 - 0)))): " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterBLF(TArgs... args) {
        return (0 - ... - args);  // binary left fold (init == 0)
    }

    static void test_03b() {
        // binary left fold: ((((0 - 1) - 2) - 3) - 4) - 5 =  -15
        int result = anotherSubtracterBLF(1, 2, 3, 4, 5);
        std::cout << "BLF: ((((0 - 1) - 2) - 3) - 4) - 5: " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterURF(TArgs... args) {
        return (args - ...);  // unary right fold
    }

    static void test_03c() {
        // unary right fold: 1 - (2 - (3 - (4 - 5))) = 3
        int result = anotherSubtracterURF(1, 2, 3, 4, 5);
        std::cout << "URF: 1 - (2 - (3 - (4 - 5))): " << result << std::endl;
    }

    // ----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterULF(TArgs... args) {
        return (... - args);  // unary left fold
    }

    static void test_03d() {
        // unary left fold: ((((1 - 2) - 3) - 4) - 5 = -13
        int result = anotherSubtracterULF(1, 2, 3, 4, 5);
        std::cout << "URF: ((((1 - 2) - 3) - 4) - 5: " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    // Folding over a comma: ',' operator
    // (left or right folding is the same in this case)

    template <typename... TArgs>
    static void printerWithSeperatorRight(TArgs... args) {
        std::string sep = " ";
        ((std::cout << args << sep), ...) << std::endl;
    }

    template <typename... TArgs>
    static void printerWithSeperatorLeft(TArgs... args) {
        std::string sep = " ";
        (..., (std::cout << args << sep)) << std::endl;
    }

    // demonstrating fold expressions
    static void test_04() {
        printerWithSeperatorRight(1, 2, 3, "ABC", "DEF", "GHI");
        printerWithSeperatorLeft(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // -----------------------------------------------------------------------

    // Folding over a comma - pack arguments can be handled by a separate function

    template <typename T>
    static std::ostream& handleArg(T arg) {
        std::cout << arg << "-";
        return std::cout;
    }

    template <typename... TArgs>
    static void printerWithSeperator(TArgs... args) {
        (handleArg(args), ...) << std::endl;
    }

    // demonstrating fold expressions
    static void test_05() {
        printerWithSeperator(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // -----------------------------------------------------------------------

    // Performance Comparison
    static auto addFolding(auto ... values) {

        volatile auto result = (... + values);

        return result;
    }

    static auto addIterating(auto ... values) {

        auto list = { values ... };

        //using ElemType = decltype (list)::value_type;
        //ElemType sum{};

        volatile auto sum{ 0 };
        for (auto elem : list) {
            sum += elem;
        }

        return sum;
    }

#ifdef _DEBUG
    constexpr size_t MaxIterations = 10'000'000;     // debug
#else
    constexpr size_t MaxIterations = 100'000'000;    // release
#endif

    static void test_06_benchmark_folding() {

        ScopedTimer watch{ };

        for (size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addFolding(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
        }
    }

    static void test_06_benchmark_iterating() {

        ScopedTimer watch{ };

        for (size_t i{}; i != MaxIterations; ++i) {
            auto sum{ addIterating(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
        }
    }

    static size_t test_06_benchmark_folding_02() {

        ScopedTimer watch{ };

        size_t total{};

        for (size_t i{}; i != MaxIterations; ++i) {
            volatile auto sum{ addFolding(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
            total += sum;
        }

        return total;
    }

    static size_t test_06_benchmark_iterating_02() {

        ScopedTimer watch{ };

        size_t total{};

        for (size_t i{}; i != MaxIterations; ++i) {
            volatile auto sum{ addIterating(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
            total += sum;
        }

        return total;
    }
}

void main_folding()
{
    //Folding_Seminar::test_folding();
    //return;


    using namespace Folding;
    //test_01();
    //test_02();
    //test_03a();
    //test_03b();
    //test_03c();
    //test_03d();
    //test_04();
    //test_05();

    // Benchmarks: need to switch to nano seconds! 
    // Optimizer in Release mode is very aggressive
    test_06_benchmark_folding();
    test_06_benchmark_iterating();
    size_t result1{ test_06_benchmark_folding_02() };
    size_t result2{ test_06_benchmark_iterating_02() };
    std::println("Result1: {}", result1);
    std::println("Result2: {}", result2);
}

// =====================================================================================
// End-of-File
// =====================================================================================
