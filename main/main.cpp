
#include "duckdb_util.hpp"

#include <iostream>
#include <map>
#include <string>
#include <sys/time.h>
#include <variant>
using namespace std;

int main(int argc, char **argv) {
	DuckDB db(nullptr);
	Connection con(db);

	con.Query("CREATE TABLE integers(i INTEGER)");
	con.Query("INSERT INTO integers VALUES (3)");
	auto result = con.Query("SELECT * FROM integers");
	result->Print();
}