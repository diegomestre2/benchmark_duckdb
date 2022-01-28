 SELECT
     nation,
     o_year,
     sum(amount) AS sum_profit
 FROM (
        SELECT
            n_name AS nation,
            extract(year FROM o_orderdate) AS o_year,
            l_extendedprice * (1 - l_discount) - temp.ps_supplycost * l_quantity AS amount
        FROM (
                SELECT ps_supplycost, 
                    ps_partkey, n_name
                FROM supplier
                    JOIN lineitem ON s_suppkey = l_suppkey
                    JOIN partsupp ON ps_suppkey = s_suppkey
                    JOIN part ON ps_partkey = p_partkey
                    JOIN nation ON s_nationkey = n_nationkey
                WHERE p_name LIKE '%green%'
            ) AS temp
        JOIN lineitem ON  ps_partkey = l_partkey
        JOIN orders ON o_orderkey = l_orderkey
    ) as temp2
GROUP BY
    nation,
    o_year
ORDER BY
    nation,
    o_year DESC;

