#include <iostream>
#include <ctime>
#include <fstream>
#include <functional>
#include <iomanip>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

// Calendar structure to manage dates from 2024-2034
class Calendar {
public:
  map<string, bool>
      attendanceMap; // "YYYY-MM-DD" -> true(present)/false(absent)

  bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
  }

  int getDaysInMonth(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year))
      return 29;
    return days[month - 1];
  }

  string getDateKey(int year, int month, int day) {
    stringstream ss;
    ss << year << "-" << setfill('0') << setw(2) << month << "-" << setw(2)
       << day;
    return ss.str();
  }

  void markAttendance(int year, int month, int day, bool present) {
    string key = getDateKey(year, month, day);
    attendanceMap[key] = present;
  }

  bool getAttendance(int year, int month, int day) {
    string key = getDateKey(year, month, day);
    if (attendanceMap.find(key) != attendanceMap.end()) {
      return attendanceMap[key];
    }
    return false; // Default: absent
  }

  int getTotalPresent() {
    int count = 0;
    for (auto &entry : attendanceMap) {
      if (entry.second)
        count++;
    }
    return count;
  }

  int getTotalAbsent() {
    int count = 0;
    for (auto &entry : attendanceMap) {
      if (!entry.second)
        count++;
    }
    return count;
  }

  int getTotalDays() { return attendanceMap.size(); }
};

class Subject {
public:
  string subjectName;
  string marksStatus;
  Subject *next;

  Subject(string name, string status) {
    subjectName = name;
    marksStatus = status;
    next = nullptr;
  }
};

class Student {
public:
  string rollNo; // Changed to string
  string name;
  string fatherName;
  string className;
  string classCategory;
  int admissionYear;
  int currentYear; // Current academic year
  string dateOfBirth;
  string cnicBForm;
  string address;
  Subject *subjectHead;
  int totalSubjects;
  float totalMarks;
  float maxMarks;
  float percentage;
  char grade;
  int termsCompleted;     // Track number of terms completed
  bool boardMarksEntered; // Track if board marks are entered
  Calendar attendance;    // Attendance calendar
  Student *next;
  Student *left;
  Student *right;

  Student(string r, string n, string fn, string cn, string cc, int ay,
          string dob, string cnic, string addr) {
    rollNo = r;
    name = n;
    fatherName = fn;
    className = cn;
    classCategory = cc;
    admissionYear = ay;
    currentYear = ay;
    dateOfBirth = dob;
    cnicBForm = cnic;
    address = addr;
    subjectHead = nullptr;
    totalSubjects = 0;
    totalMarks = 0;
    maxMarks = 0;
    percentage = 0;
    grade = 'F';
    termsCompleted = 0;
    boardMarksEntered = false;
    next = nullptr;
    left = nullptr;
    right = nullptr;
  }

  int getMaxMarksForSubject(string subjectName) {
    try {
      int classNum = stoi(className);
      if (classNum >= 8 && classNum <= 12) {
        if (subjectName == "Physics" || subjectName == "Chemistry" ||
            subjectName == "Computer Science" || subjectName == "Biology") {
          return 75;
        }
      }
    } catch (...) {
      // If className is not a valid number, default to 100
    }
    return 100;
  }

  void addSubject(string subjectName, string status) {
    Subject *newSubject = new Subject(subjectName, status);

    if (subjectHead == nullptr) {
      subjectHead = newSubject;
    } else {
      Subject *temp = subjectHead;
      while (temp->next != nullptr) {
        temp = temp->next;
      }
      temp->next = newSubject;
    }

    totalSubjects++;

    if (status != "A" && status != "L" && status != "-") {
      float marks = stof(status);
      totalMarks += marks;
      int maxMarksForSub = getMaxMarksForSubject(subjectName);
      maxMarks += maxMarksForSub;
    }

    if (maxMarks > 0) {
      percentage = (totalMarks / maxMarks) * 100.0;
      grade = calculateGrade(percentage);
    }
  }

  static char calculateGrade(float perc) {
    if (perc >= 90)
      return 'A';
    else if (perc >= 80)
      return 'B';
    else if (perc >= 70)
      return 'C';
    else if (perc >= 60)
      return 'D';
    else if (perc >= 50)
      return 'E';
    else
      return 'F';
  }

