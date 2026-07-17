# 🗄️ NITCBase — Relational Database Management System

A relational database management system built from scratch in **C++**, implementing core DBMS concepts such as storage management, buffer management, B+ Tree indexing, schema management, and relational algebra.

---

## 📖 Overview

NITCBase is an educational DBMS developed as part of the Database Systems Laboratory. The project demonstrates the internal architecture of a relational database by implementing every major component from scratch instead of relying on existing database libraries.

The implementation follows a layered design, where each module is responsible for a specific functionality such as storage, caching, indexing, or query execution.

---

## ✨ Features

- 📁 Disk-based storage manager
- ⚡ Buffer manager for efficient block caching
- 🌳 B+ Tree indexing
- 🗂️ Relation and Attribute Catalogs
- 📋 Schema management
- 🔍 Relational algebra operations
  - Select
  - Project
  - Join
  - Insert
- 💻 Interactive command-line interface
- 📄 Batch execution using input files

---

## 🏗️ Project Structure

```
NITCbase
│
├── mynitcbase/
│   ├── Algebra/
│   ├── BPlusTree/
│   ├── BlockAccess/
│   ├── Buffer/
│   ├── Cache/
│   ├── Disk_Class/
│   ├── Frontend/
│   ├── FrontendInterface/
│   ├── Schema/
│   ├── define/
│   ├── Makefile
│   └── main.cpp
│
├── XFS_Interface/
├── Disk/
└── Files/
    ├── Input_Files/
    ├── Output_Files/
    └── Batch_Execution_Files/
```

---

## 🛠️ Technologies Used

- C++
- Make
- File-based Storage
- B+ Tree
- Object-Oriented Programming

---

## 🚀 Getting Started

### Clone the repository

```bash
git clone https://github.com/<your-username>/NITCbase.git
```

### Build

```bash
cd NITCbase/mynitcbase
make
```

### Run

```bash
./nitcbase
```

---

## 💻 Example Commands

```text
CREATE TABLE Students (RollNo NUM, Name STR, CGPA NUM);

OPEN TABLE Students;

INSERT INTO Students VALUES (1, Alice, 9.5);

SELECT * FROM Students INTO Result WHERE CGPA >= 9.0;

CREATE INDEX ON Students.RollNo;

JOIN Students Courses INTO StudentCourses
ON Students.RollNo = Courses.RollNo;
```

---

## 📚 Learning Outcomes

Through this project I gained practical experience with:

- Database storage architecture
- Buffer management
- Block access mechanisms
- B+ Tree indexing
- Schema management
- Query processing
- Relational algebra
- File organization inside a DBMS

---

## 📄 License

This project was developed for educational purposes as part of the Database Systems Laboratory coursework at NIT Calicut.

---

## 👨‍💻 Author

**M. Sujith**

B.Tech Computer Science and Engineering

National Institute of Technology Calicut
