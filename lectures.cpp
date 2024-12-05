#include "lectures.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <cstring>

using namespace std;

string read_json_file(const string &filepath) {
    ifstream file(filepath, ios::ate | ios::binary);
    if (!file) {
        return "";
    }
    string content(file.tellg(), '\0');
    file.seekg(0);
    file.read(&content[0], content.size());

    return content;
}

size_t write_callback(void* contents, size_t size, size_t nmemb, FILE* file) {
    size_t written = fwrite(contents, size, nmemb, file);
    return written;
}

bool get_file_from_url(const string &url, const string &path, const string &filename) {
    CURL *curl;
    CURLcode res;
    FILE *file;

    string file_name = path + "lectures/" + filename;

    file = fopen(file_name.c_str(), "wb");

    if (file == NULL) {
        cerr << "Failed to open the file" << endl;
        return false;
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cerr << "libcurl error" << endl;
            return false;
        }

        fclose(file);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return true;
}

string extract_file_extension(const string &url) {
    size_t pos = url.rfind(".");
    if (pos == string::npos) {
        return "";
    }
    return url.substr(pos);
}

string encode_url(const string &url) {
    string encoded_url = url;

    size_t pos = 0;
    while ((pos = encoded_url.find(" "), pos) != string::npos) {
        encoded_url.replace(pos, 1, "%20");
        pos += 3;
    }
    return encoded_url;
}

void parse_json(string json_data, const string path) {
    cJSON *root = cJSON_Parse(json_data.c_str());

    if (root == NULL) {
        cerr << "Error Parsing Json" << endl;
        return;
    }

    cJSON *array = cJSON_GetObjectItem(root, "data");
    size_t data_size = cJSON_GetArraySize(array);

    for (int i = 0; i < data_size; i++) {
        cJSON *item = cJSON_GetArrayItem(array, i);

        cJSON* id_item = cJSON_GetObjectItem(item, "id");
        cJSON* title_item = cJSON_GetObjectItem(item, "title");
        cJSON* files_item = cJSON_GetObjectItem(item, "files");
        string title = title_item->valuestring ;
        size_t files_item_size = cJSON_GetArraySize(files_item);
        if (files_item && cJSON_GetArraySize(files_item) > 0) {

            for (int j = 0; j < files_item_size; j++) {
                cJSON* file = cJSON_GetArrayItem(files_item, j);

                cJSON* filename_item = cJSON_GetObjectItem(file, "filename");
                cJSON* fileurl_item = cJSON_GetObjectItem(file, "fileUrl");

                const string name = filename_item->valuestring;
                const string url = fileurl_item->valuestring;

                string encoded_url = encode_url(url);

                string extension = extract_file_extension(encoded_url);

                const string final_file_name = title +"_(" + to_string(j)+")" + extension;

                get_file_from_url(encoded_url, path, final_file_name);
            }
        }
    }

    cJSON_Delete(root);
}

void lecture_parsing(const string json_path, const string path) {
    string json_data = read_json_file(json_path);
    parse_json(json_data, path);
}
