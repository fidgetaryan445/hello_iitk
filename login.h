#ifndef LOGIN_REQUEST
#define LOGIN_REQUEST

#include<iostream>
#include<string>
#include <curl/curl.h>
#include <sstream>

using namespace std  ;

class LoginRequest{
    private:
    CURL *curl ;
    CURLcode res ;
    FILE *header_body ;
    FILE *response ;
    struct curl_slist *headers ;     
    const string url ; 
    static size_t header_callback(char *buffer,size_t size,size_t nitems,void *userdata);
    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
    void setheader();

    public:
    string username ;
    string password ;
    void perform_login();
    LoginRequest();
    ~LoginRequest();

};

#endif //LOGIN_REQUEST