insert into categoria values ('Batatas');
insert into categoria values ('Chocolates');
insert into categoria values ('Aguas');
insert into categoria values ('Ruffles');
insert into categoria values ('Lays');

insert into categoria_simples values ('Chocolates');
insert into categoria_simples values ('Aguas');
insert into categoria_simples values ('Ruffles');
insert into categoria_simples values ('Lays');

insert into super_categoria values ('Batatas');

insert into tem_outra values ('Batatas', 'Ruffles');
insert into tem_outra values ('Batatas', 'Lays');

insert into produto values ('5601126900010',  'Chocolates', 'Chocolate para crianças');
insert into produto values ('5601126900020',  'Aguas', 'Se quiser ser assaltado');
insert into produto values ('5601126900030',  'Aguas', 'Pior que a do cano');
insert into produto values ('5601126900040',  'Lays', 'Com o sabor original do campo');
insert into produto values ('5601126900050',  'Ruffles', 'Tire uma onda');
insert into produto values ('5601126900060',  'Batatas', 'Quem e que comeu os cheetos');
insert into produto values ('5601126900070',  'Chocolates', 'Tu nao es tu quando tens fome');
insert into produto values ('5601126900080',  'Lays', 'Impossivel comer uma so');
insert into produto values ('5601126900090',  'Ruffles', 'Tire uma onda');
insert into produto values ('5601126900100',  'Batatas', 'Once you pop you cant stop');

insert into tem_categoria values ('5601126900010',  'Chocolates');
insert into tem_categoria values ('5601126900020',  'Aguas');
insert into tem_categoria values ('5601126900030',  'Aguas');
insert into tem_categoria values ('5601126900040',  'Lays');
insert into tem_categoria values ('5601126900050',  'Ruffles');
insert into tem_categoria values ('5601126900060',  'Batatas');
insert into tem_categoria values ('5601126900070',  'Chocolates');
insert into tem_categoria values ('5601126900080',  'Lays');
insert into tem_categoria values ('5601126900090',  'Ruffles');
insert into tem_categoria values ('5601126900100',  'Batatas');

insert into IVM values ('1111', 'Continente');
insert into IVM values ('2222', 'Lidl');
insert into IVM values ('3333', 'Pingo doce');
insert into IVM values ('4444', 'Auchan');
insert into IVM values ('5555', 'Continente');
insert into IVM values ('6666', 'Auchan');

insert into ponto_de_retalho values ('Taguspark','Lisboa','Oeiras');
insert into ponto_de_retalho values ('Alameda','Lisboa','Lisboa');
insert into ponto_de_retalho values ('Baixa','Porto','Porto');
insert into ponto_de_retalho values ('Chiado','Lisboa','Lisboa');
insert into ponto_de_retalho values ('Rossio','Lisboa','Lisboa');
insert into ponto_de_retalho values ('Marques','Lisboa','Lisboa');

insert into instalada_em values ('1111', 'Continente','Taguspark');
insert into instalada_em values ('2222', 'Lidl','Alameda');
insert into instalada_em values ('3333', 'Pingo doce','Baixa');
insert into instalada_em values ('4444', 'Auchan','Chiado');
insert into instalada_em values ('5555', 'Continente','Rossio');
insert into instalada_em values ('6666', 'Auchan','Marques');


insert into prateleira values (1, '1111', 'Continente',40,'Chocolates');
insert into prateleira values (1, '2222', 'Lidl',60,'Aguas');
insert into prateleira values (2, '3333', 'Pingo doce',80,'Batatas');
insert into prateleira values (2, '4444', 'Auchan',100,'Chocolates');
insert into prateleira values (3, '5555', 'Continente',60,'Aguas');
insert into prateleira values (3, '6666', 'Auchan',40,'Batatas');

insert into planograma values ('5601126900050', 1, '1111', 'Continente', 4, 20, 1);
insert into planograma values ('5601126900060', 2, '3333', 'Pingo doce', 3, 15, 2);
insert into planograma values ('5601126900080', 2, '4444', 'Auchan', 5, 25, 3);
insert into planograma values ('5601126900030', 3, '5555', 'Continente', 4, 20, 4);
insert into planograma values ('5601126900070', 3, '6666', 'Auchan', 2, 10, 5);
insert into planograma values ('5601126900040', 1, '2222', 'Lidl', 4, 20, 6);

insert into retalhista values ('111111', 'Anastacio');
insert into retalhista values ('222222', 'Cristiano Ronaldo');
insert into retalhista values ('333333', 'Silvino');
insert into retalhista values ('444444', 'Zezinho');
insert into retalhista values ('555555', 'Albertina');

insert into responsavel_por values('Ruffles','222222','1111','Continente');
insert into responsavel_por values('Batatas','111111','3333', 'Pingo doce');
insert into responsavel_por values('Lays','333333','4444', 'Auchan');
insert into responsavel_por values('Aguas','444444','5555', 'Continente');
insert into responsavel_por values('Chocolates','555555','6666', 'Auchan');
insert into responsavel_por values('Lays','222222','2222', 'Lidl');
insert into responsavel_por values('Chocolates','222222','2222', 'Lidl');
insert into responsavel_por values('Aguas','222222','2222', 'Lidl');

insert into evento_reposicao values ('5601126900050', 1 , '1111', 'Continente','2020-03-15 11:20:30',5,'222222' );
insert into evento_reposicao values ('5601126900060', 2,  '3333', 'Pingo doce', '2020-07-04 14:20:40',5, '111111');
insert into evento_reposicao values ('5601126900080', 2,  '4444', 'Auchan', '2020-11-08 11:45:20',5,'333333');
insert into evento_reposicao values ('5601126900030', 3,  '5555', 'Continente', '2020-09-10 12:20:15',5,'444444');
insert into evento_reposicao values ('5601126900070', 3,  '6666', 'Auchan', '2020-02-13 13:17:25',5,'555555');
insert into evento_reposicao values ('5601126900040', 1,  '2222', 'Lidl', '2020-02-13 14:12:12',5,'222222');