  void clearSubjects() {
    Subject *current = subjectHead;
    while (current != nullptr) {
      Subject *next = current->next;
      delete current;
      current = next;
    }
    subjectHead = nullptr;
    totalSubjects = 0;
    totalMarks = 0;
    maxMarks = 0;
    percentage = 0;
    grade = 'F';
  }

  // Check if student can be promoted
  bool canBePromoted() {
    // Must have 3 terms completed
    if (termsCompleted < 3) {
      return false;
    }

    // Must have board marks entered (for classes 8-12)
    try {
      int classNum = stoi(className);
      if (classNum >= 8 && classNum <= 12) {
        if (!boardMarksEntered) {
          return false;
        }
      }
    } catch (...) {
      return false; // Invalid class number
    }

    // Must have percentage calculated
    if (maxMarks == 0 || percentage == 0) {
      return false;
    }

    return true;
  }

  // Auto-promote to next class (only if eligible)
  bool promoteToNextClass() {
    if (!canBePromoted()) {
      return false;
    }

    try {
      int classNum = stoi(className);
      if (classNum < 12) {
        classNum++;
        className = to_string(classNum);
        currentYear++;
        // Clear old marks for new year
        clearSubjects();
        termsCompleted = 0;
        boardMarksEntered = false;
        return true;
      }
    } catch (...) {
      return false; // Invalid class number
    }
    return false;
  }

  ~Student() { clearSubjects(); }
};

class StudentManager {
private:
  Student *head;
  int totalStudents;
  map<string, Student *> hashTable; // String-based hash table

  Student *searchInHashTable(string rollNo) {
    if (hashTable.find(rollNo) != hashTable.end()) {
      return hashTable[rollNo];
    }
    return nullptr;
  }

  void addToHashTable(Student *student) {
    hashTable[student->rollNo] = student;
  }

  void removeFromHashTable(string rollNo) { hashTable.erase(rollNo); }

  void showStudentInfo(Student *student) {
    if (student == nullptr)
      return;
    cout << "\n+------------------------------------------------------+\n";
    cout << "  Roll Number          : " << student->rollNo << "\n";
    cout << "  Name                 : " << student->name << "\n";
    cout << "  Father's Name        : " << student->fatherName << "\n";
    cout << "  Class                : " << student->className << "\n";
    if (!student->classCategory.empty()) {
      cout << "  Category             : " << student->classCategory << "\n";
    }
    cout << "  Admission Year       : " << student->admissionYear << "\n";
    cout << "  Current Year         : " << student->currentYear << "\n";
    cout << "  Date of Birth        : " << student->dateOfBirth << "\n";
    cout << "  CNIC/B-Form          : " << student->cnicBForm << "\n";
    cout << "  Address              : " << student->address << "\n";

    // Attendance info
    cout << "  ---------------------------------------------------\n";
    cout << "  ATTENDANCE SUMMARY:\n";
    cout << "  Total Days Marked    : " << student->attendance.getTotalDays()
         << "\n";
    cout << "  Present Days         : " << student->attendance.getTotalPresent()
         << "\n";
    cout << "  Absent Days          : " << student->attendance.getTotalAbsent()
         << "\n";

    cout << "  ---------------------------------------------------\n";
    cout << "  SUBJECT DETAILS:\n";
    cout << "  ---------------------------------------------------\n";

    Subject *current = student->subjectHead;
    int count = 1;
    while (current != nullptr) {
      cout << "  " << count << ". " << current->subjectName << " : ";
      if (current->marksStatus == "A") {
        cout << "Absent\n";
      } else if (current->marksStatus == "L") {
        cout << "Leave\n";
      } else if (current->marksStatus == "-") {
        cout << "Not Entered\n";
      } else {
        int maxMarksForSub =
            student->getMaxMarksForSubject(current->subjectName);
        cout << current->marksStatus << " / " << maxMarksForSub << "\n";
      }
      current = current->next;
      count++;
    }

    cout << "  ---------------------------------------------------\n";
    cout << "  Total Subjects       : " << student->totalSubjects << "\n";
    if (student->maxMarks > 0) {
      cout << "  Total Marks          : " << fixed << setprecision(2)
           << student->totalMarks << " / " << student->maxMarks << "\n";
      cout << "  Percentage           : " << student->percentage << "%\n";
      cout << "  Grade                : " << student->grade << "\n";
    } else {
      cout << "  Total Marks          : Not Calculated\n";
    }
    cout << "  ---------------------------------------------------\n";
    cout << "  Terms Completed      : " << student->termsCompleted << "/3\n";
    int classNum = stoi(student->className);
    if (classNum >= 8 && classNum <= 12) {
      cout << "  Board Marks Entered  : "
           << (student->boardMarksEntered ? "Yes" : "No") << "\n";
    }
    cout << "+------------------------------------------------------+\n";
  }

