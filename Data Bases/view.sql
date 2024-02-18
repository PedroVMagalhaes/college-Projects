DROP TABLE view_table CASCADE;
CREATE TABLE view_table AS (
SELECT 
    p.ean,p.cat,
    EXTRACT(YEAR FROM er.instante) as Year,
    EXTRACT(QUARTER FROM er.instante) as Trimester,
    EXTRACT(MONTH FROM er.instante) as Month,
    EXTRACT(DAY FROM er.instante) as Day,
    EXTRACT(DOW FROM er.instante) as WeekDay,
    pr.distrito,
    pr.concelho,
    er.unidades
    --INTO view_table
FROM
    produto as p
INNER JOIN
    evento_reposicao AS er ON p.ean = er.ean 
INNER JOIN
    instalada_em AS ie ON er.num_serie =ie.num_serie and er.fabricante = ie.fabricante
INNER JOIN
    ponto_de_retalho as pr ON ie.local = pr.nome);


SELECT * from view_table;