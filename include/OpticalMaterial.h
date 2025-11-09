#ifndef OPTICAL_MATERIAL_H
#define OPTICAL_MATERIAL_H

#include <string>
#include <iostream>

class OpticalMaterial {
private:
    std::string type;
    double thickness;
    double diopter;
    std::string materialName;
    double price;

    void validateThickness(double t) const;
    void validatePrice(double p) const;

public:
    OpticalMaterial();
    
    OpticalMaterial(const std::string& type, double thickness, double diopter, 
                   const std::string& materialName, double price);
    
    OpticalMaterial(const OpticalMaterial& other);
    
    ~OpticalMaterial();

    std::string getType() const;
    double getThickness() const;
    double getDiopter() const;
    std::string getMaterialName() const;
    double getPrice() const;

    void setType(const std::string& type);
    void setThickness(double thickness);
    void setDiopter(double diopter);
    void setMaterialName(const std::string& materialName);
    void setPrice(double price);

    friend std::ostream& operator<<(std::ostream& os, const OpticalMaterial& material);
    
    friend std::istream& operator>>(std::istream& is, OpticalMaterial& material);
    
    void saveToFile(std::ostream& os) const;
    void loadFromFile(std::istream& is);
};

#endif

