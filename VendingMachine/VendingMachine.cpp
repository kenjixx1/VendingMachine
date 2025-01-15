
#include <iostream>
#include "sqlite/sqlite3.h"
#include <string>
using namespace std;

void Setup();

class Database {
protected:
    sqlite3* db;
    char* errMsg = nullptr;
    int rc;
    
private:
    static int callback(void* data, int argc, char** argv, char** azColName) {
        cout << static_cast<const char*>(data) << "\n";
        for (int i = 0; i < argc; i++) {
            cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
        }
        cout << "-------------------------\n";
        return 0;
    }

    const char* sql = "SELECT * FROM snack;";

public:
    Database() {
        rc = sqlite3_open("db.db", &db);
        if (rc) {
            cerr << "Cant Open:" << sqlite3_errmsg(db) << endl;
        }
        else {
            cout << "Succesfully Opened!" << endl;
        }
    }

    ~Database() {
        sqlite3_close(db);
        cout << "Database connection closed." << endl;
    }

    void CreateTable() {
        Setup();
    }
        

    void ListItem() {
        const char* data = "\nDisplaying Users Table:";
        if (sqlite3_exec(db, sql, callback, (void*)data, &errMsg) != SQLITE_OK) {
            cerr << "SQL Error: " << errMsg << "\n";
            sqlite3_free(errMsg);
        }
        else {
            cout << "Query executed successfully.\n";
        }
    }
    void InsertMoney(int value, int amount) {
        const char* i_money = "UPDATE money SET amount = amount + ? WHERE value = ?;";

        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, i_money, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Peparing Insert Money:" << sqlite3_errmsg(db) << endl;
        }
        else {
            cout << "Prepared || Money " << endl;

        }

        sqlite3_bind_int(stmt, 1, amount);
        sqlite3_bind_int(stmt, 2, value);
       
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Error Executing statement:" << sqlite3_errmsg(db) << endl;
        }
        else {
            cout << "Inserted" << endl;
        }

        sqlite3_finalize(stmt);

    }
        
    /*void InsertUser(const string& name,int id, int age) {
        const char* insertuser = R"(
INSERT INTO users(name,studentid,age) VALUES (?,?,?);
)";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, insertuser, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error:" << sqlite3_errmsg(db) << endl;
            return;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, id);
        sqlite3_bind_int(stmt, 3, age);
        
        

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Error Executing statement:" << sqlite3_errmsg(db) << endl;
        }
        else {
            cout << "Inserted" << endl; 
        }
        sqlite3_finalize(stmt);
    }*/

    /*void FindUser(int id) {
        const char* userid = "SELECT * FROM users WHERE studentid = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, userid, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Preparing: " << sqlite3_errmsg(db) << endl;
            return;
        }

        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* name = reinterpret_cast<const char*> (sqlite3_column_text(stmt, 1));
            int sid = sqlite3_column_int(stmt, 2);
            int age = sqlite3_column_int(stmt, 3);
            cout << "User Found!" << endl;
            cout << "ID:" << sid << " || Name:" << name << " || Age" << age << endl;

        }
        else {
            cout << "No Found!" << endl;
        }
        sqlite3_finalize(stmt);


    }*/

    void DeleteTable(string table) {
        string m_delete = "DROP TABLE IF EXISTS " + table + ";";

        if (sqlite3_exec(db, m_delete.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            cerr << "Delete Table Error: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
        else {
            cout << "Deleted Table" << table << endl;
        }
    }

    void InsertProduct() {
        string name;
        int price;
        int stock;

        cin.ignore();
        cout << "Name Of Product:";
        getline(cin, name);
        cout << "Price Of Product:";
        cin >> price;
        cout << "Amount Of Stock:";
        cin >> stock;


        const char* i_product = "INSERT INTO snack(name,price,stock) VALUES(?,?,?);";

        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, i_product, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error:" << sqlite3_errmsg(db) << endl;
        }
        else {
            cout << "Prepared" << endl;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, price);
        sqlite3_bind_int(stmt, 3, stock);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            cout << "Inserted Product" << endl;
        }
        else {
            cerr << "Error Inserting Product||" << sqlite3_errmsg(db) << endl;
        }

        sqlite3_finalize(stmt);
    }



   

};

class User :public Database {
protected:
    int to_pay = 0;
public:
    void Login() {
        int studentid;
        cout << "Student ID:";
        cin >> studentid;

    }
    
    void DisplayProduct(string table) {
        string displayproduct = "SELECT * FROM "+table+";";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, displayproduct.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Displaying Product || " << sqlite3_errmsg(db) << endl;
        }
        

