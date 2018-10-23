#include "run.hpp"
#include <cstdlib>

int main(){
//    NotificationInit init;
//    ZipFile zip;
//
//    const char* cloud_url = "https://drive.google.com/uc?id=1QEDHZlpCm7nFZCti4NAiWjOPEBCuBWZQ&export=download";
//    const char* download_file = "/tmp/git_push_notification.zip";
//    const char* unzip_file = "/tmp/git_push_notification/";
//
//    init.FindGitHooks();
//    zip.FileDownload(cloud_url, download_file);
//    zip.Unzip(download_file, unzip_file);
//    init.ProjectInit(unzip_file,zip);
//
    int code = system("bash /Users/donggun/Documents/commit_notification/commit_notification/test.sh");
//    int code = system("cat <(crontab -l) <(echo \"0 0 * * * /tmp/notification-test/daily-notification.sh\") | crontab -");
    
    std::cout << code << std::endl;
    

}
