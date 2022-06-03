
/////////////
// files.h //
/////////////

#ifndef FILES_H
#define FILES_H

#include <string>
#include <fstream>
#include <memory>

// Class `InputFile` represents an input stream that is either a file or the `cin` stream.

class InputFile
{
    public:

        static constexpr const char * stdin_name  = "STDIN";  // Special name for input files, indicating stdin (cin) should be used.

        InputFile(const std::string & filename);

        std::istream & get_istream_reference() const;

    private: // Instance member variables.

        std::unique_ptr<std::ifstream> input_file;
};

// Class `OutputFile` represents an output stream that is either a file or the `cout` stream.

class OutputFile
{
    public:

        static constexpr const char * stdout_name = "STDOUT"; // Special name for output files, indicating stdout (cout) should be used.

        OutputFile(const std::string & filename);

        std::ostream & get_ostream_reference() const;

    private: // Instance member variables.

        std::unique_ptr<std::ofstream> output_file;
};

#endif // FILES_H
