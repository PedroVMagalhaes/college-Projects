#!/usr/bin/python3

from wsgiref.handlers import CGIHandler
from flask import Flask
from flask import render_template, request

## Libs postgres
import psycopg2
import psycopg2.extras

app = Flask(__name__)

## SGBD configs
DB_HOST="db.tecnico.ulisboa.pt"
DB_USER="ist174224" 
DB_DATABASE=DB_USER
DB_PASSWORD="mfpw4568"
DB_CONNECTION_STRING = "host=%s dbname=%s user=%s password=%s" % (DB_HOST, DB_DATABASE, DB_USER, DB_PASSWORD)


## Runs the function once the root page is requested.
## The request comes with the folder structure setting ~/web as the root
@app.route('/')
def menu():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    query = "SELECT * FROM categoria;"
    cursor.execute(query)
    return render_template("index.html", cursor=cursor)
  except Exception as e:
    return str(e) #Renders a page with the error.
  finally:
    cursor.close()
    dbConn.close()


@app.route('/categoria')
def categorias():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    query = "SELECT * FROM categoria;"
    cursor.execute(query)
    return render_template("categoria.html", cursor=cursor)
  except Exception as e:
    return str(e) #Renders a page with the error.
  finally:
    cursor.close()
    dbConn.close()

@app.route('/categoriasimples')
def categorias_simples():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    query = "SELECT * FROM categoria_simples;"
    cursor.execute(query)
    return render_template("categoriasimples.html", cursor=cursor)
  except Exception as e:
    return str(e) #Renders a page with the error.
  finally:
    cursor.close()
    dbConn.close()


@app.route('/criacategoria')
def cria_categoria():
  try:
    return render_template("criacategoria.html", params=request.args)
  except Exception as e:
    return str(e)


@app.route('/criacategoriasimples')
def cria_categoriasimples():
  try:
    return render_template("criacategoriasimples.html", params=request.args)
  except Exception as e:
    return str(e)    

@app.route('/removecategoria')
def remove_categoria():
  try:
    return render_template("removecategoria.html", params=request.args)
  except Exception as e:
    return str(e)    


@app.route('/updatecategoria', methods=["POST"])
def insert_categoria_intoDB():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    query = "INSERT INTO categoria VALUES (%s);"
    data = (request.form["cat"],)
    cursor.execute(query,data)
    return render_template("categoria.html", cursor=cursor)
  except Exception as e:
    return str(e) 
  finally:
    dbConn.commit()
    cursor.close()
    dbConn.close()

@app.route('/updatecategoriasimples', methods=["POST"])
def insert_categoria_simples_intoDB():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    query = "INSERT INTO categoria_simples VALUES (%s);"
    data = (request.form["cat"],)
    cursor.execute(query,data)
    return render_template("categoria.html", cursor=cursor)
  except Exception as e:
    return str(e) 
  finally:
    dbConn.commit()
    cursor.close()
    dbConn.close()    

@app.route('/updateremove', methods=["POST"])
def remove_categoria_fromDB():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    query = "DELETE FROM categoria WHERE nome=%s;"
    data = (request.form["cat"],)
    cursor.execute(query,data)
    return render_template("categoria.html", cursor=cursor)
  except Exception as e:
    return str(e) 
  finally:
    dbConn.commit()
    cursor.close()
    dbConn.close()


CGIHandler().run(app)