  string getValidInput(string prompt, bool canSkip,
                       function<bool(string)> validator = nullptr) {
    string input;
    while (true) {
      cout << prompt;
      if (canSkip)
        cout << " (Press Enter to skip)";
      cout << ": ";
      getline(cin, input);

      if (input.empty() && canSkip) {
        return "";
      }

      if (input.empty() && !canSkip) {
        cout << "[ERROR] This field cannot be empty! Please try again.\n";
        continue;
      }

      if (validator && !validator(input)) {
        cout << "[ERROR] Invalid input! Please try again.\n";
        continue;
      }

      return input;
    }
  }

  static bool validateCNIC(string cnic) {
    if (cnic.length() != 13)
      return false;
    for (char c : cnic) {
      if (c < '0' || c > '9')
        return false;
    }
    return true;
  }

  static bool validateDOB(string dob) {
    if (dob.length() != 8)
      return false;
    for (char c : dob) {
      if (c < '0' || c > '9')
        return false;
    }
    return true;
  }

  static bool validateYear(string year) {
    try {
      int y = stoi(year);
      return y >= 1900 && y <= 2100;
    } catch (...) {
      return false;
    }
  }

  static bool validateMarks(string marks, int maxMarks = 100) {
    if (marks == "A" || marks == "L" || marks == "-")
      return true;
    try {
      float m = stof(marks);
      return m >= 0 && m <= maxMarks;
    } catch (...) {
      return false;
    }
  }

  // Validate admission date based on class and academic year rules
  bool validateAdmissionDate(int classNum, int admissionYear, int currentMonth,
                             int currentYear) {
    // Classes 1-7: Papers in February, new session starts in March
    // Classes 8-10: Papers in February/March, new session in April
    // Classes 11-12: Papers in May/June, new session in June/July

    if (classNum >= 1 && classNum <= 7) {
      // For classes 1-7, admission should be in March (month 3) or later in the
      // year If current month is before March, admission year should be
      // previous year
      if (currentMonth < 3) {
        // If it's Jan/Feb, admission year should be previous year
        return (admissionYear == currentYear - 1);
      } else {
        // If it's March or later, admission year should be current year
        return (admissionYear == currentYear);
      }
    } else if (classNum >= 8 && classNum <= 10) {
      // For classes 8-10, papers in Feb/March, new session in April
      if (currentMonth < 4) {
        // If it's Jan/Feb/March, admission year should be previous year
        return (admissionYear == currentYear - 1);
      } else {
        // If it's April or later, admission year should be current year
        return (admissionYear == currentYear);
      }
    } else if (classNum >= 11 && classNum <= 12) {
      // For classes 11-12, papers in May/June, new session in June/July
      if (currentMonth < 6) {
        // If it's before June, admission year should be previous year
        return (admissionYear == currentYear - 1);
      } else {
        // If it's June or later, admission year should be current year
        return (admissionYear == currentYear);
      }
    }
    return true; // Default validation
  }

  // Get current date components
  void getCurrentDate(int &year, int &month, int &day) {
    // Get current system date
    time_t now = time(0);
    tm *ltm = localtime(&now);
    year = 1900 + ltm->tm_year;
    month = 1 + ltm->tm_mon;
    day = ltm->tm_mday;
  }

  string formatCNIC(string input) {
    return input.substr(0, 5) + "-" + input.substr(5, 7) + "-" +
           input.substr(12, 1);
  }

  string formatDOB(string input) {
    return input.substr(0, 2) + "-" + input.substr(2, 2) + "-" +
           input.substr(4, 4);
  }

