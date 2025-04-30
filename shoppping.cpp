#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

class Product {
public:
    int id;
    string name;
    int quantity;
    float price;

    Product() = default;

    Product(int id, string name, int quantity, float price)
        : id(id), name(name), quantity(quantity), price(price) {}
};

class Shop {
private:
    vector<Product> inventory;

    void loadInventory() {
        inventory.clear();
        fstream data("database.txt", ios::in);
        if (!data) return;

        Product temp;
        while (data >> temp.id >> temp.name >> temp.quantity >> temp.price) {
            inventory.push_back(temp);
        }
        data.close();
    }

    void saveInventory() {
        fstream data("database.txt", ios::out | ios::trunc);
        for (auto& p : inventory) {
            data << p.id << " " << p.name << " " << p.quantity << " " << p.price << "\n";
        }
        data.close();
    }

    int findProductIndex(int id) {
        for (int i = 0; i < inventory.size(); ++i) {
            if (inventory[i].id == id) return i;
        }
        return -1;
    }

public:
    void addProduct() {
        Product p;
        cout << "Enter Product ID: ";
        cin >> p.id;
        if (findProductIndex(p.id) != -1) {
            cout << "Product already exists.\n";
            return;
        }
        cout << "Enter Product Name: ";
        cin >> p.name;
        cout << "Enter Quantity: ";
        cin >> p.quantity;
        cout << "Enter Price: ";
        cin >> p.price;

        inventory.push_back(p);
        saveInventory();
        cout << "Product added successfully.\n";
    }

    void updateProduct() {
        int id;
        cout << "Enter Product ID to update: ";
        cin >> id;
        int index = findProductIndex(id);
        if (index == -1) {
            cout << "Product not found.\n";
            return;
        }

        cout << "Enter New Name: ";
        cin >> inventory[index].name;
        cout << "Enter New Quantity: ";
        cin >> inventory[index].quantity;
        cout << "Enter New Price: ";
        cin >> inventory[index].price;

        saveInventory();
        cout << "Product updated successfully.\n";
    }

    void deleteProduct() {
        int id;
        cout << "Enter Product ID to delete: ";
        cin >> id;
        int index = findProductIndex(id);
        if (index == -1) {
            cout << "Product not found.\n";
            return;
        }
        inventory.erase(inventory.begin() + index);
        saveInventory();
        cout << "Product deleted successfully.\n";
    }

    void displayProducts() {
        loadInventory();
        cout << "\nAvailable Products:\n";
        cout << setw(10) << "ID" << setw(15) << "Name" << setw(15) << "Quantity" << setw(15) << "Price\n";
        for (auto& p : inventory) {
            cout << setw(10) << p.id << setw(15) << p.name << setw(15) << p.quantity << setw(15) << p.price << "\n";
        }
    }

    void buyProducts() {
        loadInventory();
        if (inventory.empty()) {
            cout << "No products available.\n";
            return;
        }

        displayProducts();

        vector<int> productIds;
        vector<int> quantities;
        char more = 'y';
        float total = 0;

        while (more == 'y') {
            int id, qty;
            cout << "Enter Product ID to buy: ";
            cin >> id;
            cout << "Enter Quantity: ";
            cin >> qty;

            int index = findProductIndex(id);
            if (index == -1 || qty > inventory[index].quantity) {
                cout << "Invalid ID or Insufficient quantity.\n";
            } else {
                productIds.push_back(id);
                quantities.push_back(qty);
                inventory[index].quantity -= qty;
                total += qty * inventory[index].price;
            }

            cout << "Buy another product? (y/n): ";
            cin >> more;
        }

        saveInventory();

        cout << "\n--- Receipt ---\n";
        for (size_t i = 0; i < productIds.size(); ++i) {
            int index = findProductIndex(productIds[i]);
            cout << inventory[index].name << " x " << quantities[i]
                 << " = " << quantities[i] * inventory[index].price << "\n";
        }
        cout << "Total Amount: " << total << "\n";
    }
};

void adminMenu(Shop& shop) {
    int choice;
    do {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Add Product\n2. Update Product\n3. Delete Product\n4. Display Products\n5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: shop.addProduct(); break;
            case 2: shop.updateProduct(); break;
            case 3: shop.deleteProduct(); break;
            case 4: shop.displayProducts(); break;
            case 5: return;
            default: cout << "Invalid choice.\n";
        }
    } while (true);
}

void userMenu(Shop& shop) {
    int choice;
    do {
        cout << "\n--- User Menu ---\n";
        cout << "1. Display Products\n2. Buy Products\n3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: shop.displayProducts(); break;
            case 2: shop.buyProducts(); break;
            case 3: return;
            default: cout << "Invalid choice.\n";
        }
    } while (true);
}

int main() {
    Shop shop;
    int role;
    string password;

    cout << "--- Welcome to Shopping System ---\n";
    cout << "1. Admin\n2. User\nEnter role: ";
    cin >> role;

    if (role == 1) {
        cout << "Enter Admin Password: ";
        cin >> password;
        if (password == "123") {
            adminMenu(shop);
        } else {
            cout << "Incorrect password.\n";
        }
    } else if (role == 2) {
        userMenu(shop);
    } else {
        cout << "Invalid role selected.\n";
    }

    return 0;
}