        cout << "-------------------------------------" << endl;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int price = sqlite3_column_int(stmt, 2);
            int stock = sqlite3_column_int(stmt, 3);

            cout << "[" << id << "] " << name << " |Price:" << price << " |Stock:" << stock << endl;
        }
        cout << "-------------------------------------" << endl;
        sqlite3_finalize(stmt);
        
    }

    void RemoveProduct(int id) {
        const char* removeproduct = "UPDATE snack SET stock = stock - 1 WHERE id = ?;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, removeproduct, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Prepared Removing Product:" << sqlite3_errmsg(db) << endl;
        }
        

        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Error Removing Product:" << sqlite3_errmsg(db) << endl;
        }
        


    }


    int GetPrice(int id) {
        const char* getprice = "SELECT price FROM snack where id = ?; ";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, getprice, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error getting price || " << sqlite3_errmsg(db) << endl;
        }
        
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int price = sqlite3_column_int(stmt, 0);
            return price;
        }

        sqlite3_finalize(stmt);
    }
    
};

class Admin :public User {
public:
    void AdminMenu() {
        int choice;
        cout << "[Admin Menu]\n[1]Restock\n[2]Add Product\n[3]Update Product\n[4]View Product\n[5]Search Product\n[6]\n";
        cin >> choice;
        if (choice == 2) {

            InsertProduct();
        }
        
    }

    void RestockProduct() {
        string name;
        cout << "Name Of product that you want to restock:";
        cin >> name;
        int amount;
        cout << "How much would you like to restock? :";
        cin >> amount;

        const char* restockproduct = "UPDATE snack SET stock = stock + ? where name = ?;";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, restockproduct, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Preparing restock" << sqlite3_errmsg << endl;
        }
        

        sqlite3_bind_int(stmt, 1, amount);
        sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) != SQLITE_OK) {
            cerr << "Error restocking||" << sqlite3_errmsg(db) << endl;
        }
        
    }
};


class VendingMachine :public Admin {
public:
    void CheckMoney() {
        const char* checkmoney = "SELECT * FROM money;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, checkmoney, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Peparing to check money:" << sqlite3_errmsg(db) << endl;
        }


        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int value = sqlite3_column_int(stmt, 0);
            int amount = sqlite3_column_int(stmt, 1);
            cout << "Value:" << value << " ||Amount:" << amount << endl;
            if (amount == 0) {
                CloseVendingMachine();
            }
        }
    }

    void CloseVendingMachine() {
        cout << "Vending Machine Closed." << endl;
        exit(0);
    }

    int AskPayment() {
        int money;
        cout << "[1]1 Baht\n[2]5 Baht\n[3]10 Baht\n[4]20 Baht\n[5]100 Baht" << endl;
        cin >> money;
        int r_value;
        switch (money) {
        case 1: 
            r_value = 1;
            break;
        case 2:
            r_value = 5;
            break;
        case 3:
            r_value = 10;
            break;
        case 4:
            r_value = 20;
            break;
        case 5:
            r_value = 100;
            break;
        }
        ReceiveMoney(r_value);
        return r_value;

    }
    

    void ReceiveMoney(int value) {
        InsertMoney(value, 1);
        cout << "Received Payment" << endl;
    }

    void GiveChange(int amount) {
        amount = amount * -1;
        while (amount > 1) {
            if (amount >= 100) {
                InsertMoney(100, -1);
                amount -= 100;
            }
            else if (amount >= 20) {
                InsertMoney(20, -1);
                amount -= 20;
            }
            else if (amount >= 10) {
                InsertMoney(10, -1);
                amount -= 10;
            }
            else if (amount >= 5) {
                InsertMoney(5, -1);
                amount -= 5;
            }
            else if (amount >= 1) {
                InsertMoney(1, -1);
                amount -= 1;
            }

            cout << "Amount Of changes:" << amount << endl;
        }
    }

    void ChooseProduct() {
        int choice;
        DisplayProduct("snack");
        cout << "Choose Product:";
        cin >> choice;
        to_pay = GetPrice(choice);
        while (to_pay > 1) {
            cout << "To pay:" << to_pay << endl;
            to_pay = to_pay - AskPayment();
            CheckMoney();
        }
        cout << to_pay << endl;
        if (to_pay < 0) {
            GiveChange(to_pay);
        }

    }
};





int main()
{

    Admin a;
    a.AdminMenu();
    
    

    

    

}

