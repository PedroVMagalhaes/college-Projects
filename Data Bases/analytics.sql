

--1:
SELECT WeekDay, concelho, SUM(unidades)
FROM view_table
WHERE Year = 2020 and Month >= 2 and Month <= 7
GROUP BY GROUPING SETS ((WeekDay), (concelho))
union all
SELECT null,null,SUM(unidades)
FROM view_table
WHERE Year = 2020 and Month >= 2 and Month <= 7;

--2:

SELECT concelho, cat,WeekDay, SUM(unidades)
FROM view_table
WHERE distrito = 'Lisboa'
GROUP BY GROUPING SETS ((concelho),(cat),(WeekDay) )
union all
SELECT null,null,null,SUM(unidades)
FROM view_table
WHERE distrito = 'Lisboa';


