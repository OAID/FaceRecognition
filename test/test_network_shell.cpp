#include <pthread.h>
#include <unistd.h>
#include <readline/readline.h>

#include <string.h>

#include "network_shell.hpp"

#include <malloc.h>


void test_cmd(int argc, char ** argv)
{
	for(int i=0;i<argc;i++)
	{
		printf("argv[%d]=%s\n",i,argv[i]);
	}


}

void * only_for_quit(void * arg)
{
	printf("will quit whole net shell\n");
	quit_network_shell(0);  
	return NULL;
}

void exit_cmd(int argc, char ** argv)
{

	pthread_t tr;

	pthread_create(&tr,NULL,only_for_quit,NULL);


}

int test_thread_mode=0;

int main(int argc, char * argv[])
{

	init_network_shell();

	register_network_shell_cmd("test",test_cmd,"test","this is a test command");
	register_network_shell_cmd("exit",exit_cmd,"test","this will exit whole net shell");


	if(test_thread_mode)
	{
		create_network_shell_thread("thread>",8080);
		sleep(-1);
	}
	else
	{
		run_network_shell("TEST>",8080);
	}

	return 0;
}