  vector<string> getSubjectsForClass(string className, string category) {
    vector<string> subjects;

    if (className == "8" || className == "9" || className == "10") {
      if (category == "Computer Science") {
        subjects = {
            "English",      "Urdu",     "Maths",       "Physics",
            "Chemistry",    "Islamiat", "Pak Studies", "Computer Science",
            "Mutaila Quran"};
      } else if (category == "Biology") {
        subjects = {"English",     "Urdu",      "Maths",
                    "Physics",     "Chemistry", "Islamiat",
                    "Pak Studies", "Biology",   "Mutaila Quran"};
      }
    } else if (className == "11") {
      if (category == "Computer Science") {
        subjects = {"English",      "Urdu",     "Maths",
                    "Physics",      "Islamiat", "Computer Science",
                    "Mutaila Quran"};
      } else if (category == "Pre-Engineering") {
        subjects = {"English",   "Urdu",     "Maths",        "Physics",
                    "Chemistry", "Islamiat", "Mutaila Quran"};
      } else if (category == "Pre-Medical") {
        subjects = {"English",   "Urdu",     "Physics",      "Biology",
                    "Chemistry", "Islamiat", "Mutaila Quran"};
      }
    } else if (className == "12") {
      if (category == "Computer Science") {
        subjects = {"English",      "Urdu",        "Maths",
                    "Physics",      "Pak Studies", "Computer Science",
                    "Mutaila Quran"};
      } else if (category == "Pre-Engineering") {
        subjects = {"English",   "Urdu",        "Maths",        "Physics",
                    "Chemistry", "Pak Studies", "Mutaila Quran"};
      } else if (category == "Pre-Medical") {
        subjects = {"English",   "Urdu",        "Physics",      "Biology",
                    "Chemistry", "Pak Studies", "Mutaila Quran"};
      }
    }

    return subjects;
  }

public:
  StudentManager() {
    head = nullptr;
    totalStudents = 0;
  }

  void addStudent() {
    cout << "\n--- Adding Student ---\n";

    string rollNo = getValidInput("Enter Roll Number", false);
    if (searchInHashTable(rollNo) != nullptr) {
      cout << "[ERROR] Roll number already exists!\n";
      return;
    }

    string name = getValidInput("Enter Name", false);
    string fatherName = getValidInput("Enter Father's Name", false);
    string className = getValidInput("Enter Class (1-12)", false);

    string classCategory = "";
    vector<string> subjects;

    int classNum = stoi(className);

    if (classNum >= 1 && classNum <= 7) {
      string numStr = getValidInput("How many subjects? (minimum 1)", false);
      int numSubjects = stoi(numStr);

      if (numSubjects < 1) {
        cout << "[ERROR] At least 1 subject required!\n";
        return;
      }

      for (int i = 0; i < numSubjects; i++) {
        string subName =
            getValidInput("Subject " + to_string(i + 1) + " Name", false);
        subjects.push_back(subName);
      }
    } else if (classNum >= 8 && classNum <= 12) {
      cout << "\nSelect Category:\n";
      if (classNum >= 8 && classNum <= 10) {
        cout << "1. Computer Science\n2. Biology\n";
      } else {
        cout << "1. Computer Science\n2. Pre-Engineering\n3. Pre-Medical\n";
      }

      string choiceStr = getValidInput("Enter choice", false);
      int choice = stoi(choiceStr);

      if (classNum >= 8 && classNum <= 10) {
        if (choice == 1)
          classCategory = "Computer Science";
        else if (choice == 2)
          classCategory = "Biology";
      } else {
        if (choice == 1)
          classCategory = "Computer Science";
        else if (choice == 2)
          classCategory = "Pre-Engineering";
        else if (choice == 3)
          classCategory = "Pre-Medical";
      }

      subjects = getSubjectsForClass(className, classCategory);
    }

    string admissionYearStr =
        getValidInput("Enter Admission Year", false, validateYear);
    int admissionYear = stoi(admissionYearStr);

    string dob =
        getValidInput("Enter Date of Birth (DDMMYYYY)", true, validateDOB);
    string formattedDOB = dob.empty() ? "" : formatDOB(dob);

    string cnic =
        getValidInput("Enter CNIC/B-Form (13 digits)", true, validateCNIC);
    string formattedCNIC = cnic.empty() ? "" : formatCNIC(cnic);

    string address = getValidInput("Enter Address", true);

    Student *newStudent =
        new Student(rollNo, name, fatherName, className, classCategory,
                    admissionYear, formattedDOB, formattedCNIC, address);

    for (const string &sub : subjects) {
      newStudent->addSubject(sub, "-");
    }

    if (head == nullptr) {
      head = newStudent;
    } else {
      Student *temp = head;
      while (temp->next != nullptr) {
        temp = temp->next;
      }
      temp->next = newStudent;
    }

    addToHashTable(newStudent);
    totalStudents++;

    cout << "[SUCCESS] Student added successfully!\n";
    cout << "Total students: " << totalStudents << "\n";
  }

