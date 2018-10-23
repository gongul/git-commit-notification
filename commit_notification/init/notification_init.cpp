#include "notification_init.hpp"


int NotificationInit::FindGitHooks(){
    FILE* fp;
    char buff[1024];
    
    fp = popen("find ~/ -name \"hooks\" -type d | grep .git", "r");
    sleep(10);
    
    if(fp == NULL){
        std::cout << "null" << std::endl;
        return -1;
    }
    
    while( fgets( buff, 1024, fp) ){
        std::string str = "";
        int i=0;
        while(buff[i] && buff[i] != '\n'){
            str += buff[i];
            i++;
        }
        str.append("/test3/");
//        hooks.push_back(str);
    }
    
    hooks.push_back("/tmp/notification-test/");
    
    pclose(fp);
    
    return 1;
}

int NotificationInit::ProjectInit(const char* unzip_file,ZipFile& zip){
    const char* link_names[] = {"jandi","discord"};
    const size_t link_count = sizeof(link_names)/sizeof(char*);
    
    
    const size_t project_count = hooks.size();
    
    for(int i=0;i<project_count;i++){
        int loop_count = 0;
        std::string link;
        std::cout << "경로 : " << hooks[i] << std::endl;
        
        while (loop_count < link_count) {  // webhook의 개수
            char buf[200];
            std::string api_links;
            
            std::cout << "프로젝트의 알림을 보낼 [" << link_names[loop_count] << "] webhook api 주소를 적어주세요 : ";
            //            cin.getline(buf, 200);
            buf[0] = 'a';
            
            if(std::cin.fail()){
                //                BufClear();
                
                std::cout << "URL 길이가 너무 깁니다." << std::endl;
                continue;
            }
            
            api_links = buf;
            link += link_names[loop_count];
            link += ":" + api_links + "\n";
            
            loop_count++;
        }
        
        bool end_point = true;
        char flag;
        
        while(end_point){
            flag = ' ';
            
            std::cout << "프로젝트 커밋 알림을 활성화 하시겠습니까? (y/n) : ";
            //            flag = cin.get();
            flag = 'y';
            
            if(!(flag == 'y' || flag =='Y' || flag == 'n' || flag == 'N')){
                std::cout << "정상적이지 않은 값입니다." << std::endl;
                //                BufClear();
                continue;
            }
            
            end_point = false;
        }
        
        uint32_t* notification;
        size_t notification_size;
        
        
        //        if(flag == 'y' || flag == 'Y'){
        //            uint32_t notification_true[] = {0x6c696164,0x6f6e2d79,0x69666974,0x69746163,0x743d6e6f,0xa657572,0x69746f6e,0x61636966,0x6e6f6974,0x7572743d,0x65,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x61000e};
        //
        //            notification_size = sizeof(notification_true) / sizeof(uint32_t);
        //            notification = notification_true;
        //        }else{
        uint32_t notification_false[] = {0x6c696164,0x6f6e2d79,0x69666974,0x69746163,0x663d6e6f,0x65736c61,0x746f6e0a,0x63696669,0x6f697461,0x61663d6e,0x65736c,0x0,0x1000400,0x1,0x1000470,0x1,0x4c8,0x0,0x100047c,0x1,0x4bc,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
        
        notification_size = sizeof(notification_false) / sizeof(uint32_t);
        notification = notification_false;
        //        }
        
        
        zip.FileCopy(unzip_file, &(hooks[i])[0], link, notification, notification_size);
        //        FileCopy(file_structure, "/tmp/git_push_notification/",&(hooks[i])[0],link,notification,notification_size);
        //        BufClear();
        
    }
    
    return 1;
}
