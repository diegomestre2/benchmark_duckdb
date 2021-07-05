import duckdb

runs = 10
probe_side = 1024
build_side = 1024
con = duckdb.connect(':memory:')
query = "SELECT test1.r, test2.r FROM test1,test2 WHERE test1.r=test2.r;"


def create_tables():
    con.execute("CREATE TABLE test1 AS SELECT range r FROM range(0, 1024, 1);")
    con.execute("CREATE TABLE test2(r integer);")


def populate_data(probe_times_build):
    for i in range(0, probe_times_build):
        con.execute("INSERT INTO test2 SELECT * FROM test1;")


def run_and_profile_query(query):
    con.execute("PRAGMA enable_profiling;")
    con.execute("PRAGMA profiling_output='out.log';")
    con.execute("PRAGMA profiling_mode=detailed;")
    con.execute(query)
    con.execute(
        "SELECT EXTRA_INFO, TIME FROM pragma_detailed_profiling_output();")
    print(con.fetchall())


def main():
    create_tables()
    for probe_range in [10, 100, 1000, 10000, 100000, 1000000]:
        print("Probe size: " + (probe_range*1000), end='')
        populate_data(probe_range)
        run_and_profile_query(query)


if __name__ == "__main__":
    main()

'''Test duckdb when installed with pip install duckdb'''