  void markAttendance(string rollNo) {
    cout << "\n--- Mark Attendance ---\n";

    Student *student = searchInHashTable(rollNo);
    if (student == nullptr) {
      cout << "[ERROR] Student not found!\n";
      return;
    }

    cout << "Student: " << student->name << "\n";

    string yearStr = getValidInput("Enter Year (2024-2034)", false);
    int year = stoi(yearStr);
    if (year < 2024 || year > 2034) {
      cout << "[ERROR] Year must be between 2024-2034!\n";
      return;
    }

    string monthStr = getValidInput("Enter Month (1-12)", false);
    int month = stoi(monthStr);
    if (month < 1 || month > 12) {
      cout << "[ERROR] Invalid month!\n";
      return;
    }

    string dayStr = getValidInput("Enter Day", false);
    int day = stoi(dayStr);
    int maxDays = student->attendance.getDaysInMonth(month, year);
    if (day < 1 || day > maxDays) {
      cout << "[ERROR] Invalid day for this month!\n";
      return;
    }

    string status = getValidInput("Mark as (P=Present, A=Absent)", false);
    bool present = (status == "P" || status == "p");

    student->attendance.markAttendance(year, month, day, present);
    cout << "[SUCCESS] Attendance marked!\n";
  }

  void promoteAllStudents() {
    cout << "\n--- Promoting All Students ---\n";
    Student *current = head;
    int promoted = 0;
    int notEligible = 0;

    // Check current date to determine if it's promotion time
    int currentYear, currentMonth, currentDay;
    getCurrentDate(currentYear, currentMonth, currentDay);

    while (current != nullptr) {
      int classNum = stoi(current->className);
      if (classNum < 12) {
        // Check if it's the right time for promotion based on class
        bool isPromotionTime = false;

        if (classNum >= 1 && classNum <= 7) {
          // Classes 1-7: Papers in February, new session starts in March
          isPromotionTime = (currentMonth >= 3);
        } else if (classNum >= 8 && classNum <= 10) {
          // Classes 8-10: Papers in February/March, new session in April
          isPromotionTime = (currentMonth >= 4);
        } else if (classNum >= 11 && classNum <= 12) {
          // Classes 11-12: Papers in May/June, new session in June/July
          isPromotionTime = (currentMonth >= 6);
        }

        if (isPromotionTime) {
          if (current->promoteToNextClass()) {
            promoted++;
          } else {
            notEligible++;
            cout << "[INFO] Student " << current->name
                 << " (Roll: " << current->rollNo
                 << ") cannot be promoted yet.\n";
            cout << "  - Terms completed: " << current->termsCompleted
                 << "/3\n";
            cout << "  - Board marks entered: "
                 << (current->boardMarksEntered ? "Yes" : "No") << "\n";
            cout << "  - Percentage calculated: "
                 << (current->percentage > 0 ? "Yes" : "No") << "\n";
          }
        } else {
          cout << "[INFO] It's not promotion time for class " << classNum
               << " yet.\n";
        }
      }
      current = current->next;
    }

    cout << "[SUCCESS] " << promoted << " students promoted!\n";
    if (notEligible > 0) {
      cout << "[INFO] " << notEligible
           << " students were not eligible for promotion.\n";
    }
  }

