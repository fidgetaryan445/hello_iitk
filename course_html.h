#ifndef COURSE_HTML
#define COURSE_HTML

#include<iostream>
#include<string> 
#include<fstream>
#include<curl/curl.h>

using namespace std ;

class CourseHtml{
    private:
    string cookie_sses ;
    string cookie_token ;
    string cookie_username ;
    string updated_cookie ;
    string cookie_visitor ;
    CURL* curl ;
    CURLcode res ; 
    
    
    struct curl_slist *header;
    //ifstream *sses ;
    static size_t header_callback(char *buffer,size_t size,size_t nitems,void *userdata);
    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
    void setheader();

    public:
    void extract_token();
    void perform_request();
    string url ;
    FILE *courselist ;
    FILE *header_body ;
    CourseHtml();
    ~CourseHtml();


};

#endif //COURSE_HTML
