#ifndef LECTURES
#define LECTURES

#include <iostream>
#include <fstream>
#include <string>
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <cstring>

using namespace std;

string read_json_file(const string &filepath);

size_t write_callback(void* contents, size_t size, size_t nmemb, FILE* file);

bool get_file_from_url(const string &url, const string &path, const string &filename) ;

string extract_file_extension(const string &url);

string encode_url(const string &url);

void parse_json(string json_data, const string path);

void lecture_parsing(const string json_path, const string path);

#endif //LECTURES