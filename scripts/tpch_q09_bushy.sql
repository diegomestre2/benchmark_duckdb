SELECT
    nation,
    o_year,
    sum(amount) AS sum_profit
FROM (
        SELECT
            n_name AS nation,
            extract(year FROM o_orderdate) AS o_year,
            l_extendedprice * (1 - l_discount) - ps_supplycost * l_quantity AS amount
        FROM
            lineitem 
            JOIN (SELECT p_partkey FROM part WHERE p_name LIKE '%green%') as temp ON p_partkey = l_partkey 
            JOIN partsupp ON ps_partkey = l_partkey AND ps_suppkey = l_suppkey
            JOIN orders ON o_orderkey = l_orderkey
            JOIN supplier ON s_suppkey = l_suppkey
            JOIN nation ON s_nationkey = n_nationkey
 ) as profit
GROUP BY
    nation,
    o_year
ORDER BY
    nation,
    o_year DESC;
