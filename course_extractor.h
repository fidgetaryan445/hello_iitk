#ifndef COURSE_EXTRACTOR_H
#define COURSE_EXTRACTOR_H

#include <string>
#include <vector>

struct Course {
    std::string course_name;
    std::string course_code;
    std::string course_link;
    std::string instructor_name;
    std::string year_of_study;
};


std::vector<Course> extractCourseDetails(const std::string& html);

#endif // COURSE_EXTRACTOR_H
