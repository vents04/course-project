#include "OpticalMaterial.h"
#include <stdexcept>
#include <iomanip>

void OpticalMaterial::validateThickness(double t) const {
    if (t <= 0) {
        throw std::invalid_argument("Thickness must be positive");
    }
}

void OpticalMaterial::validatePrice(double p) const {
    if (p < 0) {
        throw std::invalid_argument("Price cannot be negative");
    }
}

OpticalMaterial::OpticalMaterial() 
    : type("Unknown"), thickness(1.0), diopter(0.0), materialName("Unknown"), price(0.0) {
}

OpticalMaterial::OpticalMaterial(const std::string& type, double thickness, double diopter, 
                               const std::string& materialName, double price)
    : type(type), diopter(diopter), materialName(materialName) {
    validateThickness(thickness);
    validatePrice(price);
    this->thickness = thickness;
    this->price = price;
}

OpticalMaterial::OpticalMaterial(const OpticalMaterial& other)
    : type(other.type), thickness(other.thickness), diopter(other.diopter),
      materialName(other.materialName), price(other.price) {
}

OpticalMaterial::~OpticalMaterial() {}

std::string OpticalMaterial::getType() const {
    return type;
}

double OpticalMaterial::getThickness() const {
    return thickness;
}

double OpticalMaterial::getDiopter() const {
    return diopter;
}

std::string OpticalMaterial::getMaterialName() const {
    return materialName;
}

double OpticalMaterial::getPrice() const {
    return price;
}

void OpticalMaterial::setType(const std::string& type) {
    if (type.empty()) {
        throw std::invalid_argument("Type cannot be empty");
    }
    this->type = type;
}

void OpticalMaterial::setThickness(double thickness) {
    validateThickness(thickness);
    this->thickness = thickness;
}

void OpticalMaterial::setDiopter(double diopter) {
    this->diopter = diopter;
}

void OpticalMaterial::setMaterialName(const std::string& materialName) {
    if (materialName.empty()) {
        throw std::invalid_argument("Material name cannot be empty");
    }
    this->materialName = materialName;
}

void OpticalMaterial::setPrice(double price) {
    validatePrice(price);
    this->price = price;
}

std::ostream& operator<<(std::ostream& os, const OpticalMaterial& material) {
    os << "Type: " << material.type 
       << ", Thickness: " << std::fixed << std::setprecision(2) << material.thickness << "mm"
       << ", Diopter: " << material.diopter 
       << ", Material: " << material.materialName 
       << ", Price: " << material.price << " BGN";
    return os;
}

std::istream& operator>>(std::istream& is, OpticalMaterial& material) {
    std::string type, materialName;
    double thickness, diopter, price;
    
    std::cout << "Enter type: ";
    is.ignore();
    std::getline(is, type);
    
    std::cout << "Enter thickness (mm): ";
    is >> thickness;
    
    std::cout << "Enter diopter: ";
    is >> diopter;
    
    std::cout << "Enter material name: ";
    is.ignore();
    std::getline(is, materialName);
    
    std::cout << "Enter price (BGN): ";
    is >> price;
    
    material.setType(type);
    material.setThickness(thickness);
    material.setDiopter(diopter);
    material.setMaterialName(materialName);
    material.setPrice(price);
    
    return is;
}

void OpticalMaterial::saveToFile(std::ostream& os) const {
    os << type << "\n"
       << thickness << "\n"
       << diopter << "\n"
       << materialName << "\n"
       << price << "\n";
}

void OpticalMaterial::loadFromFile(std::istream& is) {
    std::getline(is, type);
    is >> thickness;
    is >> diopter;
    is.ignore();
    std::getline(is, materialName);
    is >> price;
    is.ignore();
}

