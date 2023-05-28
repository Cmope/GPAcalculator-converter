
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include <limits> // Added for clearing cin buffer

std::vector<std::pair<std::string, float>> courses;
float currentScale = 10;
float toConvertScale = 10;

void addOCourse()
{
    std::pair<std::string, float> course;
    std::cout << "Please enter course name: ";
    std::getline(std::cin >> std::ws, course.first);
    course.second = -99;
    courses.push_back(course);
}

void addCourse()
{
    std::pair<std::string, float> course;
    std::cout << "Please enter course name: ";
    std::cin.ignore();
    std::getline(std::cin, course.first);
    std::cout << "Now enter the grade: ";
    std::string gradeInput;
    std::getline(std::cin, gradeInput);

    try {
        course.second = std::stof(gradeInput);
    } catch (const std::invalid_argument& e) {
        std::cout << "ERROR: please try again and enter a number." << std::endl;
        return;
    }
    courses.push_back(course);
}

void writeCourse()
{
    std::string filename;
    std::cout << "Please enter filename to save to: ";
    std::getline(std::cin >> std::ws, filename);
    std::ofstream file(filename);
    if (file.is_open()) {
        file << currentScale << std::endl;
        for (const auto& course : courses) {
            file << course.first << "," << course.second << std::endl;
        }
        file.close();
        std::cout << "Courses have been successfully saved in: " << filename << std::endl;
    } else {
        std::cout << "Unable to save to file " << filename << ". Please try again with another filename." << std::endl;
    }
}

void readIn()
{
    std::string filename;
    std::cout << "Please specify the filename: ";
    std::getline(std::cin >> std::ws, filename);
    std::ifstream file(filename);
    if (file.is_open()) {
        courses.clear();
        std::string line;
        std::getline(file, line);
        currentScale = std::stof(line);
        toConvertScale = std::stof(line);
        while (std::getline(file, line)) {
            std::string courseName;
            float courseGrade;
            size_t commaPos = line.find(',');
            if (commaPos != std::string::npos) {
                courseName = line.substr(0, commaPos);
                courseGrade = std::stof(line.substr(commaPos + 1));
                courses.emplace_back(courseName, courseGrade);
            }
        }
        file.close();
        std::cout << "Courses successfully loaded." << std::endl;
    } else {
        std::cout << "ERROR: Unable to open file." << std::endl;
    }
}

float averageGPA()
{
    int noCourses = 0;
    float total = 0;
    for (auto& course : courses) {
        if (course.second != -99) {
            total += (course.second / currentScale) * toConvertScale;
            course.second = (course.second / currentScale) * toConvertScale;
            noCourses++;
        }
    }
    if (noCourses == 0) {
        return 0.0; // Return 0.0 if no valid courses found
    }
    float average = total / noCourses;
    return average;
}

void printCourse()
{
    std::cout << "Course Overview" << std::endl;
    std::cout << std::left << std::setw(40) << "Course Name" << " Course Grade" << std::endl;
    std::cout << "==================================================" << std::endl;
    for (const auto& course : courses) {
        if (course.second == -99) {
            std::cout << std::left << std::setw(40) << course.first << " | course not passed yet" << std::endl;
        } else {
            std::cout << std::left << std::setw(40) << course.first << " | " << course.second << std::endl;
        }
    }
    std::cout << "Average GPA: " << averageGPA() << std::endl;
}

void changeScale()
{
    std::cout << "Enter the new scale: ";
    std::cin >> toConvertScale;
    if (toConvertScale <= 0) {
        std::cout << "ERROR: Invalid scale value. Scale must be greater than zero." << std::endl;
        return;
    }
    averageGPA();
    currentScale = toConvertScale;
    std::cout << "Conversion scale changed successfully." << std::endl;
}

void updateGrade()
{
    std::string course;
    std::cout << "Enter the course name: ";
    std::getline(std::cin >> std::ws, course);
    for (auto& crs : courses) {
        if (crs.first == course) {
            std::cout << "Enter new grade: ";
            std::string gradeInput;
            std::getline(std::cin >> std::ws, gradeInput);
            try {
                float newGrade = std::stof(gradeInput);
                crs.second = newGrade;
                std::cout << "Grade updated successfully." << std::endl;
                return;
            } catch (const std::invalid_argument& e) {
                std::cout << "ERROR: Please try again and enter a valid number." << std::endl;
                return;
            }
        }
    }
    std::cout << "ERROR: Course not found." << std::endl;
}

void removeCourse()
{
    std::string course;
    std::cout << "Enter the course name: ";
    std::getline(std::cin >> std::ws, course);
    for (auto it = courses.begin(); it != courses.end(); ++it) {
        if (it->first == course) {
            courses.erase(it);
            std::cout << "Course removed." << std::endl;
            return;
        }
    }
    std::cout << "ERROR: Course not found." << std::endl;
}

void menu()
{
    int choice = 0;
    while (choice != 8) {
        printCourse();
        std::cout << "1. Add a new course" << std::endl;
        std::cout << "2. Add a new course and grade" << std::endl;
        std::cout << "3. Adjust average GPA scale" << std::endl;
        std::cout << "4. Update grade" << std::endl;
        std::cout << "5. Remove course" << std::endl;
        std::cout << "6. Save to file" << std::endl;
        std::cout << "7. Load from file" << std::endl;
        std::cout << "8. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                // Add a new course
                addOCourse();
                break;
            case 2:
                // Add a new course and grade
                addCourse();
                break;
            case 3:
                // Adjust average GPA scale
                changeScale();
                break;
            case 4:
                // Update grade
                updateGrade();
                break;
            case 5:
                // Remove course
                removeCourse();
                break;
            case 6:
                // Save to file
                writeCourse();
                break;
            case 7:
                // Load from file
                readIn();
                break;
            case 8:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }

        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore remaining characters in cin buffer
    }
}

int main()
{
    menu();
    return 0;
}