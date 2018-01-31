#pragma once
#include <fstream>
#include <string>
#include <iterator>
#include <set>
#include <regex>

std::set<std::string> extract_hyperlinks( const std::string& text, std::string& url);