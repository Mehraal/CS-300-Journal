//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Alex Mehr
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Project Two
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;



// forward declarations
double strToDouble(string str, char ch);

void findCoursesAndPrerequisites(int course);

// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string courseName;
    vector<string> prerequisiteList;

    string getCourseName() {
        return courseName;
    }

    vector<string> getPrerequisites() {
        return prerequisiteList;

    }
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

class BinarySearchTree {

private:
    Node* root;
    void inOrder(Node* node);
    int size = 0;

public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course aCourse);
    void Remove(string courseId);
    Course Search(const string& courseId);
    int Size();
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // Initialize housekeeping variables
    root = nullptr;
}

void BinarySearchTree::InOrder() {
    inOrder(root);
}



void BinarySearchTree::Insert(Course aCourse) {
    Node* currentNode = root;
    if (root == NULL) {
        root = new Node(aCourse);
    } else {
        while (currentNode != NULL){
            if(aCourse.courseId < currentNode->course.courseId){
                if(currentNode->left == nullptr){
                    currentNode->left = new Node(aCourse);
                    currentNode = nullptr;
                }
                else {
                    currentNode = currentNode->left;
                }
            }
            else {

                if (currentNode->right == nullptr) {

                    currentNode->right = new Node(aCourse);
                    currentNode = NULL;
                }
            else {
                currentNode = currentNode->right;
            }
        }
    }
}
size++;
}


void BinarySearchTree::Remove(string courseId) {
    Node* par = NULL;
    Node* current = root;

    while (current != NULL) {
        if (current->course.courseId == courseId) {
            if (current->left == NULL && current->right == NULL) {
                if (par == NULL) {
                    root = nullptr;
                } else if (par->left == current) {
                    par->left = NULL;
                } else {
                    par->right = NULL;
                }
            } else if (current->right == NULL) {
                if (par == NULL) {
                    root = current->left;
                } else if (par->left == current) {
                    par->left = current->left;
                } else {
                    par->right = current->left;
                }
            } else if (current->left == NULL) {
                if (par == NULL) {
                    root = current->right;
                } else if (par->left == current) {
                    par->left = current->right;
                } else {
                    par->right = current->right;
                }
            } else {
                Node *suc = current->right;

                while (suc->left != NULL) {
                    suc = suc->left;

                }
                Node successorData = Node(suc->course);
                Remove(suc->course.courseId);
                current->course = successorData.course;
            }
            return;
        }
        else if (current->course.courseId < courseId) {
                par = current;
                current = current->right;
            }
        else {
                par = current;
                current = current->left;
            }
        }
        cout << "\nValue not found" << endl;

}

/**
 * Search for a bid
 */
Course BinarySearchTree::Search(const string& courseId) {
    Node* current = root;

    while (current!= nullptr) {
        if (current->course.courseId == courseId) {
            return current->course;
        }
        if (courseId.compare(current->course.courseId) < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    // If the course is not found, throw an exception or return a default value
    throw runtime_error("Course not found");
    // or return a default value
    // Course course;
    // return course;
}


void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        return;
    }
    inOrder(node->left); //traverse to left side first
    cout << node->course.courseId << ", " << node->course.courseName << endl;
    inOrder(node->right);  //Traverse th node to right side after checking each level
}

void displayCourse(const Course& aCourse) {
    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    if (aCourse.prerequisiteList.empty()) {//if the list is empty then there are no prereq

        cout << "none" << endl;
    }
    else {

        for (unsigned int i = 0; i < aCourse.prerequisiteList.size(); i++) {

            cout << aCourse.prerequisiteList.at(i);

            if (i < aCourse.prerequisiteList.size() - 1) {//put a comma for any elements greater than 1

                cout << ", ";
            }
        }
        cout << endl; // add a newline after printing all prerequisites
    }
}

//void findCoursesAndPrerequisites(int course) {
//    cout << "Course: " << course.getCourseName() << endl;
//    cout << "Prerequisites: ";
//    for (const auto& prerequisite : course.getPrerequisites()) {
//        cout << prerequisite.getPrerequisites() << " ";
//    }
//    cout << endl;
//
//}


void loadCourses(const string& csvPath, BinarySearchTree* courseList) {
    ifstream inFS(csvPath); // open the file in constructor

    if (!inFS.is_open()) { // error handling
        cerr << "Could not open file. Please check inputs." << endl;
        return;
    }

    string line; //line feed
    while (getline(inFS, line)) {
        vector<string> stringTokens;
        stringstream ss(line);
        string token;

        while (getline(ss, token, ',')) {
            stringTokens.push_back(token);
        }

        if (stringTokens.size() < 2) { //if there aren't 2 tokens the line is misformatted
            cout << "\nError. Skipping line." << endl;
            continue;
        }

        Course aCourse;//create a new struct for each "line"
        aCourse.courseId = stringTokens.at(0);
        aCourse.courseName = stringTokens.at(1);

        for (unsigned int i = 2; i < stringTokens.size(); i++) {
            aCourse.prerequisiteList.push_back(stringTokens.at(i));
        }

        // push this course to the end
        courseList->Insert(aCourse);
    }

    inFS.close(); //close the file
}

/**
 * The one and only main() method
 */
int main(int argc, char *argv[]) {

    // process command line arguments
    string csvPath, aCourseKey;
    switch (argc) {
        case 2:
            csvPath = argv[1];
            break;
        case 3:
            csvPath = argv[1];
            aCourseKey = argv[2];
            break;
        default:
            csvPath = "ABCU_Advising_Program_Input.csv";
    }

    cout << "Using CSV file: " << csvPath << endl;

    BinarySearchTree courseList;

    int choice;
    while (true) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All courses" << endl;
        cout << "  3. Find courses and prerequisites" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        cin >> choice;

        try {
            if (choice == 1) {
                loadCourses(csvPath, &courseList);
            } else if (choice == 2) {
                courseList.InOrder();
            } else if (choice == 3) {
                string courseId;
                cout << "Enter course ID: ";
                cin >> courseId;
//                findCoursesAndPrerequisites(courseId, &courseList);
            } else if (choice == 9) {
                cout << "Good Bye" << endl;
                break;
            } else {
                throw runtime_error("Invalid choice");
            }
        } catch (const exception& e) {
            cout << "\nError: " << e.what() << endl;
        }
    }

    return 0;
}



