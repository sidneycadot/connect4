
//////////////
// files.cc //
//////////////

#include <iostream>

#include "files.h"

using namespace std;

InputFile::InputFile(const string & filename) :
    input_file(filename == stdin_name ? nullptr : make_unique<ifstream>(ifstream(filename)))
{
    // Empty body.
}

istream & InputFile::get_istream_reference() const
{
    return input_file ? *input_file : cin;
}

OutputFile::OutputFile(const string & filename) :
    output_file(filename == stdout_name ? nullptr : make_unique<ofstream>(ofstream(filename)))
{
    // Empty body.
}

ostream & OutputFile::get_ostream_reference() const
{
    return output_file ? *output_file : cout;
}
