#include "file.hpp"

size_t File::WriteDate(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int File::FileDownload(const char* url,const char* file_path){
    CURL* curl;
    FILE* fp;
    CURLcode res;
    curl = curl_easy_init();
    
    if(curl){
        fp = fopen(file_path, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    
    return 1;
}


int ZipFile::Unzip(const char *zip_path, const char *unzip_path){
    const std::string tmp_path = unzip_path;
    unz_global_info global_info;
    
    unzFile zipfile = unzOpen(zip_path);
    
    if(zipfile == NULL){
        return -1;
    }
    
    if(unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK){// could not read file global info
        unzClose(zipfile);
        return -1;
    }
    
    if(mkdir(&tmp_path[0], 0755) == -1){ // error
        unzClose(zipfile);
        return -1;
    }
    
    for(unsigned int i=0;i<global_info.number_entry;i++){
        unz_file_info file_info;
        char file_name[512];
        
        if(unzGetCurrentFileInfo(zipfile, &file_info, &file_name[0], 512, NULL, 0, NULL, 0) != UNZ_OK){ // could not read file
            unzClose(zipfile);
            return -1;
        }
        
        const size_t file_name_length  = strlen(file_name);
        const std::string file = tmp_path+file_name;
        
        file_structure.push_back(file_name);
        
        if(file_name[file_name_length - 1] == '/'){
            mkdir(&file[0],0755);
        }else{
            if(unzOpenCurrentFile(zipfile) != UNZ_OK){// could not read file
                unzClose(zipfile);
                return -1;
            }
            
            FILE *out = fopen(&file[0], "wb");
            
            if(out == NULL){// could not open destination file
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return -1;
            }
            
            int error = UNZ_OK;
            
            do{
                char read_buffer[8192];
                
                error = unzReadCurrentFile(zipfile,read_buffer,8192);
                if ( error < 0 ){// error
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    fclose(out);
                    return -1;
                }
                
                if (error > 0){ // Write data to file.
                    fwrite(read_buffer,error,1,out); // You should check return of fwrite...
                }
            } while (error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Go the the next entry listed in the zip file.
        if ((i+1) < global_info.number_entry){
            if ( unzGoToNextFile( zipfile ) != UNZ_OK ){// cound not read next file
                unzClose( zipfile );
                return -1;
            }
        }
        
    }
    
    unzClose( zipfile );
    
    return 0;
}

int ZipFile::FileCopy(const char* to, const char* from,const std::string links,const uint32_t* notification,const size_t notification_size){
    const size_t file_count = file_structure.size();
    int source,dest;
    
    if(access(from, 00) == -1){
        mkdir(from, 0755);
    }

    for(int i=0;i<file_count;i++){
        char buf[BUFSIZ];
        size_t size;
        std::string orginal_file = to + file_structure[i];
        std::string copy_file = from + file_structure[i];
        source = 0;
        dest = 0;
        
        const size_t file_name_length  = copy_file.length();
        
        if((&copy_file[0])[file_name_length-1] == '/'){
            mkdir(&copy_file[0],0755);
            continue;
        }
        
        source = open(&orginal_file[0], O_RDONLY);
        if (source < 0){
            close(source);
            
            return -1;
        }
        
        
        dest = open(&copy_file[0], O_WRONLY | O_CREAT | O_EXCL, 0666);
        if (dest < 0){
            close(source);
            close(dest);
            
            return -1;
        }
        
        if(file_structure[i].find("api-links.json") != std::string::npos){
            write(dest,&links[0],links.length());
        }else if(file_structure[i].find("notification.config") != std::string::npos){
            write(dest,notification,notification_size);
        }else {
            while ((size = read(source, buf, BUFSIZ)) > 0) {
                write(dest, buf, size);
            }
        }
        
        close(source);
        close(dest);
    }
    
    
    return 1;
}