  void updateMarks(string rollNo) {
    cout << "\n--- Update Marks ---\n";

    Student *student = searchInHashTable(rollNo);
    if (student == nullptr) {
      cout << "[ERROR] Student not found!\n";
      return;
    }

    if (student->subjectHead == nullptr) {
      cout << "[ERROR] No subjects found!\n";
      return;
    }

    cout << "\nStudent: " << student->name << "\n";

    Subject *current = student->subjectHead;
    student->totalMarks = 0;
    student->maxMarks = 0;

    while (current != nullptr) {
      int maxMarksForSub = student->getMaxMarksForSubject(current->subjectName);
      // Create a lambda function for validation with specific max marks
      auto validateMarksForSub = [maxMarksForSub](string marks) -> bool {
        return validateMarks(marks, maxMarksForSub);
      };
      string marks =
          getValidInput("Enter marks for " + current->subjectName + " (0-" +
                            to_string(maxMarksForSub) + ", A=Absent, L=Leave)",
                        true, validateMarksForSub);

      if (!marks.empty()) {
        current->marksStatus = marks;
        if (marks != "A" && marks != "L" && marks != "-") {
          float markValue = stof(marks);
          if (markValue > maxMarksForSub) {
            cout << "[WARNING] Marks exceed maximum (" << maxMarksForSub
                 << "). Setting to maximum.\n";
            markValue = maxMarksForSub;
            current->marksStatus = to_string((int)markValue);
          }
          student->totalMarks += markValue;
          student->maxMarks += maxMarksForSub;
        }
      } else {
        if (current->marksStatus != "A" && current->marksStatus != "L" &&
            current->marksStatus != "-") {
          student->totalMarks += stof(current->marksStatus);
          student->maxMarks += maxMarksForSub;
        }
      }
      current = current->next;
    }

    if (student->maxMarks > 0) {
      student->percentage = (student->totalMarks / student->maxMarks) * 100.0;
      student->grade = Student::calculateGrade(student->percentage);
    }

    // Ask if these are board marks (for classes 8-12)
    int classNum = stoi(student->className);
    if (classNum >= 8 && classNum <= 12) {
      string boardMarks = getValidInput("Are these board marks? (Y/N)", false);
      if (boardMarks == "Y" || boardMarks == "y") {
        student->boardMarksEntered = true;
      }
    }

    cout << "[SUCCESS] Marks updated!\n";
  }

  void updateTerms(string rollNo) {
    cout << "\n--- Update Terms ---\n";

    Student *student = searchInHashTable(rollNo);
    if (student == nullptr) {
      cout << "[ERROR] Student not found!\n";
      return;
    }

    cout << "Student: " << student->name << "\n";
    cout << "Current terms completed: " << student->termsCompleted << "/3\n";

    string termsStr =
        getValidInput("Enter number of terms completed (0-3)", false);
    int terms = stoi(termsStr);
    if (terms < 0 || terms > 3) {
      cout << "[ERROR] Terms must be between 0 and 3!\n";
      return;
    }

    student->termsCompleted = terms;
    cout << "[SUCCESS] Terms updated!\n";
  }

  void manageSubjects(string rollNo) {
    cout << "\n--- Manage Subjects ---\n";

    Student *student = searchInHashTable(rollNo);
    if (student == nullptr) {
      cout << "[ERROR] Student not found!\n";
      return;
    }

    cout << "Student: " << student->name << " (Class: " << student->className
         << ")\n";
    cout << "\nCurrent Subjects:\n";

    Subject *current = student->subjectHead;
    int count = 1;
    while (current != nullptr) {
      cout << "  " << count << ". " << current->subjectName << "\n";
      current = current->next;
      count++;
    }

    cout << "\n[INFO] Subjects are managed automatically based on class and "
            "category.\n";
    cout << "To modify subjects, please delete and re-add the student with "
            "correct category.\n";
  }

  void deleteStudent(string rollNo) {
    cout << "\n--- Deleting Student ---\n";

    if (head == nullptr) {
      cout << "[ERROR] No students in system!\n";
      return;
    }

    Student *toDelete = searchInHashTable(rollNo);
    if (toDelete == nullptr) {
      cout << "[ERROR] Student not found!\n";
      return;
    }

    if (head->rollNo == rollNo) {
      head = head->next;
      removeFromHashTable(rollNo);
      delete toDelete;
      totalStudents--;
      cout << "[SUCCESS] Student deleted!\n";
      return;
    }

    Student *current = head;
    Student *previous = nullptr;

    while (current != nullptr && current->rollNo != rollNo) {
      previous = current;
      current = current->next;
    }

    if (current != nullptr) {
      previous->next = current->next;
      removeFromHashTable(rollNo);
      delete current;
      totalStudents--;
      cout << "[SUCCESS] Student deleted!\n";
    }
  }

