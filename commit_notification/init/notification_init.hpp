#ifndef notification_init_hpp
#define notification_init_hpp

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include "../IO/file.hpp"

class NotificationInit{
private:
    std::vector<std::string> hooks;
public:
    int FindGitHooks();
    int ProjectInit(const char* unzip_file,ZipFile& zip);
};

#endif /* notification_init_hpp */
