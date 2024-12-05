#include"course_html.h"

CourseHtml::CourseHtml(): cookie_sses("") ,cookie_token("") , cookie_username("") , cookie_visitor("") , curl(NULL) ,res(CURLE_OK) , courselist(NULL), header_body(NULL) ,header(NULL) , updated_cookie("") {
   curl_global_init(CURL_GLOBAL_DEFAULT) ;
   curl = curl_easy_init() ;
   
    

}

CourseHtml::~CourseHtml(){
    if(curl){
        curl_easy_cleanup(curl);
    }
    if(header){
        curl_slist_free_all(header);
    }
    if(header_body){
        fclose(header_body);
    }
    if(courselist){
        fclose(courselist);
    }
    //if(sses){
        //fclose(sses);
    //}
}

size_t CourseHtml::header_callback(char *buffer,size_t size,size_t nitems,void *userdata){
    size_t data_written = size*nitems;

    FILE *body = static_cast<FILE*>(userdata) ;
    if(body){
        fwrite(buffer,size,nitems,body);
    }
    return data_written ; 
};

size_t CourseHtml::write_callback(char *ptr, size_t size, size_t nmemb, void *userdata){
    size_t total_size = size*nmemb ; 
    FILE *body = static_cast<FILE*>(userdata) ; 
    if(body){
        fwrite(ptr,size,nmemb,body);
    }
    return total_size ;
};

void CourseHtml::setheader(){
    header= curl_slist_append(header , updated_cookie.c_str()) ;
    header= curl_slist_append(header , "Cache-Control: max-age=0") ;
    header= curl_slist_append(header , "Accept-Language: en-US,en;q=0.9") ;
    header= curl_slist_append(header , "Upgrade-Insecure-Requests: 1") ;
    header= curl_slist_append(header , "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.6778.86 Safari/537.36") ;
    header= curl_slist_append(header , "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7") ;
    header= curl_slist_append(header , "Sec-Fetch-Site: same-origin") ;
    header = curl_slist_append(header , "Sec-Fetch-Mode: navigate") ;
    header = curl_slist_append(header , "Sec-Fetch-User: ?1") ;
    header = curl_slist_append(header , "Sec-Fetch-Dest: document") ;
    header = curl_slist_append(header , "Sec-Ch-Ua: \"Chromium\";v=\"131\", \"Not_A Brand\";v=\"24\"") ;
    header = curl_slist_append(header , "Sec-Ch-Ua-Mobile: ?0") ;
    header = curl_slist_append(header , "Sec-Ch-Ua-Platform: \"Windows\"") ;
    header = curl_slist_append(header , "Referer: https://hello.iitk.ac.in/user/60019") ;
    header = curl_slist_append(header , "Accept-Encoding: gzip, deflate, br") ;
    header = curl_slist_append(header , "Priority: u=0, i") ;
    header = curl_slist_append(header, "Connection: keep-alive");
}

void CourseHtml::extract_token(){
    ifstream sses("headers.txt") ;
    if(!sses){
        cerr <<" Failed to open file" << endl;
    }
    string cook, line ; 
    while(getline(sses , line)){
        cook +=line ;  
        //cout << line << '\n';
    }
   // cout << cook<< 5 << '\n';
    
    size_t pos = cook.find("SSESS");
    size_t endpos = cook.find(";" , pos) ;
    cookie_sses = cook.substr(pos , endpos - pos) ; 
    //cout << cookie_sses << '\n'; ; 
    
    pos = cook.find("Drupal.visitor") ;
    endpos = cook.find(";", pos) ; 
    cookie_visitor = cook.substr(pos,endpos - pos) ;

    pos = cook.find("token");
    endpos = cook.find(";", pos) ;
    cookie_token = cook.substr(pos,endpos - pos) ;

    pos = cook.find("username=");
    endpos = cook.find(";" , pos) ;
    cookie_username = cook.substr(pos , endpos - pos) ;

    updated_cookie = "Cookie: uid=60019; role=student;" + cookie_username + "; _ga=GA1.1.975615793.1728786340; _ga_ZLD0V1ECY0=GS1.1.1732970497.2.1.1732970936.0.0.0;" + cookie_visitor +";"+ cookie_sses+";" + cookie_token ;//Drupal.visitor.autologout_login=1733056126; SSESS0eb1fcc85b8e9f239577c36111e88baf=0cFWu2xZPrjTXO6noEpblWzaJxpTvjjEQl%2Csit794RJcfyqf; token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJodHRwczovL2hlbGxvLmlpdGsuYWMuaW4iLCJpYXQiOjE3MzMwNTYxMjYsImV4cCI6MjA0ODQxNjEyNiwic3ViIjoiYmY0Nzk5YmEwNjcwNjZlZjU3N2RkMzRlZTljY2E2MGEyMDcwNDQzZSJ9.djyg6XoeeNQunoFH4Jqvu35neMkuNOGjZUWtej0CfW4" ; 
    ofstream cookielist("cookie.txt") ;
    cookielist << updated_cookie ;

    return;
}

void CourseHtml::perform_request(){
    //url= "https://hello.iitk.ac.in/courses" ;
    setheader();
    if(curl){
    curl_easy_setopt(curl , CURLOPT_HTTPHEADER, header);
    curl_easy_setopt(curl , CURLOPT_URL , url.c_str()) ;
    curl_easy_setopt(curl , CURLOPT_HEADERFUNCTION , header_callback) ;
    curl_easy_setopt(curl, CURLOPT_HEADERDATA ,header_body );
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip, deflate, br");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION , write_callback ) ;
    curl_easy_setopt(curl , CURLOPT_WRITEDATA, courselist) ;


    res = curl_easy_perform(curl) ; 
    if(res!=CURLE_OK){
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
     }
    }
}