#include <stdio.h>
#include <string.h>
#include <mysql.h>

void Select(char* buf)
{
    //printf("%s\n",mysql_get_client_info());
    MYSQL* sql = mysql_init(NULL);
    if(mysql_real_connect(sql,"127.0.0.1","root","xzzcjm1314","student",0,NULL,0) == 0)
    {
        printf("mysql connect err\n");
        exit(1);
    }

    char* p = "select * from stu";
    mysql_query(sql,p);

    MYSQL_RES* result = mysql_store_result(sql);
    int row = mysql_num_rows(result);
    int col = mysql_num_fields(result);
    MYSQL_FIELD* field  = mysql_fetch_fields(result);
    int i = 0;
    int j = 0;
    for(; i<col; i++)
        printf("%s\t",field[i].name);
    printf("<br>");
    MYSQL_ROW lines;
    for(i = 0; i<row; i++)
    {
        lines = mysql_fetch_row(result);//huo qu yi hang
        for(j=0; j<col; j++)
            printf("%s\t\t",lines[j]);
        printf("<br>");

    }

    free(result);
    close(sql);
}

int main()
{
    char buf[1024];
    char method[32];
    int content_length = 0;

    if(getenv("METHOD") != NULL)
    {
        strcpy(method,getenv("METHOD"));
        if(strcasecmp(method,"GET")==0)//GET
            strcpy(buf,getenv("QUERY_STRING"));
        else if(strcasecmp(method,"POST") == 0)
        {
            content_length = atoi(getenv("CONTENT_LENGTH"));
            char c;
            int i = 0;
            for(;i<content_length; i++)
            {
                read(0,&c,1);
                buf[i] = c;
            }
            buf[i] = '\0';
        }
        else{}
        Select(buf);
    }

    return 0;
}
