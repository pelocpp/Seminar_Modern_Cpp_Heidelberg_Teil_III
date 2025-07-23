// ===========================================================================
// Program.cpp
// ===========================================================================

#include <algorithm>
#include <iostream>
#include <print>
#include <sstream>
#include <streambuf>
#include <string>

// https://www.youtube.com/watch?v=JRl3pWl-F5g

// #define Verbose

namespace StreamBuffer02
{
    // pbase() - pointer to the beginning of output sequence
    // pptr()  - pointer to current position of output sequence
    // epptr() - pointer to end of output sequence
    // pbump() - increase put pointer
    // setp()  - set output sequence pointers

    class Rotate13Buffer : public std::streambuf
    {
    private:
        static const int BufSize = 3;

        std::streambuf* m_buf;
        char m_buffer[BufSize];

    public:
        Rotate13Buffer(std::streambuf* buf) : m_buf{ buf }
        {
            this->setp(m_buffer, m_buffer + sizeof(m_buffer) - 1);
        }

        std::streambuf* buffer() const
        {
            return m_buf;
        }

        void doOutput()
        {
#if defined(Verbose)
            std::println("### doOutput");
#endif

            auto dummy1 = pbase();
            auto dummy2 = pptr();

            std::transform(
                pbase(),
                pptr(),
                pbase(),
                [](char elem) -> char {
                    if (elem >= 'a' && elem <= 'z') {
                        return (elem + 13 <= 'z') ? elem + 13 : elem + 13 - 26;
                    }
                    else if (elem >= 'A' && elem <= 'Z') {
                        return (elem + 13 <= 'Z') ? elem + 13 : elem + 13 - 26;
                    }
                    else {
                        return elem;
                    }
                }
            );

            // writes multiple characters to the output sequence
            m_buf->sputn(pbase(), pptr() - pbase());

            // repositions the beginning, next, and end pointers of the output sequence
            this->setp(m_buffer, m_buffer + sizeof(m_buffer) - 1);
        }

        int_type overflow(int_type ch) override
        {
#if defined(Verbose)
            std::println("### overflow");
#endif

            char dummy = ch;

            if (ch != traits_type::eof()) {

                *pptr() = ch;
                pbump(1);
                doOutput();
            }

            return ch;
        }

        int sync() override
        {
#if defined(Verbose)
            std::println("### sync");
#endif

            doOutput();
            return 0;
        }
    };
}

static void main_streambuf02_a()
{
    using namespace StreamBuffer02;

    Rotate13Buffer rotateBuffer{ std::cout.rdbuf() };

    std::ostream os(&rotateBuffer);

    os << std::cin.rdbuf();
    os.flush();
}

static void main_streambuf02_b()
{
    using namespace StreamBuffer02;

    // use stream buffer of std::cout object, connected with the console
    Rotate13Buffer rotateBuffer{ std::cout.rdbuf() };

    // this std::ostream object should use the Rotate13Buffer functionality
    std::ostream os(&rotateBuffer);

    std::stringstream ss;
    ss << "abcdefghijklmn";

    // this statement does the work:
    // the std::stringstream's stream buffer is streamed into the output stream,
    // being written to the console,
    // because the underlying buffer of 'rotateBuffer' is connected with std::cout
    os << ss.rdbuf();

    os.flush();
}

static void main_streambuf02_c()
{
    // does not work

    using namespace StreamBuffer02;

    std::stringstream ss;
    ss << "abcdef";

    Rotate13Buffer rotateBuffer{ ss.rdbuf() };
    std::cout.set_rdbuf(rotateBuffer.buffer());

    rotateBuffer.sync();
    std::cout.flush();
}

void main_streambuf02()
{
    main_streambuf02_a();
    main_streambuf02_b();
    main_streambuf02_c();
}

// ===========================================================================
// End-of-File
// ===========================================================================
