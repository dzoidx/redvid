//
// Created by morgan on 10.02.2019.
//

#ifndef REDVID_INFO_H
#define REDVID_INFO_H

#include <string>
#include <vector>
#include <utility>
#include <map>
#include "../RESP/Decoder.h"

class InfoSection
{
public:
    InfoSection() {}
    InfoSection(const InfoSection& info)
    {
        name_ = info.name_;
        vals_ = info.vals_;
    }
    InfoSection(std::string name, std::string data);
    std::string get(std::string key) { return vals_[key]; }
private:
    std::string name_;
    std::map<std::string, std::string> vals_;
};

class Info
{
public:
    Info(const Info& info) {
        sections_ = info.sections_;
    }
    Info(Decoder& data);
    InfoSection get(const std::string& section) { return sections_[section];}

private:
    std::map<std::string, InfoSection> sections_;
};


#endif //REDVID_INFO_H
