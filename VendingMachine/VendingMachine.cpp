
#include <iostream>
#include "sqlite/sqlite3.h"
#include <string>
#include <tuple>
using namespace std;

void Setup();

struct Product {
    string name;
    int price, stock;

    Product(string n, int p, int s)
        :name(n), price(p), stock(s) {
    }
};

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
            //cout << "Query executed successfully.\n";
        }
    }
    void InsertMoney(int value, int amount) {
        const char* i_money = "UPDATE money SET amount = amount + ? WHERE value = ?;";

        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, i_money, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Peparing Insert Money:" << sqlite3_errmsg(db) << endl;
        }
        else {
            //cout << "Prepared || Money " << endl;

        }

        sqlite3_bind_int(stmt, 1, amount);
        sqlite3_bind_int(stmt, 2, value);
       
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Error Executing statement:" << sqlite3_errmsg(db) << endl;
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

    void InsertProduct(string& name, int price, int stock) {
        const char* i_product = "INSERT INTO temptable(name,price,stock) VALUES(?,?,?);";

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
    int studentid;
public:


    void CloseVendingMachine() {
        cout << "Vending Machine Closed." << endl;
        exit(0);
    }
    void Login() {
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

    int GetProductLen() {
        string displayproduct = "SELECT * FROM snack;";
        sqlite3_stmt* stmt;
        int count = 0;
        if (sqlite3_prepare_v2(db, displayproduct.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Displaying Product || " << sqlite3_errmsg(db) << endl;
        }
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int price = sqlite3_column_int(stmt, 2);
            int stock = sqlite3_column_int(stmt, 3);
            count++;
        }
        cout << "------------------"<< count <<"------------------" << endl;
        sqlite3_finalize(stmt);
        return count;
    }

    void RenameTableToSnack() {
        string rename = "ALTER TABLE temptable RENAME TO snack;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, rename.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Renaming:" << sqlite3_errmsg(db) << endl;
        }

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Error" << endl;
        }

        sqlite3_finalize(stmt);
    }

    Product GetProductInfo(int productid) {
        string getinfo = "SELECT * FROM snack WHERE id = "+to_string(productid)+";";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, getinfo.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Preparing Getting info:" << sqlite3_errmsg(db) << endl;
        }
        string name;
        int price;
        int stock;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            price = sqlite3_column_int(stmt, 2);
            stock = sqlite3_column_int(stmt, 3);
        }

        sqlite3_finalize(stmt);
        return Product(name, price, stock);
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
        cout << "[Admin Menu]\n[1]Restock\n[2]Add Product\n[3]Remove Product\n[4]Reset Money/Take\n[5]Show Money\n[6]History\n";
        cin >> choice;
        switch (choice) {
        case 1:
            RestockProduct();
            break;
        case 2:
            InsertProduct();
            break;
        case 3:
            AdminRemoveProduct();
            break;

        case 4:
            ResetMoney();
            break;
        case 5:
            ShowMoney();
            break;
        case 6:
            ShowHistory();
            break;
        }
        
    }

    void ResetMoney() {
        int amount, value;
        cout << "Value[1,5,10,20,100]:";
        cin >> value;
        cout << "Amount:";
        cin >> amount;
        const char* i_money = "UPDATE money SET amount = ? WHERE value = ?;";

        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, i_money, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Peparing Insert Money:" << sqlite3_errmsg(db) << endl;
        }

        sqlite3_bind_int(stmt, 1, amount);
        sqlite3_bind_int(stmt, 2, value);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Error Executing statement:" << sqlite3_errmsg(db) << endl;
        }

        sqlite3_finalize(stmt);

    }

    void RestockProduct() {
        DisplayProduct("snack");
        int id;
        cout << "ID Of product that you want to restock:";
        cin >> id;
        int amount;
        cout << "How much would you like to restock? :";
        cin >> amount;

        const char* restockproduct = "UPDATE snack SET stock = stock + ? where id = ?;";
        sqlite3_stmt* stmt;
        
        if (sqlite3_prepare_v2(db, restockproduct, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Preparing restock" << sqlite3_errmsg << endl;
        }
        

        sqlite3_bind_int(stmt, 1, amount);
        sqlite3_bind_int(stmt, 2, id);
        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << "Error restocking||" << sqlite3_errmsg(db) << endl;
        }
        
    }

    void AdminRemoveProduct() {
        DisplayProduct("snack");
        const char* TempTable = R"(
CREATE TABLE IF NOT EXISTS temptable (
id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
name TEXT NOT NULL UNIQUE,
price INTEGER NOT NULL,
stock INTEGER NOT NULL
);
)";

        if (sqlite3_exec(db, TempTable, nullptr, nullptr, &errMsg) != SQLITE_OK) {
            cerr << "Error Creating temptable:" << errMsg << endl;
        }
        int productlen = GetProductLen();
        int removeid;
        cout << "ID of the product you want to remove:";
        cin >> removeid;
        for (int i = 1; i <= productlen; i++) {
            if (i != removeid) {
                Product prod = GetProductInfo(i);
                InsertProduct(prod.name, prod.price, prod.stock);
            }
        }

        DeleteTable("snack");
        RenameTableToSnack();
        DisplayProduct("snack");

        

    }
    void ShowMoney() {
        const char* checkmoney = "SELECT * FROM money;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, checkmoney, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Peparing to check money:" << sqlite3_errmsg(db) << endl;
        }


        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int value = sqlite3_column_int(stmt, 0);
            int amount = sqlite3_column_int(stmt, 1);
            cout << "Value:" << value << " ||Amount:" << amount << endl;
        }
    }

    void ShowHistory() {
        const char* viewhistory = "SELECT * FROM history;";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, viewhistory, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Error Displaying History || " << sqlite3_errmsg(db) << endl;
        }


        cout << "-------------------------------------" << endl;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int studentidd = sqlite3_column_int(stmt, 2);
            int price = sqlite3_column_int(stmt, 3);
            cout << "[" << id << "] " << name << " |Price:" << price << " |Buyer:" << studentidd << endl;
        }
        cout << "-------------------------------------" << endl;
        sqlite3_finalize(stmt);
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
            //cout << "Value:" << value << " ||Amount:" << amount << endl;
            if (amount <= 1) {
                CloseVendingMachine();
            }
        }
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
        cout << "Amount Of changes:" << amount << endl;
        while (amount > 1) {
            CheckMoney();
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

            
        }
    }

    void BuyProduct() {
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
        if (to_pay < 0) {
            GiveChange(to_pay);
        }
        RemoveProduct(choice);
        InsertHistory(choice);
    }

    void InsertHistory(int id) {
        Product prod = GetProductInfo(id);
        string inserthistorystmt = "INSERT INTO history (name,buyerid,price) VALUES\n('" + prod.name + "'," + to_string(studentid) + "," + to_string(prod.price) + ")";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, inserthistorystmt.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << sqlite3_errmsg(db) << endl;
        }

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            cerr << sqlite3_errmsg(db) << endl;
        }
   
        
        sqlite3_finalize(stmt);
    }
};





int main()
{
    //Setup(); //Run This for the first time
    int choice;
    VendingMachine a;
    
    cout << "[1]Admin Mode || [2]Usermode\nEnter:";
    cin >> choice;
    if (choice == 1) {
        a.AdminMenu();
    }
    else {
        a.Login();
        a.BuyProduct();
    }
    
    
    

    

    

}

