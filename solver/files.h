
/////////////
// files.h //
/////////////

#ifndef FILES_H
#define FILES_H

#include <string>
#include <fstream>
#include <memory>
#include <iostream>

class InputFile
{
    // Class `InputFile` represents an input stream that is either a pre-existing file or the `cin` stream.

    public:

        // Special name for input files, indicating that stdin (cin) should be used.
        static constexpr const char * stdin_name  = "STDIN";

        InputFile(const std::string & filename):
            input_file(filename == stdin_name ? nullptr : std::make_unique<std::ifstream>(filename))
        {
            // Empty body.
        }

        std::istream & get_istream_reference() const
        {
            return input_file ? *input_file : std::cin;
        }

    private: // Member variables.

        std::unique_ptr<std::ifstream> input_file;
};

class OutputFile
{
    // Class `OutputFile` represents an output stream that is either a newly created file or the `cout` stream.

    public:

        // Special name for output files, indicating that stdout (cout) should be used.
        static constexpr const char * stdout_name = "STDOUT";

        OutputFile(const std::string & filename):
            output_file(filename == stdout_name ? nullptr : std::make_unique<std::ofstream>(filename))
        {
            // Empty body.
        }

        std::ostream & get_ostream_reference() const
        {
            return output_file ? *output_file : std::cout;
        }

    private: // Member variables.

        std::unique_ptr<std::ofstream> output_file;
};

#endif // FILES_H
