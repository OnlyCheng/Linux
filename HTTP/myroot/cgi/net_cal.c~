#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void cal(char* data)
{
	int first=0;
	int second=0;
	
	printf("<html>\n");
	sscanf(data,"firstnumber=%d&secondnumber=%d",&first,&second);
	printf("<h3>%d+%d=%d</h3>\n",first,second,first+second);
	printf("<h3>%d-%d=%d</h3>\n",first,second,first-second);
	printf("<h3>%d*%d=%d</h3>\n",first,second,first*second);
	printf("<h3>%d/%d=%d</h3>\n",first,second,first/second);
	printf("<h3>%d%%%d=%d</h3>\n",first,second,first%second);
	printf("</html>\n");
}

int main()
{
	char method[32];
	char buff[1024];
	if(getenv("METHOD")!=NULL)
	{
		strcpy(method,getenv("METHOD"));
		if(strcasecmp(method,"GET") == 0)
		{
			strcpy(buff,getenv("QUERY_STRING"));	
		}
		else if(strcasecmp(method,"POST")==0)
		{
			int content_length = atoi(getenv("CONTENT_LENGTH"));
			char c;
			int i = 0;
			for(;i<content_length;i++)
			{
				read(0,&c,1);
				buff[i] = c;
			}
			buff[i] = '\0';
		}
		else
		{

		}
		cal(buff);
	}
	return 0;
}
