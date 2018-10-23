#ifndef file_hpp
#define file_hpp

#include <iostream>
#include <fcntl.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include "../minizip-1.2/unzip.h"

class File{
public:
    File(){}
    
    size_t WriteDate(void *ptr, size_t size, size_t nmemb, FILE *stream);
    int FileDownload(const char* url,const char* file_path);
    
    int FileCopy(const char* to, const char* from);
};

class ZipFile:public File{
private:
    std::vector<std::string> file_structure;
public:
    ZipFile():File(){}
    
    int Unzip(const char* zip_path,const char* unzip_path);
    int FileCopy(const char* to, const char* from,const std::string links,const uint32_t* notification,const size_t notification_size);
};




#endif /* file_hpp */
