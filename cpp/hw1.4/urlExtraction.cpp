//Created by Mingmin Song on 1/28/18.

#include "urlExtraction.h"

std::set<std::string> extract_hyperlinks( const std::string& text, std::string& url)
{
    static const std::regex hl_regex( "<a href=\"(.*?)\".*>", std::regex_constants::icase  ) ;
    std::set<std::string>  res((std::sregex_token_iterator( text.begin(), text.end(), hl_regex, 1 )),
             std::sregex_token_iterator() ) ;

    std::set<std::string> todelete;
    for (std::set<std::string>::iterator it = res.begin(); it != res.end(); it++) {
    	if (it->find("http") == std::string::npos) {
   			todelete.insert(*it);
    	}
    }
    for (std::set<std::string>::iterator it = todelete.begin(); it != todelete.end(); it++) {
    	res.erase(*it);
    	res.insert(url+"/"+(*it));
    }
    return res;
}