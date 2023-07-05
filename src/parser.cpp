#include <fstream>
#include<iostream>
#include <string>
#include "parser.hpp"

using namespace std;

bool file_check(const string &name)
{
    ifstream file;
    file.open(name.c_str());
    return file.good();
}

options_class::options_class(const int argc, char* argv[])
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

options_class::~options_class()
{
    delete desc_;
    delete vm_;
}

bool options_class::check() const
{
    if (!file_check(map_file_))
    {
        cerr << "Error: no map file " << map_file_ << endl;
        return false;
    }
    if (!file_check(parameter_file_))
    {
        cerr << "Error: no parameter file " << parameter_file_ << endl;
        return false;
    }
    if (!file_check(font_file_))
    {
        cerr << "Error: no font file " << font_file_ << endl;
        return false;
    }

    return true;
}

std::string options_class::map_file_get()
{
    return map_file_;
}

std::string options_class::parameter_file_get()
{
    return parameter_file_;
}

std::string options_class::output_file_get()
{
    return output_file_;
}

std::string options_class::graphic_folder_get()
{
    return graphic_folder_;
}

std::string options_class::font_file_get()
{
    return font_file_;
}

bool options_class::graphics_get() const
{
    return graphics_;
}

bool options_class::fast_get() const
{
    return fast_;
}

int options_class::log_level_get() const
{
    return log_level_;
}

int options_class::n_test_get() const
{
    return n_test_;
}
