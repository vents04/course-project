#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <iomanip>
#include <cfloat>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
#include "OpticalMaterial.h"
#include "Supplier.h"
#include "Order.h"

// Function prototypes
void displayMainMenu();
void addSupplier(std::vector<Supplier>& suppliers);
void addMaterialToSupplier(std::vector<Supplier>& suppliers);
void displayAllSuppliers(const std::vector<Supplier>& suppliers);
void displaySupplierDetails(const std::vector<Supplier>& suppliers);
void createOrder(const std::vector<Supplier>& suppliers, std::vector<Order>& orders);
void displayAllOrders(const std::vector<Order>& orders);
void saveDataToFile(const std::vector<Supplier>& suppliers, const std::vector<Order>& orders);
void loadDataFromFile(std::vector<Supplier>& suppliers, std::vector<Order>& orders);
int selectSupplier(const std::vector<Supplier>& suppliers);
bool bulstatExists(const std::vector<Supplier>& suppliers, const std::string& bulstat);
bool phoneNumberExists(const std::vector<Supplier>& suppliers, const std::string& phoneNumber);
void clearScreen();
void pauseScreen();
int getValidatedInt(const std::string& prompt, int min = INT_MIN, int max = INT_MAX);
double getValidatedDouble(const std::string& prompt, double min = -DBL_MAX, double max = DBL_MAX);

