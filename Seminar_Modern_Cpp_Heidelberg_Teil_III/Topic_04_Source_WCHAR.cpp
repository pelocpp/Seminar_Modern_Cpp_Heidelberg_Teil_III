// ===========================================================================
// Program.cpp
// ===========================================================================

#include <iostream>
#include <string>
#include <print>

void main_wchar_01()
{
    std::wcout << L"I am a wide-character string literal." << std::endl;

    wchar_t myWideCharacter{ L'm' };
    std::wcout << std::format(L"myWideCharacter is {}", myWideCharacter) << std::endl;
}

void main_wchar_02()
{
    const char8_t* s1{ u8R"(Raw UTF-8 string literal)" };
    const wchar_t* s2{ LR"(Raw wide string literal)" };
    const char16_t* s3{ uR"(Raw UTF-16 string literal)" };
    const char32_t* s4{ UR"(Raw UTF-32 string literal)" };

    std::wcout << s2 << std::endl;
    // Can't stream s1, s3, and s4, because streams don't support char8_t, char16_t, and char32_t.

    const char8_t* formula1{ u8"\x3C0 r\xB2" };
    const char8_t* formula2{ u8"\u03C0 r\u00B2" };
  //  const char8_t* formula3{ u8"\N{GREEK SMALL LETTER PI} r\N{SUPERSCRIPT TWO}" };

    // C++23
    auto hello{ u8"こんにちは世界" };
   // std::println("{}", hello);
}

void main_wchar()
{
    //main_wchar_01();
    main_wchar_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================
