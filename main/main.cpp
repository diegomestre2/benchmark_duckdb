
#include "duckdb_util.hpp"

#include <iostream>
#include <map>
#include <string>
#include <sys/time.h>
#include <variant>
using namespace std;

void duckdb_hello(Connection &con) {

	con.EnableProfiling();
	con.Query("CREATE TABLE integers(i INTEGER);");
	con.Query("INSERT INTO integers VALUES (3);");
	auto result = con.Query("SELECT * FROM integers;");
	result->Print();
}
void create_tpch_without_pk(Connection &con) {
	auto result = con.Query("CALL dbgen(sf=0.1);");
	result->Print();
	result = con.Query("PRAGMA threads=1;");
	result->Print();
}

int main(int argc, char **argv) {
	DuckDB db(nullptr);
	Connection con(db);
	duckdb_hello(con);
	create_tpch_without_pk(con);
}