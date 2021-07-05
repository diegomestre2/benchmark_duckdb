
#include "duckdb_util.hpp"

#include <iostream>
#include <map>
#include <string>
#include <sys/time.h>
#include <variant>
using namespace std;

int main(int argc, char **argv) try {
	std::cout << "hello";
	/* 	if (argc != 3) {
	        cerr << "Syntax: ducktime duckdbname number\nAdd 'number' items to the duckdbname database\n";
	        return EXIT_FAILURE;
	    }
	    DuckDBUtil dt(argv[1]);
	    int limit = atoi(argv[2]);
	    struct timeval tv;
	    for (int n = 0; n < limit; ++n) {
	        double t;
	        gettimeofday(&tv, 0);
	        t = tv.tv_sec * 1000 + tv.tv_usec / 1000.0;
	        dt.addValue({{"server", n % 16}}, "network", {{"in", n * 1234}, {"out", n * 321}}, t);

	        if (!(n % (1 << 20))) {
	            cerr << "Cycle time: " << 100.0 * n / limit << "%" << endl;
	            dt.cycle();
	        }
	    }
	} catch (std::exception &e) {
	    cerr << "Fatal error: " << e.what() << endl; */
}