#pragma once
#include <string>
#include <boost/program_options.hpp>

class options_class final
{
    boost::program_options::options_description* desc_;
    boost::program_options::variables_map* vm_;
    std::string map_file_ = "map.txt";
    std::string parameter_file_ = "expe.txt";
    std::string output_file_ = "result.txt";
    std::string graphic_folder_ = ".";
    std::string font_file_ = "cour.ttf";
    bool graphics_ = true;
    bool fast_ = false;
    int log_level_ = 0;
    int n_test_ = 1;

public:
    options_class(int argc, char* argv[]);
    ~options_class();

    [[nodiscard]] bool check() const;

    std::string map_file_get();

    std::string parameter_file_get();

    std::string output_file_get();

    std::string graphic_folder_get();

    std::string font_file_get();

    [[nodiscard]] bool graphics_get() const;

    [[nodiscard]] bool fast_get() const;

    [[nodiscard]] int log_level_get() const;

    [[nodiscard]] int n_test_get() const;
};
