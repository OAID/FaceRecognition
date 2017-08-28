#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <sched.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <poll.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>

extern "C" {
#include "telnet_buf.h"
}

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "network_shell.hpp"

static int saved_stdin_fd;
static int saved_stdout_fd;
static int saved_stderr_fd;
static int g_listen_fd;
static int g_conn_fd=-1;
static volatile int shell_quit_flag=0;
static volatile int shell_quit_done=0;
static volatile int terminal_server_done=0;
static int master_fd=-1;
static int slave_fd=-1;
static char slave_pts_name[128];
static int verbose=0;

static std::vector<std::string> help_list;

static std::map<std::string,net_shell_cmd_t> g_cmd_set;
static const char * bye_banner="Bye, have a nice day!\n";

/* test networking connection */

static int check_net_connection(int fd)
{
	struct msghdr msg;
	struct iovec iovec;
	int len;
	char buf;

	iovec.iov_base = &buf;
	iovec.iov_len = 1;
	msg.msg_name = NULL;
	msg.msg_namelen=0;
	msg.msg_iov = &iovec;
	msg.msg_iovlen = 1;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;

	len = recvmsg(fd, &msg, MSG_PEEK|MSG_DONTWAIT);

	if (len <= 0 && errno!=EAGAIN) {
		return 0;
	}


	return  1;

}



static int setup_listen_socket(unsigned int bind_addr, int port)
{
	int fd;
	int on = 1;

	struct sockaddr_in srv_addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);

	if (fd < 0)
		return -1;

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on))
			< 0) {
		printf("setsockopt error\n");
		goto error;
	}

	memset(&srv_addr, 0x0, sizeof(srv_addr));

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = bind_addr;
	srv_addr.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
		printf("bind failed at port [%d]\n", port);
		goto error;
	}

	if (listen(fd, 10) < 0) {
		printf("listen\n");
		goto error;
	}

	return fd;

error:
	close(fd);
	return -1;
}



static int init_network_server(int port)
{
	g_listen_fd=setup_listen_socket(0,port);

	if(g_listen_fd<0)
		return -1;

	return 0;
}


static int  wait_connection(void)
{
	struct sockaddr_in clnt_addr;
	unsigned int clnt_addr_len=sizeof(clnt_addr);


	if(g_conn_fd>0 && check_net_connection(0))
		return 0;

	g_conn_fd=accept(g_listen_fd,(struct sockaddr *)&clnt_addr,
			&clnt_addr_len);  

	if(g_conn_fd<0)
		return -1;

	return 2;
}


static void do_quit_command(int argc, char ** arg)
{

	write(1,bye_banner,strlen(bye_banner)+1);
	usleep(5000);
	close(g_conn_fd);
	g_conn_fd=-1;
}


static void do_help_command(int argc, char ** arg)
{
	for(int i=0;i<help_list.size();i++)
	{
		std::cout<<help_list[i]<<std::endl;
	}
}



static void init_command_shell(void)
{
	g_cmd_set["quit"]=do_quit_command;
	g_cmd_set["Q"]=do_quit_command;
	g_cmd_set["q"]=do_quit_command;
	g_cmd_set["help"]=do_help_command;
	g_cmd_set["h"]=do_help_command;

	help_list.push_back("help/h          \t\tshow this message");
	help_list.push_back("quit/Q/q        \t\tquit shell. You can reconnect later");

}


static void handle_command(const char * cmd, char * arg)
{
	int n=0;
	int ptr_n=0;
	int step=8;
	char **argv;
	char *p;

	if(g_cmd_set.find(cmd)==g_cmd_set.end())
	{
		printf("%s: no such command\n",cmd);
		return ; 
	}


	/*prepare the argment vector */

	p=arg;
	argv=(char **)malloc(sizeof(char *)*step);
	n=step;

	argv[ptr_n++]=(char *)cmd;

	while(*p)
	{
		while(*p && *p==' ') p++;

		if(ptr_n==n)
			argv=(char **)realloc(argv,(n+step)*sizeof(char *));

		argv[ptr_n++]=p;

		while(*p&&*p!=' ')p++;

		if(*p==' ') *p++=0x0;
	}


	/* the command disptach */
	net_shell_cmd_t func=g_cmd_set[cmd];

	func(ptr_n,argv);

	free(argv);

}


/* have to replace the getc in readline lib, as it use pselect to disable all signals */

int net_shell_getc (FILE *stream)
{
	int fd=fileno(stream);

	int ret;

	unsigned char c;

	while(1)
	{
		ret=read(fd,&c,sizeof(c));

		if(ret==1)
			return c;

		if(ret==0)
			return EOF;

		/* ret<0 */

		if(errno!=EINTR)
			return EOF;

		fprintf(stderr,"SIGNAL RECEIVED!");
	}

}

static int command_shell(const char * prompt)
{
	char *  line;
	char * cmd;

	line=readline(prompt);

	if(line==NULL)
		return -1;


	if(line[0])
	{
		add_history(line);
		cmd=strtok(line," \n");

		if(cmd)
			handle_command(cmd,cmd+strlen(cmd)+1);
	}

	free(line);

	return 0;
}




static void notify_shell_exit(void)
{
	shell_quit_flag=1;

	close(master_fd);
	usleep(1000);
	close(1);
	close(slave_fd);
	close(0);

	while(!shell_quit_done);

}

typedef void *(*pthread_func_t)(void *);

void quit_network_shell(int in_shell)
{
	pthread_t tr;

	if(!in_shell)
		notify_shell_exit();
	else
		pthread_create(&tr,NULL,(pthread_func_t)notify_shell_exit,NULL);  

}

