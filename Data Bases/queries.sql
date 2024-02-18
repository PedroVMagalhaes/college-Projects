
--1



-- select COUNT(tin) from responsavel_por;
select nome from retalhista where tin in (
select tin as sub from (
select tin ,count(distinct nome_cat) as sub from responsavel_por
        group by tin
          having count(distinct nome_cat) =(select max(sub) from 
            (select tin,count(distinct nome_cat) as sub from responsavel_por group by tin) responsavel_por)) as sub_table);


--2:
-- select nome from retalhista where tin in (select tin from (
--     select tin,count(distinct nome_cat) from responsavel_por
--     where count = select count(distinct nome) from categoria_simples;
-- )) 


SELECT
  tin,
  COUNT(distinct nome_cat) AS asd
FROM
  responsavel_por
GROUP BY 
  tin
ORDER BY 
  asd DESC;


-- SELECT LASTNAME, MAX(*)
-- FROM STUDENTS
-- GROUP BY LASTNAME
--2


--3
select ean from produto where ean not in (
    select ean from evento_reposicao
);

--4
select ean from evento_reposicao group by ean having count(distinct tin) = 1;