  void searchStudent(string rollNo) {
    cout << "\n--- Search Student ---\n";
    Student *student = searchInHashTable(rollNo);
    if (student != nullptr) {
      cout << "[FOUND] Student Details:\n";
      showStudentInfo(student);
    } else {
      cout << "[NOT FOUND] No student with roll number " << rollNo << "\n";
    }
  }

  void displayAll() {
    if (head == nullptr) {
      cout << "\n[INFO] No students in system!\n";
      return;
    }

    cout << "\n========================================\n";
    cout << "       ALL STUDENT RECORDS\n";
    cout << "========================================\n";
    cout << "Total Students: " << totalStudents << "\n\n";

    Student *current = head;
    while (current != nullptr) {
      showStudentInfo(current);
      current = current->next;
    }
  }

  void saveToFile(string filename) {
    if (head == nullptr) {
      cout << "\n[WARNING] No data to save!\n";
      return;
    }

    ofstream file(filename);
    if (!file.is_open()) {
      cout << "[ERROR] Cannot open file!\n";
      return;
    }

    Student *current = head;
    while (current != nullptr) {
      file << current->rollNo << "|" << current->name << "|"
           << current->fatherName << "|" << current->className << "|"
           << current->classCategory << "|" << current->admissionYear << "|"
           << current->currentYear << "|" << current->dateOfBirth << "|"
           << current->cnicBForm << "|" << current->address << "|"
           << current->totalSubjects << "|" << current->termsCompleted << "|"
           << (current->boardMarksEntered ? "1" : "0") << "|";

      Subject *sub = current->subjectHead;
      while (sub != nullptr) {
        file << sub->subjectName << ":" << sub->marksStatus;
        if (sub->next != nullptr)
          file << ",";
        sub = sub->next;
      }
      file << "|";

      // Save attendance data
      for (auto &entry : current->attendance.attendanceMap) {
        file << entry.first << ":" << (entry.second ? "P" : "A") << ",";
      }

      file << "\n";
      current = current->next;
    }

    file.close();
    cout << "[SUCCESS] Data saved to " << filename << "\n";
  }

  void loadFromFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
      cout << "[ERROR] Cannot open file!\n";
      return;
    }

    string line;
    int loadedCount = 0;

    while (getline(file, line)) {
      vector<string> tokens;
      size_t pos = 0;

      while ((pos = line.find('|')) != string::npos) {
        tokens.push_back(line.substr(0, pos));
        line.erase(0, pos + 1);
      }
      tokens.push_back(line);

      if (tokens.size() >= 12) {
        string r = tokens[0];

        if (searchInHashTable(r) == nullptr) {
          Student *newStudent =
              new Student(r, tokens[1], tokens[2], tokens[3], tokens[4],
                          stoi(tokens[5]), tokens[7], tokens[8], tokens[9]);
          newStudent->currentYear = stoi(tokens[6]);

          // Load subjects (check old format first for backward compatibility)
          string subjectsData;
          if (tokens.size() >= 12) {
            // Check if new format (with terms and board marks)
            if (tokens.size() >= 14 && tokens[12].find(':') == string::npos) {
              // New format: terms and board marks are separate tokens
              newStudent->termsCompleted = stoi(tokens[12]);
              newStudent->boardMarksEntered = (tokens[13] == "1");
              subjectsData = tokens[11];
              // Attendance starts at token 14
            } else {
              // Old format: subjects at token 11, attendance at token 12
              subjectsData = tokens[11];
            }
          }
          size_t subPos;
          while ((subPos = subjectsData.find(',')) != string::npos) {
            string subPair = subjectsData.substr(0, subPos);
            size_t colonPos = subPair.find(':');
            if (colonPos != string::npos) {
              newStudent->addSubject(subPair.substr(0, colonPos),
                                     subPair.substr(colonPos + 1));
            }
            subjectsData.erase(0, subPos + 1);
          }
          // Handle last subject (no comma after it)
          if (!subjectsData.empty()) {
            size_t colonPos = subjectsData.find(':');
            if (colonPos != string::npos) {
              newStudent->addSubject(subjectsData.substr(0, colonPos),
                                     subjectsData.substr(colonPos + 1));
            }
          }

          // Load attendance if exists
          int attendanceTokenIndex = 12; // Default for old format
          if (tokens.size() >= 14) {
            // New format: attendance is at token 14
            attendanceTokenIndex = 14;
          }

          if (tokens.size() > attendanceTokenIndex &&
              !tokens[attendanceTokenIndex].empty()) {
            string attData = tokens[attendanceTokenIndex];
            while ((pos = attData.find(',')) != string::npos) {
              string attPair = attData.substr(0, pos);
              size_t colonPos = attPair.find(':');
              if (colonPos != string::npos) {
                string dateKey = attPair.substr(0, colonPos);
                bool present = (attPair.substr(colonPos + 1) == "P");
                newStudent->attendance.attendanceMap[dateKey] = present;
              }
              attData.erase(0, pos + 1);
            }
          }

          if (head == nullptr) {
            head = newStudent;
          } else {
            Student *temp = head;
            while (temp->next != nullptr) {
              temp = temp->next;
            }
            temp->next = newStudent;
          }

          addToHashTable(newStudent);
          totalStudents++;
          loadedCount++;
        }
      }
    }

    file.close();
    cout << "[SUCCESS] " << loadedCount << " students loaded!\n";
  }

  ~StudentManager() {
    Student *current = head;
    while (current != nullptr) {
      Student *next = current->next;
      delete current;
      current = next;
    }
  }
};

