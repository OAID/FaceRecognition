#include <log.h>


uint32_t g_LOG_config = (BIT(LOG_EMERG)|BIT(LOG_ERROR)|BIT(LOG_ALARM)|BIT(LOG_PERFIX));

char LOG_e_str[][30] = 
{
	"LOG_NULL",
    "LOG_VERB",
    "LOG_STAT",
    "LOG_INFO",
    "LOG_DEBUG",
    "LOG_CONFIG",
    "LOG_EVENT",
    "LOG_WARN",
    "LOG_ALARM",
    "LOG_ERROR",
    "LOG_EMERG",
    "LOG_PERFIX",
    "LOG_MAX",
    ""
};


void log_config_init() 
{ 
    char * temp = NULL; 
    int len = 0, index = 0; 
	char *log_config = NULL;
	
	if(BIT_GET(g_LOG_config, LOG_MAX))
		return;
	g_LOG_config |= BIT(LOG_MAX); 
	
    log_config = getenv("AID_LOG_CONFIG"); 
    if(log_config == NULL) 
         return; 
	 
    len = strlen(log_config); 
    temp =(char *)malloc(len + 3); 
    strcpy(temp, ":"); 
    strcat(temp, log_config); 
    strcat(temp, ":"); 
  
    for(index=LOG_NULL; index<LOG_MAX; index++) 
    { 
        char content[80]; 
        strcpy(content, ":"); 
        strcat(content, LOG_e_str[index]); 
        strcat(content, ":"); 
        if(strstr(temp,content)) 
        { 
            g_LOG_config |= BIT(index); 
        } 
    } 
	
}

void log_config_get(string &str) 
{ 
    int index = 0; 
    
    str = "";
    
    for(index=LOG_NULL; index<LOG_MAX; index++) 
    { 
        if(BIT_GET(g_LOG_config, index))
        {
            str += LOG_e_str[index];
            str += ":";
        }
    } 
	
}

void log_config_set(string &str) 
{ 
    int index = 0; 
    uint32_t config_data = 0;

    str = ":" + str + ":";
  
    for(index=LOG_NULL; index<LOG_MAX; index++) 
    { 
        char content[80]; 
        strcpy(content, ":"); 
        strcat(content, LOG_e_str[index]); 
        strcat(content, ":"); 
        if(strstr(str.data(),content)) 
        { 
            config_data |= BIT(index); 
        } 
    } 
     
    if(BIT_GET(g_LOG_config, LOG_MAX))
		config_data |= BIT(LOG_MAX);
    
    g_LOG_config = config_data;
    
}
