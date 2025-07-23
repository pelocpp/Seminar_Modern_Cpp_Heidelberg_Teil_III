// ===========================================================================
// Program.cpp
// ===========================================================================

#include "ScopedTimer.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <print>
#include <string>
#include <unordered_map>
#include <span>

//#define FileName "trunc.txt"
#define FileName "weather_stations.csv"

// #define Verbose

namespace Simon_Toth_Performance_01
{
    struct Record {
        uint64_t cnt;
        double sum;

        float min;
        float max;
    };

    using DB = std::unordered_map<std::string, Record>;

    static DB process_input(std::istream& in) {
        DB db;

        std::string station;
        std::string value;

        // Grab the station and the measured value from the input
        while (std::getline(in, station, ';') && std::getline(in, value, '\n')) {

            // Convert the measured value into a floating point
            float fp_value = std::stof(value);

            // Lookup the station in our database
            auto it = db.find(station);
            if (it == db.end()) {
                // If it's not there, insert
                db.emplace(station,
                    Record{ 1, fp_value, fp_value, fp_value });
                continue;
            }
            // Otherwise update the information
            it->second.min = std::min(it->second.min, fp_value);
            it->second.max = std::max(it->second.max, fp_value);
            it->second.sum += fp_value;
            ++it->second.cnt;
        }

        return db;
    }

    static void format_output(std::ostream& out, const DB& db)
    {
        std::vector<std::string> names(db.size());
    
        // Grab all the unique station names
        // std::ranges::copy(db | std::views::keys, names.begin());
        
        std::transform(
            db.begin(),
            db.end(),
            names.begin(),
            [](const auto& elem) { return elem.first; }
        );

        
        // Sorting UTF-8 strings lexicographically is the same
        // as sorting by codepoint value
        std::ranges::sort(names);

        std::string delim = "";


#if defined (Verbose)
        out << std::setiosflags(out.fixed | out.showpoint)
            << std::setprecision(1);

        out << "{";
        for (auto& k : names) {
            // Print StationName:min/avg/max
            auto& [_, record] = *db.find(k);
            out << std::exchange(delim, ", ") << k << "="
                << record.min << "/" << (record.sum / record.cnt)
                << "/" << record.max << '\n';
        }
        out << "}\n";
#endif

    }
}

// =====================================================================

namespace Simon_Toth_Performance_02
{
    // https://www.geeksforgeeks.org/cpp/what-is-the-efficient-way-of-reading-a-huge-text-file/

    struct Record {
        uint64_t cnt;
        double sum;

        float min;
        float max;
    };

    using DB = std::unordered_map<std::string, Record>;

    static DB process_input(std::istream& in)
    {
        DB db;

        std::string station;
        std::string value;

        // Declare the buffer size
        const int BUFFER_SIZE = 4096;

        std::vector<char> buffer(BUFFER_SIZE);
        std::istringstream iss;

        // Parse the chunk of data from the text file into lines
        while (in.read(buffer.data(), BUFFER_SIZE)) {

            std::streamsize bytes_read = in.gcount();

            iss.str(std::string(buffer.data(), bytes_read));
            iss.clear();

            std::string line;
            //while (std::getline(iss, line)) {
            //    if (!line.empty()) {
            //        std::cout << line << std::endl;
            //    }
            //}

            while (std::getline(in, station, ';') && std::getline(in, value, '\n')) {

                // Convert the measured value into a floating point
                float fp_value = std::stof(value);

                // Lookup the station in our database
                auto it = db.find(station);
                if (it == db.end()) {
                    // If it's not there, insert
                    db.emplace(station,
                        Record{ 1, fp_value, fp_value, fp_value });
                    continue;
                }
                // Otherwise update the information
                it->second.min = std::min(it->second.min, fp_value);
                it->second.max = std::max(it->second.max, fp_value);
                it->second.sum += fp_value;
                ++it->second.cnt;
            }
        }

        return db;
    }

    static void format_output(std::ostream& out, const DB& db)
    {
        std::vector<std::string> names(db.size());

        // Grab all the unique station names
        // std::ranges::copy(db | std::views::keys, names.begin());

        std::transform(
            db.begin(),
            db.end(),
            names.begin(),
            [](const auto& elem) { return elem.first; }
        );


        // Sorting UTF-8 strings lexicographically is the same
        // as sorting by codepoint value
        std::ranges::sort(names);

        std::string delim = "";


#if defined (Verbose)
        out << std::setiosflags(out.fixed | out.showpoint)
            << std::setprecision(1);

        out << "{";
        for (auto& k : names) {
            // Print StationName:min/avg/max
            auto& [_, record] = *db.find(k);
            out << std::exchange(delim, ", ") << k << "="
                << record.min << "/" << (record.sum / record.cnt)
                << "/" << record.max << '\n';
        }
        out << "}\n";
#endif

    }
}

// ===========================================================================

void test_simon_toth_performance_01()
{
    using namespace Simon_Toth_Performance_01;

    std::ifstream ifile(FileName, std::ios_base::in);

    // analyze the stream buffer
    const auto& streamBuffer = ifile.rdbuf();
    //streamBuffer

    if (not ifile.is_open())
        throw std::runtime_error("Failed to open the input file.");

    ScopedTimer watch;

    auto db = process_input(ifile);

    format_output(std::cout, db);

    ifile.close();
}


void test_simon_toth_performance_02()
{
    using namespace Simon_Toth_Performance_02;

    std::ifstream ifile(FileName, std::ios_base::in);

    // analyze the stream buffer
    const auto& streamBuffer = ifile.rdbuf();
    //streamBuffer

    if (not ifile.is_open())
        throw std::runtime_error("Failed to open the input file.");

    ScopedTimer watch;

    auto db = process_input(ifile);

    format_output(std::cout, db);

    ifile.close();
}

// ===========================================================================
// End-of-File
// ===========================================================================