void displayMainMenu() {
  cout << "\n+------------------------------------------------------+\n";
  cout << "|       STUDENT RECORD MANAGEMENT SYSTEM              |\n";
  cout << "+------------------------------------------------------+\n";
  cout << "| 1. Add New Student                                  |\n";
  cout << "| 2. Update Student Marks                             |\n";
  cout << "| 3. Mark Attendance                                  |\n";
  cout << "| 4. Delete Student                                   |\n";
  cout << "| 5. Search Student                                   |\n";
  cout << "| 6. Display All Students                             |\n";
  cout << "| 7. Promote All Students (New Year)                  |\n";
  cout << "| 8. Update Terms                                     |\n";
  cout << "| 9. Save to File                                     |\n";
  cout << "| 10. Load from File                                  |\n";
  cout << "| 11. Manage Subjects                                 |\n";
  cout << "| 0. Exit                                             |\n";
  cout << "+------------------------------------------------------+\n";
  cout << "Enter choice: ";
}

int main() {
  StudentManager manager;
  int choice;

  cout << "\n+------------------------------------------------------+\n";
  cout << "|       WELCOME TO STUDENT RECORD MANAGEMENT SYSTEM   |\n";
  cout << "+------------------------------------------------------+\n";

  do {
    displayMainMenu();
    cin >> choice;
    cin.ignore();

    switch (choice) {
    case 1:
      manager.addStudent();
      break;
    case 2: {
      string roll;
      cout << "Enter Roll Number to update marks: ";
      getline(cin, roll);
      manager.updateMarks(roll);
      break;
    }
    case 3: {
      string roll;
      cout << "Enter Roll Number for attendance: ";
      getline(cin, roll);
      manager.markAttendance(roll);
      break;
    }
    case 4: {
      string roll;
      cout << "Enter Roll Number to delete: ";
      getline(cin, roll);
      manager.deleteStudent(roll);
      break;
    }
    case 5: {
      string roll;
      cout << "Enter Roll Number to search: ";
      getline(cin, roll);
      manager.searchStudent(roll);
      break;
    }
    case 6:
      manager.displayAll();
      break;
    case 7:
      manager.promoteAllStudents();
      break;
    case 8: {
      string roll;
      cout << "Enter Roll Number to update terms: ";
      getline(cin, roll);
      manager.updateTerms(roll);
      break;
    }
    case 9:
      manager.saveToFile("students.txt");
      break;
    case 10:
      manager.loadFromFile("students.txt");
      break;
    case 11: {
      string roll;
      cout << "Enter Roll Number to manage subjects: ";
      getline(cin, roll);
      manager.manageSubjects(roll);
      break;
    }
    case 0:
      cout << "\nThank you for using Student Record Management System!\n";
      break;
    default:
      cout << "[ERROR] Invalid choice! Please enter 0-11\n";
    }

    if (choice != 0) {
      cout << "\nPress Enter to continue...";
      cin.get();
    }

  } while (choice != 0);

  return 0;
}