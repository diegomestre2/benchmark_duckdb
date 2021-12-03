#pragma once
#include "../third_party/duckdb/src/include/duckdb.hpp"

#include <iostream>
#include <map>
#include <string>
#include <sys/time.h>
#include <variant>
using namespace std;

using namespace duckdb;

class DuckDBUtil {
public:
	explicit DuckDBUtil(string_view fname) : d_db(&fname[0]), d_con(d_db) {
		d_con.BeginTransaction();
	}

	~DuckDBUtil() {
		cerr << "Closing appender" << endl;
		if (d_appender)
			d_appender->Close();
		cerr << "Committing transaction" << endl;
		d_con.Commit();
		cerr << "Done with commit" << endl;
	}

	void cycle() {
		if (d_appender)
			d_appender->Flush();
	}
	typedef std::variant<double, int32_t, uint32_t, int64_t, string> var_t;
	//! store a datum. Tags could be indexed later.
	void addValue(const std::vector<std::pair<std::string, var_t>> &tags, std::string name,
	              const initializer_list<std::pair<const char *, var_t>> &values, double t);

private:
	// for each table, the known types
	std::map<std::string, std::vector<std::pair<std::string, std::string>>> d_types;
	duckdb::DuckDB d_db;
	duckdb::Connection d_con;
	std::unique_ptr<duckdb::Appender> d_appender;
};