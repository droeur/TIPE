#pragma once
#include <iostream>
#include <string>
#include <boost/program_options.hpp>

class options_class
{
public:
    options_class(const int argc, char* argv[])
    {
        desc_ = new boost::program_options::options_description{"Options"};
        desc_->add_options()
            ("help,h", "Help screen")
            ("map,m", boost::program_options::value<std::string>(), "Map location")
            ("parameter,p", boost::program_options::value<std::string>(), "Location of parameter files")
            ("no-graphic,n", "Start without graphics")
            ("graphic-folder,g", boost::program_options::value<std::string>(), "Graphic folder location")
            ("font,t", boost::program_options::value<std::string>(), "Font file")
            ("fast,f", "Start without speed limitation")
            ("log,l", boost::program_options::value<int>() ,"Log level")
            ("output,o", boost::program_options::value<std::string>(), "Output file");
        vm_ = new boost::program_options::variables_map;
        store(parse_command_line(argc, argv, *desc_), *vm_);
        notify(*vm_);

        if (vm_->count("help"))
            std::cout << *desc_ << '\n';
        if (vm_->count("map"))
            map_file_ = (*vm_)["map"].as<std::string>();
        if (vm_->count("parameter"))
            parameter_file_ = (*vm_)["parameter"].as<std::string>();
        if (vm_->count("output"))
            output_file_ = (*vm_)["output"].as<std::string>();
        if (vm_->count("no-graphic"))
            graphics_ = false;
        if (vm_->count("graphic-folder"))
            graphic_folder_ = (*vm_)["graphic-folder"].as<std::string>();
        if (vm_->count("font"))
            font_file_ = (*vm_)["font"].as<std::string>();
        if (vm_->count("fast"))
            fast_ = true;
        if (vm_->count("log"))
            log_level_ = (*vm_)["log"].as<int>();
    }

    ~options_class()
    {
        delete desc_;
        delete vm_;
    }

    std::string map_file_get()
    {
        return map_file_;
    }

    std::string parameter_file_get()
    {
        return parameter_file_;
    }

    std::string output_file_get()
    {
        return output_file_;
    }

    std::string graphic_folder_get()
    {
        return graphic_folder_;
    }

    std::string font_file_get()
    {
        return font_file_;
    }

    [[nodiscard]] bool graphics_get() const
    {
        return graphics_;
    }

    [[nodiscard]] bool fast_get() const
    {
        return fast_;
    }

    [[nodiscard]] int log_level_get() const
    {
        return log_level_;
    }

    [[nodiscard]] int n_test_get() const { return n_test_; }

private:
    boost::program_options::options_description* desc_;
    boost::program_options::variables_map* vm_;
    std::string map_file_;
    std::string parameter_file_;
    std::string output_file_;
    std::string graphic_folder_;
    std::string font_file_;
    bool graphics_ = true;
    bool fast_ = false;
    int log_level_ = 0;
    int n_test_ = 100;
};
