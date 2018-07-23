#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/sendfile.h>
#include <pthread.h>

#define MAX 1024
#define HOME_PAGE "home.html"
int StartUp(int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		printf("sock error\n");
		exit(2);
	}

	int opt = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family  = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(port);

	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{
		printf("bind error\n");
		exit(3);
	}
	printf("bind seccess\n");

	if(listen(sock,5)<0)
	{
		printf("listen error\n");
		close(sock);
		exit(4);
	}
	return sock;
}

int cgi = 0;

//获取一行并将 \n \r\n \r 转为\n
int GetLine(int client_fd,char buf[],int size)
{
	int i = 0;
	char ch = 'A';
	while(ch != '\n' && i < size-1)
	{
		ssize_t s = recv(client_fd,&ch,1,0);
		if(s < 0)
		{
			printf("read error\n");
			break;
		}
		if(ch == '\r')
		{
			recv(client_fd,&ch,1,MSG_PEEK);
			if(ch == '\n')
				recv(client_fd, &ch, 1,0);
			else
				ch = '\n';
		}
		buf[i++] = ch;
	}
	buf[i] = '\0';
	return i;
}

void ClearHead(int client_fd)
{
	char buf[1024];
	while(strcmp(buf,"\n")!= 0)
	{
		GetLine(client_fd,buf,sizeof(buf));	
	}
}

void Echo_err(int errcode)
{
	switch(errcode)
	{
		case 404:
			break;
		case 501:
			break;
		case 503:
			break;
		default:
		break;
	}
}

int Exe_cgi(int sock,char path[],char method[],char* query_string)
{
	char line[MAX];
	int content_length = -1;
	char method_env[MAX];
	char query_string_env[MAX];
	char content_length_env[MAX/8];
	if(strcasecmp(method , "GET") == 0)
	{
		ClearHead(sock);
	}
	else//POST
	{
		//get the arguments
		while(strcmp(line,"\n"))
		{
			GetLine(sock,line,MAX);
			if(strncmp(line,"Content-Length: ",16)==0)
				content_length = atoi(line+16);
		}
		if(content_length == -1)
			return 404;
	}
	//Get:path query_string  POST:path content_length
	int input[2];
	int output[2];

	pipe(input);
	pipe(output);

	pid_t pid = fork();
	if(pid < 0)
		return 404;
	else if(pid == 0)//child
	{
		close(input[1]);
		close(output[0]);
		
		dup2(output[1],1);
		dup2(input[0],0);
		sprintf(method_env,"METHOD=%s",method);
		putenv(method_env);
		if(strcasecmp(method,"GET")==0)
		{
			sprintf(query_string_env,"QUERY_STRING=%s",query_string);
			putenv(query_string_env);
		}
		else
		{
			sprintf(content_length_env,"CONTENT_LENGTH=%d",content_length);
			putenv(content_length_env);
		}
		execl(path,path,NULL);
		exit(1);
	}
	else
	{
		close(input[0]);
		close(output[1]);
		
	    char line[MAX];
	    sprintf(line,"HTTP/1.0 200 OK\r\n");
	    send(sock,line,strlen(line),0);
	    sprintf(line,"Content-Type:text/html;charset=ISO-8859-1\r\n");
	    send(sock,line,strlen(line),0);
	    sprintf(line,"\r\n");
	    send(sock,line,strlen(line),0);

		char buf[content_length];
		char c;
		if(strcasecmp(method,"POST")==0)
		{
			recv(sock,buf,content_length,0);
			write(input[1],buf,content_length);
		}
		while(read(output[0],&c,1)>0)
			send(sock,&c,1,0);
		waitpid(pid, NULL, 0);

		close(input[1]);
		close(output[0]);
	}

}
int Echo_www(int sock,char* path,int size)
{
	ClearHead(sock);
	int fd = open(path,O_RDONLY);
	if(fd < 0)
		return 404;
	
	char line[MAX];
	sprintf(line,"HTTP/1.0 200 OK\r\n");
	send(sock,line,strlen(line),0);
	sprintf(line,"Content-Type:text/html;charset=ISO-8859-1\r\n");
	send(sock,line,strlen(line),0);
	sprintf(line,"\r\n");
	send(sock,line,strlen(line),0);

	sendfile(sock,fd,NULL, size);
	return 200;
}
void* handler(void* arg)
{
	int sock = (int)arg;
	char line[MAX];
	char method[MAX/32];
	char url[MAX];
	char path[MAX];
	char* query_string;
	int errCode = 200;
	GetLine(sock,line,MAX);
	printf("get a line is :%s",line);
	int i = 0;
	int j = 0;
	while(i < sizeof(method)-1 && j < sizeof(line) && !isspace(line[j]))
	{
		method[i] = line[j];
		i++;j++;
	}
	method[i] = '\0';
	printf("method is : %s\n",method);
	while((j<sizeof(line)) && isspace(line[j]))
		j++;
	
	i = 0;
	while(i < sizeof(url) && j<sizeof(line) && !isspace(line[j]))
	{
		url[i] = line[j];
		i++; j++;
	}
	url[i] = '\0';
	printf("url is : %s\n",url);
	if(strcasecmp(method,"GET")== 0)
	{
		query_string = url;
		while(*query_string != '\0')
		{
			if(*query_string == '?')
			{
				*query_string = '\0';
				query_string ++;
				cgi = 1;
				break;
			}
			query_string ++;
		}
	}
	else if(strcasecmp(method,"POST"))
	{
		cgi = 1;
	}
	else
	{
		errCode = 404;
		goto end;
	}
	//url is clean
	sprintf(path,"myroot%s",url);
	if(path[strlen(path)-1] == '/')
		strcat(path,HOME_PAGE);
	printf("path is : %s\n",path);
	struct stat st;//get information of a file
	if(stat(path,&st)<0)
	{
		errCode = 404;
		goto end;
	}
	else//whether it is a directory or has x mode
	{
		if(S_ISDIR(st.st_mode))
			strcat(path,HOME_PAGE);
		else if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
			cgi = 1;
		else
		{}
		if(cgi)
		{
			Exe_cgi(sock,path,method,query_string);
		}
		else
		{
			errCode = Echo_www(sock,path,st.st_size);
		}
	}

	end:
		if(errCode != 200)
		{
			Echo_err(errCode);
		}
}

int main(int argc, char* argv[])
{
	if(2 != argc)
	{
		printf("Usage:\n\t %s port]\n",argv[0]);
		return 1;
	}

	int listen_sock = StartUp(atoi(argv[1]));
	printf("startUp end\n");
	for(;;)
	{
		struct sockaddr_in client_addr;
		socklen_t len = sizeof(client_addr);
		int client_fd = accept(listen_sock,(struct sockaddr*)&client_addr, &len);
		printf("accecpt seccess client_fd is %d\n",client_fd);
		if(client_fd < 0)
		{
			printf("accept error\n");
			continue;
		}
		pthread_t tid = 0;
		
		pthread_create(&tid,NULL,handler,(void*)client_fd);
		pthread_detach(tid);
	}
	return 0;
}
