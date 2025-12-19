# Student Record Management System

## 📌 Overview
A C++ console-based application for managing student records with attendance, marks, promotion, and file persistence.

## ✨ Features

### 📊 Student Management
- Add, delete, search, and display student records
- Store student details: roll number, name, class, CNIC/B-Form, address
- Track admission year and current academic year

### 📝 Academic Tracking
- Subject management with custom lists per class/category
- Marks entry with validation (0-100 or A/L/- for absent/leave/not entered)
- Automatic grade calculation (A-F based on percentage)
- Term completion tracking (0-3 terms)
- Board marks flag for classes 8-12

### 📅 Attendance System
- Calendar-based attendance for 2024-2034
- Mark present/absent for specific dates
- View attendance summary (total/present/absent days)

### 🎓 Promotion System
- Automatic promotion to next class (1-11)
- Requires 3 completed terms and board marks (classes 8-12)
- Validates promotion timing based on academic calendar
- Clears old marks after promotion

### 💾 Data Persistence
- Save/load records to "students.txt"
- File format supports subjects and attendance
- Backward compatibility with old file formats

### 🧠 Data Structures
- Linked list for main student records
- Hash table for O(1) student lookup by roll number
- Linked list for subject lists per student
- Map for calendar attendance tracking

## 🏗️ Class Structure

### `Calendar`
- Manages date-based attendance
- Handles leap years and month-day validation
- Tracks present/absent status

### `Subject`
- Stores subject name and marks status
- Linked list node structure

### `Student`
- Core student data container
- Manages subject list and attendance
- Calculates grades and percentages
- Handles promotion logic

### `StudentManager`
- Main system controller
- Implements all operations (add, delete, search, etc.)
- Manages file I/O and hash table

## 🚀 Getting Started

### Prerequisites
- C++ compiler (GCC, Clang, or MSVC)
- Standard Library support

### Compilation
```bash
g++ -std=c++11 -o student_system Student_Record_Management_System.cpp
```

### Execution
```bash
./student_system
```

## 📋 Usage

### Main Menu Options
1. **Add New Student** - Enter all student details with validation
2. **Update Marks** - Enter marks for each subject
3. **Mark Attendance** - Record daily attendance
4. **Delete Student** - Remove student by roll number
5. **Search Student** - Find and display student details
6. **Display All** - Show all student records
7. **Promote All** - Auto-promote eligible students
8. **Update Terms** - Set terms completed (0-3)
9. **Save to File** - Export data to students.txt
10. **Load from File** - Import data from students.txt
11. **Manage Subjects** - View subject list (auto-managed)
0. **Exit** - Close the application

### Class Categories
- **Classes 1-7**: Custom subjects (minimum 1)
- **Classes 8-10**: Computer Science or Biology
- **Classes 11-12**: Computer Science, Pre-Engineering, or Pre-Medical

### Marks Entry
- Enter numeric marks (0-100) or:
  - `A` for Absent
  - `L` for Leave  
  - `-` for Not Entered
- Classes 8-12 have 75 max marks for science subjects

## 📁 File Format
```
roll|name|father|class|category|admissionYear|currentYear|dob|cnic|address|totalSubjects|terms|boardMarks|subjects|attendance
```
- Subjects: `subject1:marks1,subject2:marks2`
- Attendance: `YYYY-MM-DD:P/A,YYYY-MM-DD:P/A`

## ⚠️ Validation Rules
- Roll numbers must be unique
- CNIC/B-Form must be 13 digits
- Date of birth: DDMMYYYY format
- Admission year: 1900-2100 range
- Marks: 0-maxMarks or A/L/-
- Terms: 0-3 only

## 🔧 Technical Details
- **Language**: C++ (C++11 standard)
- **Memory**: Manual memory management with proper cleanup
- **Search**: O(1) average via hash table
- **Storage**: Linked lists for dynamic data
- **Persistence**: Text-based file storage

## 📈 Future Enhancements
Potential improvements:
- GUI interface
- Database integration
- Report generation
- User authentication
- Network functionality

## 📄 License
Open-source educational project. Modify and distribute freely.