int setup_terminal(void)
{
	/* create the master first */
	int fdm;
	char * ptr;

	if((fdm=posix_openpt(O_RDWR))<0)
		return -1;

	if (grantpt(fdm) < 0 ||
			unlockpt(fdm) <0)
	{
		close(fdm);
		return -1;
	}

	ptr=ptsname(fdm);

	strncpy(slave_pts_name,ptr,128);
	slave_pts_name[127]=0;

	slave_fd=open(slave_pts_name,O_RDWR);

	if(slave_fd<0)
	{
		close(fdm);
		return -1;
	}

	master_fd=fdm;

	saved_stdin_fd=dup(0);
	saved_stdout_fd=dup(1);
	saved_stderr_fd=dup(2);

	dup2(slave_fd,0);

	return 0;
}


void * terminal_server(void * arg)
{
	struct pollfd fds[2];
	char * buf;
	int ret;

	int buf_len=get_max_data_batch_len();

	fds[0].fd=master_fd;
	fds[0].events=POLLIN;

	buf=(char *)malloc(buf_len);

	if(buf==NULL)
	{
		fprintf(stderr,"failed to allocate buffer for networking\n");
		return NULL;
	}


	while(!shell_quit_flag)
	{
		const char * init_cmd;
		int init_cmd_len;

		if(wait_connection()<0)
			continue;

		/* clinet connected */

		fds[1].fd=g_conn_fd;
		fds[1].events=POLLIN;

		/* redirect stdout */
		dup2(slave_fd,1);

		/* send the inital telnet command */
		init_cmd=get_init_cmd_buf();
		init_cmd_len=get_init_cmd_len();

		write(g_conn_fd,init_cmd,init_cmd_len);

		/* send '\n' to readline to generate prompt */
		write(master_fd,"\n",1);

		while(!shell_quit_flag)
		{
			ret=poll(fds,2,-1);

			if(ret==0) 
				continue;

			if(ret<0)
				break;

			if(fds[1].revents&(POLLERR|POLLHUP|POLLNVAL))
			{
				if(verbose)
					fprintf(stderr,"client disconnected\n");
				break;
			}

			if(fds[0].revents&(POLLERR|POLLHUP|POLLNVAL))
			{
				if(verbose)
					fprintf(stderr,"pseudo terminal destroyed\n");
				break;
			}


			if(fds[0].revents&POLLIN)
			{
				ret=read(fds[0].fd,buf,buf_len);

				buf[ret]=0;

				if(ret<=0)
					break;

				write(fds[1].fd,buf,ret);
			}

			if(fds[1].revents&POLLIN)
			{
				ret=read(fds[1].fd,buf,buf_len);

				if(ret<=0)
					break;

				save_data_to_buffer(buf,ret);

				ret=get_parsed_data(buf,buf_len);


				if(ret>0) 
					write(fds[0].fd,buf,ret);
			}
		}

		/* restore stdout */

		dup2(saved_stdout_fd,1);


		/* 
		   if user disconnect, g_conn_fd is closed already
		   here is for un-expected networking broken
		 */

		if(g_conn_fd>0)
			close(g_conn_fd);

	}

	free(buf);

	terminal_server_done=1;


	return NULL;
}

int run_network_shell(const char * prompt,int port)
{
	pthread_t tr;

	if(init_network_server(port)<0)
	{
		fprintf(stderr,"failed to bind port: %d\n",port);
		return -1;
	}

	/* replace original getc function */
	rl_getc_function=net_shell_getc;

	if(setup_terminal()<0)
	{
		fprintf(stderr,"cannot create pseduo terminal\n");
		return -1;
	}


	/* launch the terminal server thread */
	pthread_create(&tr,NULL,terminal_server,NULL);

	/* process command from network */
	while(!shell_quit_flag)
	{
		command_shell(prompt);

	}

	/* wait terminal sever stop */

	while(!terminal_server_done);

	shell_quit_done=1;

	close(g_listen_fd);

	/* restore stdin */
	dup2(saved_stdin_fd,0);

	close(saved_stdin_fd);
	close(saved_stdout_fd);
	close(saved_stderr_fd);


	return 0;

}

static char * saved_prompt;
static int  saved_port;

static void * shell_thread(void * arg)
{
	run_network_shell(saved_prompt,saved_port);

	free(saved_prompt);

	return NULL;    
}

int create_network_shell_thread(const char * prompt,int port)
{
	pthread_t tr;
	int len=strlen(prompt);

	saved_prompt=(char *)malloc(len+1);

	strcpy(saved_prompt,prompt);
	saved_port=port;

	int ret=pthread_create(&tr,NULL,shell_thread,NULL);

	if(ret<0)
		return -1;

	return 0;

}

int register_network_shell_cmd(const std::string& name, net_shell_cmd_t func, const std::string& cmd_line, const std::string& help_msg)
{
	char  buf[1204];

	if(help_msg.size()==0)
	{
		fprintf(stderr,"register command: %s failed --- help msg cannot be empty\n",name.c_str());
		return -1;
	}

	g_cmd_set[name]=func;

	int ret=snprintf(buf,1024,"%-32s%s",cmd_line.c_str(),help_msg.c_str());

	if(ret==1024)
		ret--;

	buf[ret]=0x0;


	help_list.push_back(buf);

	/* sort list */

	std::sort(help_list.begin(),help_list.end(),[](const std::string& a, const std::string& b){
			return a<=b;
			});

	return 0;
}

void init_network_shell(void)
{
	init_command_shell();
}

