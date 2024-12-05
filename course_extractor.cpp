#include "course_extractor.h"
#include <iostream>

std::string extractBetweenTags(const std::string& str, const std::string& startTag, const std::string& endTag) {
    size_t pos = str.find(startTag);
    if (pos == std::string::npos) return "";
    pos += startTag.size();
    size_t endpos = str.find(endTag, pos);
    if (endpos == std::string::npos) return "";
    std::string result = str.substr(pos, endpos - pos);
    size_t start = result.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : result.substr(start);
}

std::string name(const std::string& str) {
    return extractBetweenTags(str, "<div class=\"title\">", "</div>");
}

std::string id(const std::string& str) {
    return extractBetweenTags(str, "<div class=\"course-id\">", "</div>");
}

std::string link(const std::string& str) {
    return extractBetweenTags(str, "<a href=\"https://hello.iitk.ac.in/studio/", "/student");
}

std::string instructor(const std::string& str) {
    return extractBetweenTags(str, "<div class=\"instruct-name\"><p>", "</p>");
}

std::string yoe(const std::string& str) {
    return extractBetweenTags(str, "<div class=\"instruct-name\">Sem: ", "</div>");
}

std::vector<Course> extractCourseDetails(const std::string& html) {
    std::vector<Course> final_list;
    const std::string startTag = "<div class=\"course-card\">";
    const std::string openTag = "<div";
    const std::string closeTag = "</div>";
    size_t pos = 0;

    while ((pos = html.find(startTag, pos)) != std::string::npos) {
        int openDivs = 1, closeDivs = 0;
        size_t startPos = pos;
        size_t endPos = pos;
        
        while (openDivs != closeDivs) {
            size_t nextOpen = html.find(openTag, endPos + 1);
            size_t nextClose = html.find(closeTag, endPos + 1);

            if (nextClose < nextOpen || nextOpen == std::string::npos) {
                ++closeDivs;
                endPos = nextClose;
            } else {
                ++openDivs;
                endPos = nextOpen;
            }
        }

        std::string courseHTML = html.substr(startPos, endPos - startPos + closeTag.size());

        Course course;
        course.course_name = name(courseHTML);
        course.course_code = id(courseHTML);
        course.course_link = link(courseHTML);
        course.instructor_name = instructor(courseHTML);
        course.year_of_study = yoe(courseHTML);

        final_list.push_back(course);

        pos = endPos + closeTag.size();
    }

    return final_list;
}
