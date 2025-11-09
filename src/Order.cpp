#include "Order.h"
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <random>

void Order::calculateTotalPrice() {
    totalPrice = 0.0;
    for (const auto& item : items) {
        totalPrice += item.material.getPrice() * item.quantity;
    }
}

void Order::validateQuantity(int quantity) const {
    if (quantity <= 0) {
        throw std::invalid_argument("Quantity must be positive");
    }
}

std::string Order::generateOrderId() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10000, 99999);
    return "ORD" + std::to_string(dis(gen));
}

std::string Order::getCurrentDate() const {
    time_t now = time(0);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buffer);
}

Order::Order() 
    : orderId(generateOrderId()), supplierName("Unknown"), 
      supplierBulstat("000000000"), totalPrice(0.0), 
      orderDate(getCurrentDate()) {
}

Order::Order(const Supplier& supplier)
    : orderId(generateOrderId()), supplierName(supplier.getName()),
      supplierBulstat(supplier.getBulstat()), totalPrice(0.0),
      orderDate(getCurrentDate()) {
}

Order::Order(const Order& other)
    : orderId(other.orderId), supplierName(other.supplierName),
      supplierBulstat(other.supplierBulstat), items(other.items),
      totalPrice(other.totalPrice), orderDate(other.orderDate) {
}

Order::~Order() {
    items.clear();
}

std::string Order::getOrderId() const {
    return orderId;
}

std::string Order::getSupplierName() const {
    return supplierName;
}

std::string Order::getSupplierBulstat() const {
    return supplierBulstat;
}

double Order::getTotalPrice() const {
    return totalPrice;
}

std::string Order::getOrderDate() const {
    return orderDate;
}

int Order::getItemCount() const {
    return static_cast<int>(items.size());
}

void Order::addItem(const OpticalMaterial& material, int quantity) {
    validateQuantity(quantity);
    
    bool found = false;
    for (auto& item : items) {
        if (item.material.getType() == material.getType() &&
            item.material.getMaterialName() == material.getMaterialName() &&
            item.material.getThickness() == material.getThickness() &&
            item.material.getDiopter() == material.getDiopter()) {
            item.quantity += quantity;
            found = true;
            break;
        }
    }
    
    if (!found) {
        items.push_back(OrderItem(material, quantity));
    }
    
    calculateTotalPrice();
}

void Order::removeItem(int index) {
    if (index < 0 || index >= static_cast<int>(items.size())) {
        throw std::out_of_range("Invalid item index");
    }
    items.erase(items.begin() + index);
    calculateTotalPrice();
}

void Order::clearOrder() {
    items.clear();
    totalPrice = 0.0;
}

bool Order::isEmpty() const {
    return items.empty();
}

void Order::displayOrder() const {
    std::cout << "\n" << std::string(100, '=') << std::endl;
    std::cout << "ORDER DETAILS" << std::endl;
    std::cout << std::string(100, '=') << std::endl;
    std::cout << "Order ID: " << orderId << std::endl;
    std::cout << "Supplier: " << supplierName << " (Bulstat: " << supplierBulstat << ")" << std::endl;
    std::cout << "Order Date: " << orderDate << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    
    if (items.empty()) {
        std::cout << "No items in order." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "No."
                  << std::setw(20) << "Type"
                  << std::setw(15) << "Material"
                  << std::setw(12) << "Thickness"
                  << std::setw(10) << "Diopter"
                  << std::setw(10) << "Quantity"
                  << std::setw(12) << "Price/Unit"
                  << std::setw(12) << "Subtotal" << std::endl;
        std::cout << std::string(100, '-') << std::endl;
        
        for (size_t i = 0; i < items.size(); ++i) {
            const auto& item = items[i];
            double subtotal = item.material.getPrice() * item.quantity;
            
            std::cout << std::left << std::setw(5) << (i + 1)
                      << std::setw(20) << item.material.getType()
                      << std::setw(15) << item.material.getMaterialName()
                      << std::setw(12) << (std::to_string(item.material.getThickness()) + "mm")
                      << std::setw(10) << item.material.getDiopter()
                      << std::setw(10) << item.quantity
                      << std::setw(12) << std::fixed << std::setprecision(2) << item.material.getPrice()
                      << std::setw(12) << std::fixed << std::setprecision(2) << subtotal << std::endl;
        }
    }
    
    std::cout << std::string(100, '=') << std::endl;
    std::cout << std::right << std::setw(88) << "TOTAL: " 
              << std::fixed << std::setprecision(2) << totalPrice << " BGN" << std::endl;
    std::cout << std::string(100, '=') << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    order.displayOrder();
    return os;
}

void Order::saveToFile(std::ostream& os) const {
    os << orderId << "\n"
       << supplierName << "\n"
       << supplierBulstat << "\n"
       << orderDate << "\n"
       << totalPrice << "\n"
       << items.size() << "\n";
    
    for (const auto& item : items) {
        item.material.saveToFile(os);
        os << item.quantity << "\n";
    }
}

void Order::loadFromFile(std::istream& is) {
    std::getline(is, orderId);
    std::getline(is, supplierName);
    std::getline(is, supplierBulstat);
    std::getline(is, orderDate);
    is >> totalPrice;
    
    size_t itemCount;
    is >> itemCount;
    is.ignore();
    
    items.clear();
    for (size_t i = 0; i < itemCount; ++i) {
        OpticalMaterial material;
        material.loadFromFile(is);
        int quantity;
        is >> quantity;
        is.ignore();
        items.push_back(OrderItem(material, quantity));
    }
}

