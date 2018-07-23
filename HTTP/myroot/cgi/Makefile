.PHONY:all
all:insert select

insert:insert.c
	gcc -o insert insert.c -I./../mysql_lib/include -L./../mysql_lib/lib -lmysqlclient
select:select.c
	gcc -o select select.c -I./../mysql_lib/include -L./../mysql_lib/lib -lmysqlclient

.PHONY:clean
clean:
	rm -f insert select
