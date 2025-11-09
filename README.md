# Optical Materials & Suppliers Management System

## Description

This is a course project for "Programming Languages" - a console application for managing information about optical materials suppliers. The system allows users to add suppliers, manage their materials, and create orders with automatic price calculations.

The application provides a user-friendly menu-driven console interface where you can track suppliers, their available optical materials, and process orders efficiently. All data is automatically saved and loaded, ensuring your information is always preserved.

---

## Project Structure

The project follows a standard C++ structure with separation between source code, header files, and documentation. Source files are in `src/`, headers in `include/`, and compiled object files are automatically generated in `build/` during compilation.

```
.
├── src/                    # Source files
│   ├── main.cpp
│   ├── OpticalMaterial.cpp
│   ├── Supplier.cpp
│   └── Order.cpp
├── include/                # Header files
│   ├── OpticalMaterial.h
│   ├── Supplier.h
│   └── Order.h
├── build/                  # Compiled object files (generated)
├── docs/                   # Documentation
│   ├── CLASS_DIAGRAM.txt
│   ├── PROJECT_SUMMARY.txt
│   └── DEMO_STEPS.txt
├── Makefile               # Build configuration
└── README.md              # This file
```

---

## Prerequisites

To run the project, you'll need a C++ compiler with C++11 support (g++ or clang++) and Make for compilation. The Makefile is written to work on Windows, macOS, and Linux without additional modifications.

On Windows, you'll need MinGW/MSYS2, Git Bash, or WSL to access Make commands. On macOS, you can install Xcode Command Line Tools, while on Linux, build tools are usually already installed or can be installed through the package manager.

You can verify your setup by checking if `make` and `g++` (or `clang++`) are available in your terminal.

---

## Compilation

The easiest way to compile the project is through the Makefile. Simply run `make` in the project's root directory. This will compile all source files and create the executable `optical_system`.

After compilation, you can run the program with `make run`, which will automatically compile and execute the application. If you want to clean the compiled files, use `make clean`.

---

## Usage

After starting the program, you'll see a console menu with various options. The system allows you to add suppliers with their information (bulstat, name, location, phone), add optical materials to each supplier, and create orders that automatically calculate the total price.

Main features include adding suppliers, managing materials, viewing all suppliers and their details, creating orders, and saving/loading data from files. All data is automatically saved when exiting the program and automatically loaded on the next startup.

The menu system guides you through each operation with clear prompts and validation, ensuring data integrity throughout the application.

---

## Classes

The project contains three main classes, each representing a different part of the system's business logic.

**OpticalMaterial** represents an optical material with its characteristics - type, thickness, diopter, name, and price. Each material can be validated and used in orders. The class includes validation to ensure thickness is positive and prices are non-negative.

**Supplier** contains information about a supplier, including bulstat, name, location, phone number, and a list of materials the supplier offers. The system ensures that each supplier has a unique bulstat and phone number, preventing duplicates in the database.

**Order** represents an order containing supplier information, a list of items, total price, and order date. Orders automatically calculate the total price based on material prices and their quantities. The system allows you to add multiple items to an order and automatically updates the total as items are added or removed.

---

## Validation

The system includes complete validation of all input data. The bulstat must be exactly 9 or 13 digits, phone numbers are validated according to international standards (7-15 digits with allowed formatting characters), thickness must be a positive number, and prices cannot be negative.

All text fields are checked to ensure they're not empty, and quantities in orders must be positive integers. For invalid data, the system displays clear error messages that guide users to correct their input.

---

## Data Files

Data is stored in two text files - `suppliers.dat` for suppliers and their materials, and `orders.dat` for orders. The files are automatically created on first save and automatically loaded when starting the program.

The file format is simple and human-readable, making it easy to understand the data structure. Both files are created in the same directory as the executable.

---

## Technologies

The project is written in C++11 and uses the standard library for working with vectors, strings, input-output operations, and files. Validation is performed through exception handling, ensuring secure error processing.

The code follows object-oriented principles with proper encapsulation, using private member variables and public getters/setters. Operator overloading is implemented for input and output operations, making the code more intuitive and maintainable.

---

## Notes

The program automatically saves data on exit and loads it on next startup, meaning your data is always preserved. The code is organized in a professional structure and compiles without errors or warnings.

All requirements from the assignment have been fulfilled, including multiple constructors, proper encapsulation, validation, file I/O operations, and a dynamic menu-driven interface.

---
