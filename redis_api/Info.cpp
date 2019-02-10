//
// Created by morgan on 10.02.2019.
//

#include "Info.h"
#include <sstream>

InfoSection::InfoSection(std::string name, std::string data)
:name_(name), vals_()
{
    std::stringstream reader(data);
    std::string line;

    while (!reader.eof())
    {
        reader >> line;
        auto sep = line.find(':');
        if(sep == std::string::npos)
            break;
        auto key = line.substr(0, sep);
        auto value = line.substr(sep + 1);
        vals_[key] = value;
    }
}

Info::Info(Decoder& data)
: sections_()
{
    int sz;
    auto str_raw = data.read_bulk_string(sz);
    auto str = std::string(str_raw.get(), sz);

    size_t pos = 0;
    while (true)
    {
        pos = str.find('#', pos);
        if(pos == std::string::npos)
            break;
        pos += 2;
        auto sect_end = str.find('\r', pos);
        auto sect_name = str.substr(pos, sect_end - pos);
        std::string sect_data;
        sect_end += 2;
        pos = str.find('#', sect_end);
        if(pos == std::string::npos)
        {
            sect_data = str.substr(sect_end);
        }
        else
        {
            sect_data = str.substr(sect_end, pos - sect_end);
        }
        auto sect = InfoSection(sect_name, sect_data);
        sections_[sect_name] = sect;
    }
}