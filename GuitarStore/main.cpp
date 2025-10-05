// Simple Guitar Store Inventory System

// Includes and namespace
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <limits>   // for numeric_limits

using namespace std;


// Guitar Class (represents a guitar in the inventory)
class Guitar {
public:
    int id;             // Each guitar has a unique id
    string brand;
    string model;
    string type;        // Electric or acoustic
    double price;
    int quantity;

    //Constructor to initialize a new object
    Guitar(int id_, string brand_, string model_, string type_, double price_, int quantity_)
        : id(id_), brand(move(brand_)), model(move(model_)), type(move(type_)), price(price_), quantity(quantity_) {}


    // print guitar info to console
    void print() const {
        cout << "ID: " << id
             << " | " << brand << " " << model
             << " | Type: " << type
             << " | Price: $" << price
             << " | Qty: " << quantity << "\n";
    }
};


// Inventory Class (Holds all of the guitars)
class Inventory {
private:
    vector<Guitar> items;                   // Array of guitar objects
    unordered_map<int, size_t> idIndexMap;  // Hash map from id
    int nextId = 1;

public:
    Inventory() = default;

    // Adds a new guitar to items vector 
    int addGuitar(const string &brand, const string &model, const string &type, double price, int quantity) {
        int id = nextId++;
        items.emplace_back(id, brand, model, type, price, quantity);        // Creates a new guitar in place
        idIndexMap[id] = items.size() - 1;                                  // updates idIndexMap
        return id;                                                          // returns id
    }

    // Removes guitar by id and returns true if removed
    bool removeById(int id) {
        auto it = idIndexMap.find(id);              // Checks if id exists in idIndexMap
        if (it == idIndexMap.end()) return false;

        size_t index = it->second;
        // swap with last and pop_back to remove vector
        size_t lastIndex = items.size() - 1;
        if (index != lastIndex) {
            items[index] = items[lastIndex];        // move last element to index
            idIndexMap[items[index].id] = index;    // update moved element's index
        }
        items.pop_back();
        idIndexMap.erase(it);
        return true;
    }

    // Finds the guitar by id and sets it's quantity
    bool updateQuantity(int id, int newQuantity) {
        auto it = idIndexMap.find(id);
        if (it == idIndexMap.end()) return false;
        items[it->second].quantity = newQuantity;
        return true;
    }

    // Loops through all guitars and returns a vector of guitars matching the brand
    vector<Guitar> findByBrand(const string &brand) const {
        vector<Guitar> result;
        for (const auto &g : items) {
            if (g.brand == brand) result.push_back(g);
        }
        return result;
    }

    // Lists all guitars by looping over items and printing each one
    void listAll() const {
        if (items.empty()) {
            cout << "Inventory is empty.\n";
            return;
        }
        for (const auto &g : items) {
            g.print();
        }
    }

    // Calculate total inventory value (sum price * quantity)
    double totalValue() const {
        double total = 0.0;
        for (const auto &g : items) {
            total += g.price * g.quantity;
        }
        return total;
    }

    // Checks if an id exists
    bool exists(int id) const {
        return idIndexMap.find(id) != idIndexMap.end();
    }
};

// Helper Functions

// Skips the input buffer after an invalid input
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Prompts the user to type in a valid number
int readInt(const string &prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) {
            clearInput();
            return x;
        } else {
            cout << "Invalid integer; try again.\n";
            clearInput();
        }
    }
}

double readDouble(const string &prompt) {
    double d;
    while (true) {
        cout << prompt;
        if (cin >> d) {
            clearInput();
            return d;
        } else {
            cout << "Invalid number; try again.\n";
            clearInput();
        }
    }
}

// Reads a line of text
string readLine(const string &prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

// Prints the menu
void printMenu() {
    cout << "\n=== Guitar Store Inventory ===\n"
         << "1) Add guitar\n"
         << "2) Remove guitar by ID\n"
         << "3) Update quantity by ID\n"
         << "4) List all guitars\n"
         << "5) Find guitars by brand\n"
         << "6) Show total inventory value\n"
         << "0) Exit\n";
}


// Main Class
int main() {
    Inventory inv;

    // creates some inventory objects
    inv.addGuitar("Fender", "Stratocaster", "Electric", 1299.99, 3);
    inv.addGuitar("Gibson", "Les Paul", "Electric", 2499.00, 2);
    inv.addGuitar("Martin", "D-28", "Acoustic", 2999.00, 1);

    bool running = true;

    // Main loop
    while (running) {
        printMenu();
        int choice = readInt("Choose an option: ");

        // Conditionals to handle menu options
        switch (choice) {
            case 1: {
                string brand = readLine("Brand: ");
                string model = readLine("Model: ");
                string type = readLine("Type (Electric/Acoustic/...): ");
                double price = readDouble("Price (e.g., 1299.99): ");
                int qty = readInt("Quantity: ");
                if (price < 0 || qty < 0) {
                    cout << "Price and quantity must be non-negative.\n";
                } else {
                    int id = inv.addGuitar(brand, model, type, price, qty);
                    cout << "Added with ID " << id << ".\n";
                }
                break;
            }
            case 2: {
                int id = readInt("Enter ID to remove: ");
                if (inv.removeById(id)) {
                    cout << "Removed ID " << id << ".\n";
                } else {
                    cout << "ID not found.\n";
                }
                break;
            }
            case 3: {
                int id = readInt("Enter ID to update: ");
                if (!inv.exists(id)) {
                    cout << "ID not found.\n";
                } else {
                    int qty = readInt("New quantity: ");
                    if (qty < 0) {
                        cout << "Quantity cannot be negative.\n";
                    } else {
                        inv.updateQuantity(id, qty);
                        cout << "Quantity updated.\n";
                    }
                }
                break;
            }
            case 4: {
                inv.listAll();
                break;
            }
            case 5: {
                string brand = readLine("Brand to search (case-sensitive): ");
                auto found = inv.findByBrand(brand);
                if (found.empty()) {
                    cout << "No guitars found for brand \"" << brand << "\".\n";
                } else {
                    cout << found.size() << " result(s):\n";
                    for (const auto &g : found) g.print(); 
                }
                break;
            }
            case 6: {
                double t = inv.totalValue();
                cout.setf(std::ios::fixed); cout.precision(2);
                cout << "Total inventory value: $" << t << "\n";
                cout.unsetf(std::ios::fixed);
                break;
            }
            case 0:
                cout << "Goodbye!\n";
                running = false;
                break;
            default:
                cout << "Unknown option. Choose from the menu.\n";
                break;
        } 
    } 

    return 0;
}
