#include <stdio.h>
#include <mysql.h>
#include <string.h>

void Insert(char* buf)
{
    //printf("%s\n",mysql_get_client_info());
    MYSQL* sql = mysql_init(NULL);
    if(mysql_real_connect(sql,"127.0.0.1","root","xzzcjm1314","student",0,NULL,0) == 0)
    {
        printf("mysql connect err\n");
        exit(1);
    }

    int id = 0;
    char name[20];
    char sex[2];
    int age;
    char order[64];
    //id=45&name=jhggj&sex=m&age=12
    strtok(buf,"=");
    id = atoi(strtok(NULL,"&"));
    strtok(NULL,"=&");
    strcpy(name, strtok(NULL,"=&"));
    strtok(NULL,"=&");
    strcpy(sex, strtok(NULL,"=&"));
    strtok(NULL,"=&");
    age = atoi(strtok(NULL,"=&"));
    
    sprintf(order,"insert into stu values(%d,\"%s\",\"%s\",%d)",id,name,sex,age);
    mysql_query(sql,order);
    mysql_close(sql);
    printf("insert success\n");
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
        Insert(buf);
    }
 

    return 0;
}
