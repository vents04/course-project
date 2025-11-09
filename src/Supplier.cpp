#include "Supplier.h"
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <cctype>

void Supplier::validateBulstat(const std::string& bulstat) const {
    if (bulstat.empty()) {
        throw std::invalid_argument("Bulstat cannot be empty");
    }

    if (!std::all_of(bulstat.begin(), bulstat.end(), ::isdigit)) {
        throw std::invalid_argument("Bulstat must contain only digits");
    }

    if (bulstat.length() != 9 && bulstat.length() != 13) {
        throw std::invalid_argument("Bulstat must be 9 or 13 digits");
    }
}

void Supplier::validatePhoneNumber(const std::string& phone) const {
    if (phone.empty()) {
        throw std::invalid_argument("Phone number cannot be empty");
    }

    // Count digits and validate characters
    int digitCount = 0;
    bool startsWithPlus = false;
    
    for (size_t i = 0; i < phone.length(); ++i) {
        char c = phone[i];
        
        if (::isdigit(c)) {
            digitCount++;
        } else if (c == '+') {
            if (i == 0) {
                startsWithPlus = true;
            } else {
                throw std::invalid_argument("Phone number: '+' can only appear at the start");
            }
        } else if (c != '-' && c != ' ' && c != '(' && c != ')' && c != '.') {
            throw std::invalid_argument("Phone number contains invalid characters");
        }
    }

    // Validate digit count (E.164 standard: 7-15 digits)
    if (digitCount < 7) {
        throw std::invalid_argument("Phone number must contain at least 7 digits");
    }
    if (digitCount > 15) {
        throw std::invalid_argument("Phone number cannot contain more than 15 digits");
    }

    // If starts with +, ensure it's followed by at least one digit
    if (startsWithPlus && digitCount == 0) {
        throw std::invalid_argument("Phone number starting with '+' must contain digits");
    }

    // Ensure phone number doesn't consist only of formatting characters
    if (digitCount == 0) {
        throw std::invalid_argument("Phone number must contain at least one digit");
    }
}

Supplier::Supplier() 
    : bulstat("000000000"), name("Unknown"), location("Unknown"), phoneNumber("0000000000") {
}

Supplier::Supplier(const std::string& bulstat, const std::string& name, 
                  const std::string& location, const std::string& phoneNumber)
    : name(name), location(location) {
    validateBulstat(bulstat);
    validatePhoneNumber(phoneNumber);
    this->bulstat = bulstat;
    this->phoneNumber = phoneNumber;
}

Supplier::Supplier(const Supplier& other)
    : bulstat(other.bulstat), name(other.name), location(other.location),
      phoneNumber(other.phoneNumber), materials(other.materials) {
}

Supplier::~Supplier() {
    materials.clear();
}

std::string Supplier::getBulstat() const {
    return bulstat;
}

std::string Supplier::getName() const {
    return name;
}

std::string Supplier::getLocation() const {
    return location;
}

std::string Supplier::getPhoneNumber() const {
    return phoneNumber;
}

const std::vector<OpticalMaterial>& Supplier::getMaterials() const {
    return materials;
}

void Supplier::setBulstat(const std::string& bulstat) {
    validateBulstat(bulstat);
    this->bulstat = bulstat;
}

void Supplier::setName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Name cannot be empty");
    }
    this->name = name;
}

void Supplier::setLocation(const std::string& location) {
    if (location.empty()) {
        throw std::invalid_argument("Location cannot be empty");
    }
    this->location = location;
}

void Supplier::setPhoneNumber(const std::string& phoneNumber) {
    validatePhoneNumber(phoneNumber);
    this->phoneNumber = phoneNumber;
}

void Supplier::addMaterial(const OpticalMaterial& material) {
    materials.push_back(material);
}

void Supplier::removeMaterial(int index) {
    if (index < 0 || index >= static_cast<int>(materials.size())) {
        throw std::out_of_range("Invalid material index");
    }
    materials.erase(materials.begin() + index);
}

void Supplier::displayMaterials() const {
    if (materials.empty()) {
        std::cout << "No materials available from this supplier." << std::endl;
        return;
    }
    
    std::cout << "\nAvailable materials:" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    for (size_t i = 0; i < materials.size(); ++i) {
        std::cout << "[" << (i + 1) << "] " << materials[i] << std::endl;
    }
    std::cout << std::string(80, '-') << std::endl;
}

int Supplier::getMaterialCount() const {
    return static_cast<int>(materials.size());
}

OpticalMaterial Supplier::getMaterial(int index) const {
    if (index < 0 || index >= static_cast<int>(materials.size())) {
        throw std::out_of_range("Invalid material index");
    }
    return materials[index];
}

std::ostream& operator<<(std::ostream& os, const Supplier& supplier) {
    os << "\n" << std::string(80, '=') << std::endl;
    os << "Supplier Information:" << std::endl;
    os << std::string(80, '=') << std::endl;
    os << "Bulstat: " << supplier.bulstat << std::endl;
    os << "Name: " << supplier.name << std::endl;
    os << "Location: " << supplier.location << std::endl;
    os << "Phone: " << supplier.phoneNumber << std::endl;
    os << "Number of materials: " << supplier.materials.size() << std::endl;
    os << std::string(80, '=') << std::endl;
    return os;
}

std::istream& operator>>(std::istream& is, Supplier& supplier) {
    std::string bulstat, name, location, phoneNumber;
    
    std::cout << "Enter Bulstat (9 or 13 digits): ";
    is >> bulstat;
    
    std::cout << "Enter company name: ";
    is.ignore();
    std::getline(is, name);
    
    std::cout << "Enter location: ";
    std::getline(is, location);
    
    std::cout << "Enter phone number: ";
    std::getline(is, phoneNumber);
    
    supplier.setBulstat(bulstat);
    supplier.setName(name);
    supplier.setLocation(location);
    supplier.setPhoneNumber(phoneNumber);
    
    return is;
}

void Supplier::saveToFile(std::ostream& os) const {
    os << bulstat << "\n"
       << name << "\n"
       << location << "\n"
       << phoneNumber << "\n"
       << materials.size() << "\n";
    
    for (const auto& material : materials) {
        material.saveToFile(os);
    }
}

void Supplier::loadFromFile(std::istream& is) {
    std::getline(is, bulstat);
    std::getline(is, name);
    std::getline(is, location);
    std::getline(is, phoneNumber);
    
    size_t materialCount;
    is >> materialCount;
    is.ignore();
    
    materials.clear();
    for (size_t i = 0; i < materialCount; ++i) {
        OpticalMaterial material;
        material.loadFromFile(is);
        materials.push_back(material);
    }
}

