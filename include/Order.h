#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <iostream>
#include "OpticalMaterial.h"
#include "Supplier.h"

struct OrderItem {
    OpticalMaterial material;
    int quantity;
    
    OrderItem(const OpticalMaterial& mat, int qty) 
        : material(mat), quantity(qty) {}
};

class Order {
private:
    std::string orderId;
    std::string supplierName;
    std::string supplierBulstat;
    std::vector<OrderItem> items;
    double totalPrice;
    std::string orderDate;

    void calculateTotalPrice();
    void validateQuantity(int quantity) const;
    std::string generateOrderId() const;
    std::string getCurrentDate() const;

public:
    Order();
    
    Order(const Supplier& supplier);
    
    Order(const Order& other);
    
    ~Order();

    std::string getOrderId() const;
    std::string getSupplierName() const;
    std::string getSupplierBulstat() const;
    double getTotalPrice() const;
    std::string getOrderDate() const;
    int getItemCount() const;

    void addItem(const OpticalMaterial& material, int quantity);
    void removeItem(int index);
    void clearOrder();
    bool isEmpty() const;
    void displayOrder() const;

    friend std::ostream& operator<<(std::ostream& os, const Order& order);
    
    void saveToFile(std::ostream& os) const;
    void loadFromFile(std::istream& is);
};

#endif

