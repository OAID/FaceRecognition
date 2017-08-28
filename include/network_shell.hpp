#ifndef __NETWORK_SHELL_HPP__
#define __NETWORK_SHELL_HPP__

#include <string>


void init_network_shell(void);

typedef void (*net_shell_cmd_t)(int, char ** arg);

/* the generate msg will be formate as: sprintf("%-32s%s",name,help_msg) */

int register_network_shell_cmd(const std::string& name, net_shell_cmd_t func,const std::string&cmd_line, const std::string& help_msg);

/* 
 let run_network_shell quit.
  in_shell -- 0
    a blocking function: when it returns, all created threads have terminated  and resources are released
  in_shell --- 1
    this function is called in the same thread that shell runs. It is an async function: 
    a new thread will be created to execute the block parts.
*/

void quit_network_shell(int in_shell);

/* a dead loop to handle user input and to execute registered commands */

int run_network_shell(const char * prompt,int port);


/* create a thread inside ...*/
int create_network_shell_thread(const char * prompt,int port);


#endif
