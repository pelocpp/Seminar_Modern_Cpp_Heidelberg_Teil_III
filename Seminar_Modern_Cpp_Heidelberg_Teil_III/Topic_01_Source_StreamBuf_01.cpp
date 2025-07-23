// ===========================================================================
// Program.cpp
// ===========================================================================

#include <algorithm>
#include <iostream>
#include <print>
#include <sstream>
#include <streambuf>
#include <string>

namespace StreamBuffer01
{
    // https://siware.dev/007-cpp-custom-streambuf/

    // pbase() - pointer to the beginning of output sequence
    // pptr()  - pointer to current position of output sequence
    // epptr() - pointer to end of output sequence
    // pbump() - increase put pointer
    // setp()  - set output sequence pointers

    class FixedStreamBuf : public std::streambuf
    {
    private:
        std::string& m_buffer;

    public:
        FixedStreamBuf(std::string& buffer, size_t bufferMaxSize)
            : m_buffer{ buffer }
        {
            buffer.resize(bufferMaxSize);

            // repositions the beginning, next, and end pointers of the output sequence
            setp(buffer.data(), buffer.data() + buffer.size());
        }

        ~FixedStreamBuf()
        {
            const size_t writtenSize = pptr() - pbase();
            m_buffer.resize(writtenSize);
        }

    protected:
        // writes one character to the output sequence.
        int_type sputc(char_type ch)
        {
            std::streambuf::sputc(ch);
        }

        // writes multiple characters to the output sequence
        std::streamsize xsputn(const char* s, std::streamsize n) override
        {
            auto putPointer = pptr();    // pointer to the current character (put pointer) in the put area

            std::memcpy(putPointer, s, n);

            pbump(static_cast<int>(n));  // advances the next pointer of the output sequence

            // just for testing
            auto secondPutPointer = pptr();
            auto diff = secondPutPointer - putPointer;

            return n;
        };

        int_type overflow(int_type ch) override
        {
            // Fixed stream buffer doesn't handle overflow because we don't want to
            throw std::runtime_error("FixedStreamBuf overflow!");
        }

        pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which) override
        {
            // only output stream is supported
            if (which != std::ios_base::out)
                throw std::runtime_error("Not supported");

            if (dir == std::ios_base::cur)
                pbump((int) off);
            else if (dir == std::ios_base::end)
                setp(pbase(), epptr() + off, epptr());
            else if (dir == std::ios_base::beg)
                setp(pbase(), pbase() + off, epptr());
            return pptr() - pbase();
        }
    };
}

static void doSomething(const std::string& s)
{
    std::println("String: Size: {} - Capacity: {}", s.size(), s.capacity());
    std::println("Data:  {}", s);
}

static void main_streambuf01_a ()
{
    using namespace StreamBuffer01;

    std::string data;

    // write to 'data' via std::ostream
    {
        FixedStreamBuf streamBuf{ data, 1024 };

        std::ostream os{ &streamBuf };

        // os << "Hello World!";
        os << "Hello";
        os << " World!";
    }

    // do something with streamData
    doSomething(data);
}

static void main_streambuf01_b()
{
    using namespace StreamBuffer01;

    {
        std::string data;

        {
            FixedStreamBuf streamBuf{ data, 1024 };
            std::ostream os{ &streamBuf };
            os << "hello world";

            os.seekp(7);
            os << 'W';

            os.seekp(0, std::ios_base::end);
            os << '!';

            os.seekp(0);
            os << 'H';

           // std::cout << os.() << '\n';
            doSomething(data);

        }

        // do something with streamData
        doSomething(data);
    }
}

static void main_streambuf01_c()
{
    using namespace StreamBuffer01;

    std::string data;

    FixedStreamBuf streamBuf{ data, 1024 };
    std::ostream os{ &streamBuf };
    os << "hello world";

    // https://en.cppreference.com/w/cpp/io/basic_ostream/seekp
    /*auto pos = */streamBuf.pubseekoff(6, std::ios_base::beg, std::ios_base::out);
    // pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which) override
    // streamBuf.seekoff(7, std::ios_base::cur, std::ios_base::out);
    os << 'W';

    streamBuf.pubseekoff(0, std::ios_base::beg, std::ios_base::out);
    os << 'H';

    //os.seekp(0, std::ios_base::end);
    //os << '!';

    // overflow
    streamBuf.pubseekoff(0, std::ios_base::end, std::ios_base::out);
    os << '!';

    // std::cout << os.() << '\n';
    doSomething(data);

}

void main_streambuf01()
{
    main_streambuf01_a();
    main_streambuf01_b();
    main_streambuf01_c();
}

// ===========================================================================
// End-of-File
// ===========================================================================
