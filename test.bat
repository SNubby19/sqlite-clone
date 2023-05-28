@echo off 
del mydb.db
g++ *.cpp -o db

if "%1" == "testsize" (
	db.exe sell.txt
) else (
	db.exe mydb.db 
)

