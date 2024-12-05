#include "login.h"
#include "lectures.h"
#include "course_html.h"
#include "course_extractor.h"
#include<filesystem>
#include <iostream>
#include <string>

using namespace std;
namespace fs = std::filesystem;


void request(string url , string path , string att){
    url += "/" + att ;
    cout << url << '\n';
    path += att ;
    if(!fs::exists(path)){
        fs::create_directory(path);
    }
    string file_path = path +"/" + att +".json" ;
    //FILE* output = fopen(file_path.c_str() , "w") ;
    try{
        CourseHtml coursehtml ;
        coursehtml.url = url ;
        coursehtml.courselist =  fopen(file_path.c_str() , "w") ;//output ;
        coursehtml.extract_token();
        coursehtml.perform_request() ;
    }catch(const exception &e){
        cerr << "Error4 occured: " << e.what() << endl ;
    }

    //fclose(output) ; 

    
}

void fetch_json(const Course course){
    string main_dir = course.year_of_study ;
    if(!fs::exists(main_dir)){
        fs::create_directory(main_dir);
    }
    string course_dir = main_dir +"/" +course.course_name ;
    if(!fs::exists(course_dir)){
        fs::create_directory(course_dir) ;
    }
    string path = course_dir + "/" ;
    string url = "https://hello.iitk.ac.in/studentapi/" ;
    url+= course.course_link ;
    request(url , path , "lectures") ;
    lecture_parsing(path+"lectures/lectures.json" ,path);
    request(url , path ,  "assignments") ;
    request(url , path , "resources") ;

}



int main() {
   
    vector<Course> courses ;
    string username , password;
    cout << "Enter your username : " << endl ;
    getline(cin,username) ;
    cout <<"\nEnter your password : " << endl ;
    getline(cin,password);

    try {
        LoginRequest loginRequest;

        loginRequest.username = username;
        loginRequest.password = password ;

        loginRequest.perform_login();
        //cout << "Login request sent successfully!" << endl;
    } catch (const exception &e) {
        cerr << "Error1 occurred: " << e.what() << endl;
    }

    try{
        CourseHtml coursehtml;
        coursehtml.url= "https://hello.iitk.ac.in/courses" ;
        coursehtml.courselist = fopen("courselist.txt", "w") ; 
        coursehtml.header_body = fopen("header_body.txt", "w") ;
        coursehtml.extract_token();
        coursehtml.perform_request() ;
    }catch (const exception &e) {
        cerr << "Error2 occurred: " << e.what() << endl;
    }

    try{ifstream courselist("courselist.txt") ; 
    string line , htmlResponse ;
    while(getline(courselist , line)){
        htmlResponse += line ;
        //cout << line << '\n';
    }
    //cout << htmlResponse ;
    courses = extractCourseDetails(htmlResponse);
    }catch(const exception &e){
        cerr << "Error3 occured: " << e.what() << endl ;
    }

    for (const auto& course : courses) {
        cout << "\nCourse Name: " << course.course_name << "\n";
        cout << "Course Code: " << course.course_code << "\n";
        cout << "Course Link: " << course.course_link << "\n";
        cout << "Instructor Name: " << course.instructor_name << "\n";
        cout << "Year of Study: " << course.year_of_study << "\n\n";
        fetch_json(course) ;
    }


    return 0;
}
