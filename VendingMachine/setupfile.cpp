#include <iostream>
#include "sqlite/sqlite3.h"
#include <string>
using namespace std;


void Setup() {
	sqlite3* db;
	char* Errmsg = nullptr;
	int rc;

	const char* SQLSnackTable = R"(
CREATE TABLE IF NOT EXISTS snack (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name TEXT NOT NULL UNIQUE,
price INTEGER NOT NULL,
stock INTEGER NOT NULL
);
)";

	const char* SQLMoneyTable = R"(
CREATE TABLE IF NOT EXISTS money (
value INTEGER NOT NULL UNIQUE,
amount INTEGER NOT NULL
);
)";

	const char* SQLHistoryTable = R"(
CREATE TABLE IF NOT EXISTS history (
id INTEGER PRIMARY KEY AUTOINCREMENT,
buyerid INTEGER NOT NULL,
price INTEGER NOT NULL
);
)";


	rc = sqlite3_open("db.db", &db);
	if (rc != SQLITE_OK) {
		cerr << "Error Opening File:" << sqlite3_errmsg(db) << endl;
	}

	rc = sqlite3_exec(db, SQLSnackTable, nullptr, nullptr, &Errmsg);
	if (rc != SQLITE_OK) {
		cerr << "Error Creating SnackTable:" << Errmsg << endl;
	}

	rc = sqlite3_exec(db, SQLMoneyTable, nullptr, nullptr, &Errmsg);
	if (rc != SQLITE_OK) {
		cerr << "Error Creating MoneyTable:" << Errmsg << endl;
	}

	rc = sqlite3_exec(db, SQLHistoryTable, nullptr, nullptr, &Errmsg);
	if (rc != SQLITE_OK) {
		cerr << "Error Creating History Table:" << Errmsg << endl;
	}

	cout << "Created Succesfully" << endl;
	sqlite3_close(db);

}




