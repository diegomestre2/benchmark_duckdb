import os
from os import path
import shutil
from pathlib import Path

home_dir = str(Path.home())
master_link = "https://github.com/duckdb/duckdb.git"
fork_link = "https://github.com/diegomestre2/duckdb.git"
master_dir = "master_duckdb"
fork_dir = "fork_duckdb"
tpch_queries = ["q01", "q02", "q03", "q04", "q05", "q06", "q07", "q08", "q09", "q10",
                "q11", "q12", "q13", "q14", "q15", "q16", "q17", "q18", "q19", "q20", "q21", "q22"]


def remove_python_duckdb():
    os.system("pip uninstall duckdb")


def remove_dir(dir_path):
    if path.exists(dir_path):
        shutil.rmtree(dir_path)


def run_duckdb_tpch(dir_path="", queries=tpch_queries):
    if not path.exists(dir_path):
        current_dir = os.getcwd()
        current_dir += "/duckdb"
        if not path.exists(current_dir):
            raise Exception("This path does not exist: " + dir_path)
        dir_path = current_dir
    # Build
    print(dir_path)
    os.system("cd " + dir_path + " && BUILD_BENCHMARK=1 BUILD_TPCH=1 make")
    # Execute each query individually
    for query in queries:
        os.system("cd " + dir_path + " && build/release/benchmark/benchmark_runner " +
                  "benchmark/tpch/sf1/"+query+".benchmark")


def clone_duckdb(link, dir_path="duckdb"):
    os.system("git clone " + str(link) + " " + str(dir_path))


def main():
    # clone_duckdb(link=master_link)
    run_duckdb_tpch(queries=["q04", "q09"])
    #clone_duckdb(link=fork_link, dir_path=fork_dir)
    run_duckdb_tpch(dir_path=fork_dir, queries=["q04", "q09"])


if __name__ == "__main__":
    main()