int main() {
    try {
        std::vector<Supplier> suppliers;
        std::vector<Order> orders;
        
        loadDataFromFile(suppliers, orders);
        
        int choice;
        bool running = true;
        
        std::cout << "\n";
        std::cout << std::string(65, '=') << std::endl;
        std::cout << "        Optical Materials & Suppliers Management System        " << std::endl;
        std::cout << std::string(65, '=') << std::endl;
        
        while (running) {
            displayMainMenu();
            choice = getValidatedInt("Enter choice: ", 0, 8);
            
            try {
                switch (choice) {
                    case 1:
                        addSupplier(suppliers);
                        break;
                    case 2:
                        addMaterialToSupplier(suppliers);
                        break;
                    case 3:
                        displayAllSuppliers(suppliers);
                        break;
                    case 4:
                        displaySupplierDetails(suppliers);
                        break;
                    case 5:
                        createOrder(suppliers, orders);
                        break;
                    case 6:
                        displayAllOrders(orders);
                        break;
                    case 7:
                        saveDataToFile(suppliers, orders);
                        break;
                    case 8:
                        loadDataFromFile(suppliers, orders);
                        break;
                    case 0:
                        std::cout << "\nSaving data...\n";
                        saveDataToFile(suppliers, orders);
                        std::cout << "Thank you for using the system!\n";
                        running = false;
                        break;
                    default:
                        std::cout << "Invalid option!\n";
                }
            } catch (const std::exception& e) {
                std::cerr << "[ERROR] Error: " << e.what() << std::endl;
                pauseScreen();
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[FATAL ERROR] " << e.what() << std::endl;
        std::cout << "\nPress Enter to exit...";
        std::cin.ignore();
        std::cin.get();
        return 1;
    } catch (...) {
        std::cerr << "[FATAL ERROR] Unknown error occurred!" << std::endl;
        std::cout << "\nPress Enter to exit...";
        std::cin.ignore();
        std::cin.get();
        return 1;
    }
    
    return 0;
}

void displayMainMenu() {
    std::cout << "\n" << std::string(65, '=') << std::endl;
    std::cout << "                            MAIN MENU                            " << std::endl;
    std::cout << std::string(65, '=') << std::endl;
    std::cout << "1. Add Supplier" << std::endl;
    std::cout << "2. Add Material to Supplier" << std::endl;
    std::cout << "3. Display All Suppliers" << std::endl;
    std::cout << "4. Display Supplier Details" << std::endl;
    std::cout << "5. Create Order" << std::endl;
    std::cout << "6. Display All Orders" << std::endl;
    std::cout << "7. Save Data to File" << std::endl;
    std::cout << "8. Load Data from File" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << std::string(65, '=') << std::endl;
}

void addSupplier(std::vector<Supplier>& suppliers) {
    clearScreen();
    std::cout << "\n=== ADD SUPPLIER ===\n\n";
    
    try {
        Supplier supplier;
        std::cin >> supplier;
        
        if (bulstatExists(suppliers, supplier.getBulstat())) {
            throw std::invalid_argument("A supplier with this BULSTAT already exists!");
        }
        
        if (phoneNumberExists(suppliers, supplier.getPhoneNumber())) {
            throw std::invalid_argument("A supplier with this phone number already exists!");
        }
        
        suppliers.push_back(supplier);
        std::cout << "\n[OK] Supplier added successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Error adding supplier: " 
                  << e.what() << std::endl;
    }
    
    pauseScreen();
}

void addMaterialToSupplier(std::vector<Supplier>& suppliers) {
    clearScreen();
    
    if (suppliers.empty()) {
        std::cout << "\n[ERROR] No suppliers available! Add a supplier first.\n";
        pauseScreen();
        return;
    }
    
    std::cout << "\n=== ADD MATERIAL ===\n\n";
    
    int supplierIndex = selectSupplier(suppliers);
    if (supplierIndex == -1) {
        return;
    }
    
    try {
        OpticalMaterial material;
        std::cin >> material;
        suppliers[supplierIndex].addMaterial(material);
        std::cout << "\n[OK] Material added successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Error adding material: " 
                  << e.what() << std::endl;
    }
    
    pauseScreen();
}

void displayAllSuppliers(const std::vector<Supplier>& suppliers) {
    clearScreen();
    
    if (suppliers.empty()) {
        std::cout << "\n[ERROR] No suppliers available!\n";
        pauseScreen();
        return;
    }
    
    std::cout << "\n=== ALL SUPPLIERS ===\n";
    for (size_t i = 0; i < suppliers.size(); ++i) {
        std::cout << "\n[" << (i + 1) << "] ";
        std::cout << suppliers[i];
    }
    
    pauseScreen();
}

void displaySupplierDetails(const std::vector<Supplier>& suppliers) {
    clearScreen();
    
    if (suppliers.empty()) {
        std::cout << "\n[ERROR] No suppliers available!\n";
        pauseScreen();
        return;
    }
    
    int supplierIndex = selectSupplier(suppliers);
    if (supplierIndex == -1) {
        return;
    }
    
    std::cout << suppliers[supplierIndex];
    suppliers[supplierIndex].displayMaterials();
    
    pauseScreen();
}

void createOrder(const std::vector<Supplier>& suppliers, std::vector<Order>& orders) {
    clearScreen();
    
    if (suppliers.empty()) {
        std::cout << "\n[ERROR] No suppliers available!\n";
        pauseScreen();
        return;
    }
    
    std::cout << "\n=== CREATE ORDER ===\n\n";
    
    int supplierIndex = selectSupplier(suppliers);
    if (supplierIndex == -1) {
        return;
    }
    
    const Supplier& selectedSupplier = suppliers[supplierIndex];
    
    if (selectedSupplier.getMaterialCount() == 0) {
        std::cout << "\n[ERROR] This supplier has no available materials!\n";
        pauseScreen();
        return;
    }
    
    Order order(selectedSupplier);
    bool addingItems = true;
    
    while (addingItems) {
        clearScreen();
        std::cout << "\n=== CURRENT ORDER ===\n";
        order.displayOrder();
        
        std::cout << "\nAvailable materials from " << selectedSupplier.getName() << ":\n";
        selectedSupplier.displayMaterials();
        
        std::cout << "\nOptions:\n";
        std::cout << "1-" << selectedSupplier.getMaterialCount() 
                  << ": Add material\n";
        std::cout << "0: Finish order\n";
        
        int choice = getValidatedInt("\nChoice: ", 0, selectedSupplier.getMaterialCount());
        
        if (choice == 0) {
            if (!order.isEmpty()) {
                orders.push_back(order);
                std::cout << "\n[OK] Order created successfully!\n";
                std::cout << "Total: " << std::fixed << std::setprecision(2) 
                          << order.getTotalPrice() << " BGN\n";
            } else {
                std::cout << "\n[ERROR] Order is empty and will not be saved.\n";
            }
            addingItems = false;
        } else {
            try {
                OpticalMaterial material = selectedSupplier.getMaterial(choice - 1);
                int quantity = getValidatedInt("Quantity: ", 1, 10000);
                order.addItem(material, quantity);
                std::cout << "[OK] Material added to order!\n";
                pauseScreen();
            } catch (const std::exception& e) {
                std::cerr << "[ERROR] Error: " << e.what() << std::endl;
                pauseScreen();
            }
        }
    }
    
    pauseScreen();
}

void displayAllOrders(const std::vector<Order>& orders) {
    clearScreen();
    
    if (orders.empty()) {
        std::cout << "\n[ERROR] No orders available!\n";
        pauseScreen();
        return;
    }
    
    std::cout << "\n=== ALL ORDERS ===\n";
    for (size_t i = 0; i < orders.size(); ++i) {
        std::cout << "\n[Order " << (i + 1) << "]";
        std::cout << orders[i];
    }
    
    pauseScreen();
}

void saveDataToFile(const std::vector<Supplier>& suppliers, const std::vector<Order>& orders) {
    try {
        // Save suppliers
        std::ofstream suppliersFile("suppliers.dat");
        if (!suppliersFile) {
            throw std::runtime_error("Cannot open suppliers file");
        }
        
        suppliersFile << suppliers.size() << "\n";
        for (const auto& supplier : suppliers) {
            supplier.saveToFile(suppliersFile);
        }
        suppliersFile.close();
        
        // Save orders
        std::ofstream ordersFile("orders.dat");
        if (!ordersFile) {
            throw std::runtime_error("Cannot open orders file");
        }
        
        ordersFile << orders.size() << "\n";
        for (const auto& order : orders) {
            order.saveToFile(ordersFile);
        }
        ordersFile.close();
        
        std::cout << "\n[OK] Data saved successfully!\n";
        std::cout << "  Suppliers: " << suppliers.size() << "\n";
        std::cout << "  Orders: " << orders.size() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Error saving: " << e.what() << std::endl;
    }
    
    pauseScreen();
}

void loadDataFromFile(std::vector<Supplier>& suppliers, std::vector<Order>& orders) {
    try {
        // Load suppliers
        std::ifstream suppliersFile("suppliers.dat");
        if (suppliersFile) {
            size_t supplierCount;
            suppliersFile >> supplierCount;
            suppliersFile.ignore();
            
            suppliers.clear();
            int duplicateCount = 0;
            for (size_t i = 0; i < supplierCount; ++i) {
                Supplier supplier;
                supplier.loadFromFile(suppliersFile);
                
                if (bulstatExists(suppliers, supplier.getBulstat())) {
                    duplicateCount++;
                    std::cerr << "Warning: Skipping duplicate supplier with BULSTAT: " 
                              << supplier.getBulstat() << std::endl;
                    continue;
                }
                
                if (phoneNumberExists(suppliers, supplier.getPhoneNumber())) {
                    duplicateCount++;
                    std::cerr << "Warning: Skipping duplicate supplier with phone number: " 
                              << supplier.getPhoneNumber() << std::endl;
                    continue;
                }
                
                suppliers.push_back(supplier);
            }
            suppliersFile.close();
            
            if (duplicateCount > 0) {
                std::cout << "\n⚠ Skipped " << duplicateCount 
                          << " duplicate supplier(s) during load.\n";
            }
        }
        
        // Load orders
        std::ifstream ordersFile("orders.dat");
        if (ordersFile) {
            size_t orderCount;
            ordersFile >> orderCount;
            ordersFile.ignore();
            
            orders.clear();
            for (size_t i = 0; i < orderCount; ++i) {
                Order order;
                order.loadFromFile(ordersFile);
                orders.push_back(order);
            }
            ordersFile.close();
            
            std::cout << "\n[OK] Data loaded successfully!\n";
            std::cout << "  Suppliers: " << suppliers.size() << "\n";
            std::cout << "  Orders: " << orders.size() << "\n";
            pauseScreen();
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Error loading: " << e.what() << std::endl;
        pauseScreen();
    }
}

int selectSupplier(const std::vector<Supplier>& suppliers) {
    std::cout << "\nAvailable suppliers:\n";
    std::cout << std::string(65, '-') << std::endl;
    
    for (size_t i = 0; i < suppliers.size(); ++i) {
        std::cout << "[" << (i + 1) << "] " << suppliers[i].getName() 
                  << " - " << suppliers[i].getLocation() << std::endl;
    }
    
    std::cout << std::string(65, '-') << std::endl;
    int choice = getValidatedInt("Select supplier: ", 1, 
                                  static_cast<int>(suppliers.size()));
    
    return choice - 1;
}

bool bulstatExists(const std::vector<Supplier>& suppliers, const std::string& bulstat) {
    for (const auto& supplier : suppliers) {
        if (supplier.getBulstat() == bulstat) {
            return true;
        }
    }
    return false;
}

bool phoneNumberExists(const std::vector<Supplier>& suppliers, const std::string& phoneNumber) {
    for (const auto& supplier : suppliers) {
        if (supplier.getPhoneNumber() == phoneNumber) {
            return true;
        }
    }
    return false;
}

void clearScreen() {
#ifdef _WIN32
    // Windows-specific screen clear
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole != INVALID_HANDLE_VALUE) {
        COORD coordScreen = {0, 0};
        DWORD cCharsWritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        
        if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
            FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);
            FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
            SetConsoleCursorPosition(hConsole, coordScreen);
        }
    }
#else
    // Unix/Linux/Mac: Use ANSI escape codes
    std::cout << "\033[2J\033[1;1H";
#endif
}

void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    // Wait for Enter press (getline handles any existing newline or waits for input)
    std::string dummy;
    std::getline(std::cin, dummy);
}

int getValidatedInt(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERROR] Invalid input! Please enter a number.\n";
        } else if (value < min || value > max) {
            std::cout << "[ERROR] Number must be between " << min << " and " << max << ".\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

double getValidatedDouble(const std::string& prompt, double min, double max) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERROR] Invalid input! Please enter a number.\n";
        } else if (value < min || value > max) {
            std::cout << "[ERROR] Number must be between " << min << " and " << max << ".\n";
        } else {
            return value;
        }
    }
}

