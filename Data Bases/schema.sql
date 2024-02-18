drop table categoria cascade;
drop table categoria_simples cascade;
drop table super_categoria cascade;
drop table tem_outra cascade;
drop table produto cascade;
drop table tem_categoria cascade;
drop table IVM cascade;
drop table ponto_de_retalho cascade;
drop table instalada_em cascade;
drop table prateleira cascade;
drop table planograma cascade;
drop table retalhista cascade;
drop table responsavel_por cascade;
drop table evento_reposicao cascade;


create table categoria (
  nome varchar(100) not null,
  primary key (nome)
);

create table categoria_simples (
  nome varchar(100) not null,
  primary key (nome),
  foreign key (nome) references categoria(nome) on delete cascade
);

create table super_categoria (
  nome varchar(100) not null,
  primary key (nome),
  foreign key (nome) references categoria(nome) on delete cascade
);

create table tem_outra (
  super_categoria varchar(100) not null,
  categoria varchar(100) not null,
  primary key (super_categoria, categoria),
  foreign key (super_categoria) references super_categoria(nome) on delete cascade,
  foreign key (categoria) references categoria(nome) on delete cascade,
  check (super_categoria != categoria)
);

create table produto (
  ean char(13) not null,
  cat varchar(100) not null,
  descr varchar(100),
  primary key (ean),
  foreign key (cat) references categoria(nome) on delete cascade
);

create table tem_categoria (
    ean varchar(100),
    nome varchar(100),
    foreign key (ean) references produto(ean),
    foreign key (nome) references categoria(nome)
);

create table IVM (
    num_serie varchar(100) not null,
    fabricante varchar(100) not null,
    primary key (num_serie,fabricante)
);

create table ponto_de_retalho (
    nome varchar(100) not null,
    distrito varchar(100),
    concelho varchar(100),
    primary key (nome)
);

create table instalada_em(
    num_serie varchar(100) not null,
    fabricante varchar(100) not null,
    local varchar(100),
    foreign key (num_serie,fabricante) references IVM(num_serie,fabricante),
    foreign key (local) references ponto_de_retalho(nome),
    primary key (num_serie,fabricante)
);



create table prateleira (
  nro int not null,
  num_serie varchar(100) not null,
  fabricante varchar(100) not null,
  altura int not null,
  nome varchar(100) not null,
  primary key (nro, num_serie, fabricante),
  foreign key (num_serie,fabricante) references IVM(num_serie,fabricante),
  foreign key (nome) references categoria(nome) 
);

create table planograma (
  ean char(13) not null,
  nro int not null,
  num_serie varchar(100) not null,
  fabricante varchar(100) not null,
  faces int,
  unidades int,
  loc int,
  primary key (ean,nro, num_serie, fabricante),
  foreign key (ean) references produto(ean),
  foreign key (nro, num_serie, fabricante) references prateleira(nro,num_serie,fabricante)
);

create table retalhista (
  tin varchar(20) not null,
  nome varchar(100),
  primary key (tin)
);

create table responsavel_por(
    nome_cat varchar(100),
    tin varchar(100),
    num_serie varchar(100) not null,
    fabricante varchar(100) not null,
    foreign key (num_serie,fabricante) references IVM(num_serie,fabricante),
    foreign key (tin) references retalhista(tin),
    foreign key (nome_cat) references categoria(nome)
);


create table evento_reposicao(
    ean varchar(100) not null,
    nro int not null,
    num_serie varchar(100) not null,
    fabricante varchar(100) not null,
    instante timestamp,
    unidades int,
    tin varchar(100),
    primary key (ean,nro,num_serie,fabricante, instante),
    foreign key (ean,nro,num_serie,fabricante) references planograma(ean,nro,num_serie,fabricante),
    foreign key (tin) references retalhista(tin)
);

