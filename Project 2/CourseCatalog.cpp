#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <algorithm>

// Represents a single course
class Course {
public:
    std::string courseNumber;
    std::string title;
    std::vector<std::string> prerequisites;

    // Default constructor
    Course() = default;

    // initializes course with number and title
    Course(const std::string& number, const std::string& name) : courseNumber(number), title(name) {}

    // Adds a prerequisite course number to the course
    void addPrerequisite(const std::string& prerequisite) {
        prerequisites.push_back(prerequisite);
    }
};

// Hash table that stores courses by their course number
using HashTable = std::unordered_map<std::string, Course>;

// Converts a string to uppercase
std::string toUpperCase(const std::string& str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return upperStr;
}

// Loads course data from csv file into the hash table
void loadData(const std::string& fileName, HashTable& courses) {
    std::ifstream file(fileName);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> tokens;

        // Split line into tokens separated by commas
        while (getline(ss, item, ',')) {
            tokens.push_back(item);
        }

        // First token is course number, second is title; remaining tokens are prerequisites
        Course course(tokens[0], tokens[1]);
        for (size_t i = 2; i < tokens.size(); ++i) {
            course.addPrerequisite(tokens[i]);
        }

        // Stores the course in the hash table
        courses[toUpperCase(course.courseNumber)] = course;
    }

    file.close();
}

// Displays the main menu
void printMenu() {
    std::cout << "\n1. Load Data Structure\n"
              << "2. Print Course List\n"
              << "3. Print Course\n"
              << "4. Exit\n"
              << "Enter your choice: ";
}

// Prints all courses in alphanumeric order
void printAllCourses(const HashTable& courses) {
    std::vector<std::string> sortedKeys;

    // Extracts and sorts course numbers
    for (const auto& pair : courses) {
        sortedKeys.push_back(pair.first);
    }
    std::sort(sortedKeys.begin(), sortedKeys.end());

    // Prints the courses in sorted order
    for (const auto& key : sortedKeys) {
        const auto& course = courses.at(key);
        std::cout << course.courseNumber << ", " << course.title << std::endl;
    }
}

// Prints details for a specific course. Makes it upercase for case-sensitive lookup
void printCourse(const HashTable& courses, const std::string& courseNumber) {
    std::string upperCourseNumber = toUpperCase(courseNumber);

    auto it = courses.find(upperCourseNumber);
    if (it != courses.end()) {
        const Course& course = it->second;
        std::cout << "Course: " << course.courseNumber << " - " << course.title << "\nPrerequisites: ";
        for (const auto& prereq : course.prerequisites) {
            std::cout << prereq << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Course not found!" << std::endl;
    }
}

int main() {
    HashTable courses;
    std::string fileName;
    int choice;

    while (true) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear(); // Clear the error notification
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(); // Ignore the newline character after the number

        switch (choice) {
            case 1:
                std::cout << "Enter filename: ";
                getline(std::cin, fileName);
                loadData(fileName, courses);
                break;
            case 2:
                printAllCourses(courses);
                break;
            case 3:
                std::cout << "Enter course number: ";
                getline(std::cin, fileName); // Reusing fileName variable for course number input
                printCourse(courses, fileName);
                break;
            case 4:
                return 0; // Exit the program
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}
