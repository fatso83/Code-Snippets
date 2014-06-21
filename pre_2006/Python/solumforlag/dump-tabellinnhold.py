#coding=utf-8
#Dump dbinnhold til csv-filer

import MySQLdb
import sys

def separated_string(tuple_object, separator):
	s = ""
	for elem in tuple_object[:-1]:
		s+= str(strip_None(elem)) + separator
	return s + str(strip_None(tuple_object[-1]))
	
def strip_None(s):
	if s is None: 
		return ""
	else: return s
	
def strip_newline(s):
	newstring = ""
	for char in s:
		if char != '\n':
			newstring += char
	return newstring

## main start ##
limit = 2**31
if len(sys.argv) == 2:
	limit = sys.argv[1]
			
db=MySQLdb.connect( host="secret.mysql.domain.com", user="secretUser", passwd="secretPW", db="secretDB" )

dbc = db.cursor()

#get table names
dbc.execute("SHOW TABLES")
result = dbc.fetchall()
tables = []
for row in result:
	tables.append(row[0])

for table_name in tables:
	dbc.execute("SELECT * FROM %s LIMIT %s" % (table_name,limit))
	column_names = []
	#The column name is in the first column of each row in the description
	for c in dbc.description:
		column_names.append(c[0])
		
	result = dbc.fetchall()
	
	#write data to file
	sep = "\t"
	csvfile = open(table_name+".csv",'w')
	csvfile.write(separated_string(column_names, sep )+"\n")
	
	for row in result:
		cleansed_string = strip_newline(separated_string(row, sep))
		csvfile.write(cleansed_string +"\n")
	
	csvfile.close()

db.close()
