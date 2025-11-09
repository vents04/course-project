#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <string>
#include <vector>
#include <iostream>
#include "OpticalMaterial.h"

class Supplier {
private:
    std::string bulstat;
    std::string name;
    std::string location;
    std::string phoneNumber;
    std::vector<OpticalMaterial> materials;

    void validateBulstat(const std::string& bulstat) const;
    void validatePhoneNumber(const std::string& phone) const;

public:
    Supplier();
    
    Supplier(const std::string& bulstat, const std::string& name, 
            const std::string& location, const std::string& phoneNumber);
    
    Supplier(const Supplier& other);
    
    ~Supplier();

    std::string getBulstat() const;
    std::string getName() const;
    std::string getLocation() const;
    std::string getPhoneNumber() const;
    const std::vector<OpticalMaterial>& getMaterials() const;

    void setBulstat(const std::string& bulstat);
    void setName(const std::string& name);
    void setLocation(const std::string& location);
    void setPhoneNumber(const std::string& phoneNumber);
    
    void addMaterial(const OpticalMaterial& material);
    void removeMaterial(int index);
    void displayMaterials() const;
    int getMaterialCount() const;
    OpticalMaterial getMaterial(int index) const;

    friend std::ostream& operator<<(std::ostream& os, const Supplier& supplier);
    friend std::istream& operator>>(std::istream& is, Supplier& supplier);
    
    void saveToFile(std::ostream& os) const;
    void loadFromFile(std::istream& is);
};

#endif

