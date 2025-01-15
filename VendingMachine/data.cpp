class Database {
protected:
    sqlite3* db;
    char* errMsg = nullptr;
    int rc;

private:
    const char* UserTable = R"(
CREATE TABLE IF NOT EXISTS users (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name TEXT NOT NULL,
studentid INTEGER NOT NULL UNIQUE,
age INTEGER NOT NULL,
password TEXT
);
)";

    const char* snackTable = R"(
CREATE TABLE IF NOT EXISTS snack (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name TEXT NOT NULL UNIQUE,
price INTEGER NOT NULL,
stock INTEGER NOT NULL
);
)";

    const char* moneyTable = R"(
CREATE TABLE IF NOT EXISTS money (
value INTEGER NOT NULL UNIQUE,
amount INTEGER NOT NULL
);
)";



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
        rc = sqlite3_exec(db, snackTable, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "SQL error:" << errMsg << endl;
            sqlite3_free(errMsg);
        }
        else {
            cout << "Created" << endl;
        }

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

    void InsertUser(const string& name, int id, int age) {
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
    }

    void FindUser(int id) {
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


    }

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

    void InsertProduct(string name, int price, int stock) {
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