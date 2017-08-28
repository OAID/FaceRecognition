#ifndef __TELNET_BUF_H__
#define __TELNET_BUF_H__

/* get the data removed IAC commands, return parsed data length*/
int  get_parsed_data(char * output, int output_len);

/* the maximum data one time can save to parser buf */
int get_max_data_batch_len(void);

/* save the data receiving from network to parser buffer */
void save_data_to_buffer(char * data, int len);

/* get the initial setting command */

const char * get_init_cmd_buf(void);
int get_init_cmd_len(void);


#endif
