#include"login.h"

LoginRequest::LoginRequest(): url("https://hello.iitk.ac.in/user/login") , curl(NULL) , res(CURLE_OK) , header_body(NULL), response(NULL) , headers(NULL){
    
    curl_global_init(CURL_GLOBAL_DEFAULT) ;
    curl = curl_easy_init();
    
    header_body = fopen("headers.txt" , "w") ;
    response = fopen("response.txt" , "w" );

    if(!header_body || !response){
        cerr << "ERROR : Files couldn't be created" << endl ;
    }
}

LoginRequest::~LoginRequest(){
    if(curl){
        curl_easy_cleanup(curl);
    }
    if(headers){
        curl_slist_free_all(headers);
    }
    if(header_body){
        fclose(header_body);
    }
    if(response){
        fclose(response);
    }
}

size_t LoginRequest::header_callback(char *buffer,size_t size,size_t nitems,void *userdata){
    size_t data_written = size*nitems;

    FILE *body = static_cast<FILE*>(userdata) ;
    if(body){
        fwrite(buffer,size,nitems,body);
    }
    return data_written ; 
};

size_t LoginRequest::write_callback(char *ptr, size_t size, size_t nmemb, void *userdata){
    size_t total_size = size*nmemb ; 
    FILE *body = static_cast<FILE*>(userdata) ; 
    if(body){
        fwrite(ptr,size,nmemb,body);
    }
    return total_size ;
};

void LoginRequest::setheader(){
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    headers = curl_slist_append(headers, "Sec-Ch-Ua: \"Chromium\";v=\"131\", \"Not_A Brand\";v=\"24\"");
    headers = curl_slist_append(headers, "Sec-Ch-Ua-Mobile: ?0");
    headers = curl_slist_append(headers, "Cache-Control: max-age=0");
    headers = curl_slist_append(headers, "Sec-Ch-Ua-Platform: \"Windows\"");
    headers = curl_slist_append(headers, "Accept-Language: en-US,en;q=0.9");
    headers = curl_slist_append(headers, "Origin: https://hello.iitk.ac.in");
    headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1");
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.6778.86 Safari/537.36");
    headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
    headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
    headers = curl_slist_append(headers, "Sec-Fetch-Mode: navigate");
    headers = curl_slist_append(headers, "Sec-Fetch-User: ?1");
    headers = curl_slist_append(headers, "Sec-Fetch-Dest: document");
    headers = curl_slist_append(headers, "Referer: https://hello.iitk.ac.in/user/login");
    headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate, br");
    headers = curl_slist_append(headers, "Priority: u=0, i");
    headers = curl_slist_append(headers, "Connection: keep-alive");
    headers = curl_slist_append(headers, "Cookie: uid=60019; role=student; username=; _ga=GA1.1.975615793.1728786340; Drupal.visitor.autologout_login=1732970513; token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJodHRwczovL2hlbGxvLmlpdGsuYWMuaW4iLCJpYXQiOjE3MzI5NzA1MTMsImV4cCI6MjA0ODMzMDUxMywic3ViIjoiNTcwMDM1ODk4NGRjN2VlOTBiZmM1MzUzZGU3N2MxOTA2MmUzMzdiZiJ9.QKfbwmEbHcMr2RFfSpIdxS_VeVBtTsXGFspOk9v8zIE; _ga_ZLD0V1ECY0=GS1.1.1732970497.2.1.1732970914.0.0.0");
}

void LoginRequest::perform_login(){
    string data  = "name="+username+"&pass="+password + "&form_build_id=form-D7BDjbvvC4L1qoE4qfCR1dU6G1C3F0WHAntj3P4ykTE&form_id=user_login_form&op=SIGN+IN" ;
    const string post_data = data ;
     
     setheader();

     if(curl){
        curl_easy_setopt(curl,CURLOPT_POST,1L) ;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers) ;
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS , post_data.c_str()) ; 
        curl_easy_setopt(curl , CURLOPT_URL, url.c_str()) ;
        //curl_easy_setopt(curl , CURLOPT_HEADER , 1L) ; 
        curl_easy_setopt(curl , CURLOPT_HEADERFUNCTION , header_callback) ;
        curl_easy_setopt(curl, CURLOPT_HEADERDATA ,header_body ) ;
        curl_easy_setopt(curl , CURLOPT_WRITEFUNCTION,write_callback) ;
        curl_easy_setopt(curl , CURLOPT_WRITEDATA , response)  ; 
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        //curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);

    
        res = curl_easy_perform(curl) ; 

        if(res!=CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
    }
}

