
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
	result = con.Query(".read create_tpch_no_pk.sql;");
	result->Print();
}

int main(int argc, char **argv) {
	DuckDB db(nullptr);
	Connection con(db);
	duckdb_hello(con);
	create_tpch_without_pk(con);
}

SELECT
    nation,
    o_year,
    sum(amount) AS sum_profit
FROM (
    SELECT
        n_name AS nation,
        extract(year FROM o_orderdate) AS o_year,
        l_extendedprice * (1 - l_discount) - ps_supplycost * l_quantity AS amount
    FROM (
        SELECT ps_supplycost, ps_partkey, n_name 
        FROM supplier 
        JOIN lineitem ON s_suppkey = l_suppkey
        JOIN partsupp ON ps_suppkey = s_suppkey
        JOIN part ON ps_partkey = p_partkey
        JOIN nation ON s_nationkey = n_nationkey
        WHERE p_name LIKE '%green%') AS temp,
        lineitem,
        orders
    WHERE
        ps_suppkey = l_suppkey
        AND ps_partkey = l_partkey
        AND o_orderkey = l_orderkey
GROUP BY
    nation,
    o_year
ORDER BY
    nation,
    o_year DESC;