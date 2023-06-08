@echo off 
del mydb.db
g++ *.cpp -std=c++11 -o db -Wall -Wextra -O0 

if "%1" == "testsize" (
	db.exe sell.txt
) else (
	db.exe mydb.db 
)

