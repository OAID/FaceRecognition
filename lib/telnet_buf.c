#include <stdio.h>
#include <unistd.h>

#include <arpa/telnet.h>

#define TELNET_BUF_LEN 1024
#define ADD_RING_PTR(a,b) ((a+b)%TELNET_BUF_LEN)

struct ring_buf
{
	unsigned char data[TELNET_BUF_LEN];
	unsigned int read_ptr;
	unsigned int write_ptr;

};



static const char init_iac_cmd[]={
	IAC, DO, TELOPT_LINEMODE,
	IAC, DONT, TELOPT_ECHO,
	IAC, WILL, TELOPT_ECHO,
	IAC, DO, TELOPT_BINARY,
};

static struct ring_buf g_ring_buf;
static int ring_buf_inited=0;

static void init_ring_buf(void)
{
	struct ring_buf * p_buf=&g_ring_buf;

        if(ring_buf_inited)
           return;

         ring_buf_inited=1;

        p_buf->read_ptr=0;
        p_buf->write_ptr=0;
}

static int find_iac_se(int start_off, int left_len)
{
	int n=start_off;
	unsigned char c;
	struct ring_buf * p_buf=&g_ring_buf;

	/* search for IAC first */
	c=p_buf->data[ADD_RING_PTR(p_buf->read_ptr,start_off)];

        do {
	   while(left_len>0)
	   {
		if (c==IAC)
			break;

		n++;
		c=p_buf->data[ADD_RING_PTR(p_buf->read_ptr,n)];
		left_len--;
	   }

	   if(left_len<1)
		return -1;

	   n++;
	   c=p_buf->data[ADD_RING_PTR(p_buf->read_ptr,n)];
	   left_len--;

	  }while(c!=SE && left_len>1);

        if(c!=SE)
            return -1;

	return n;
}

int  get_parsed_data(char * output, int output_len)
{
	int out_ptr=0;
	int n;
	unsigned char c;
        struct ring_buf * p_buf=&g_ring_buf;
	unsigned int left_len=p_buf->write_ptr-p_buf->read_ptr;

	while(out_ptr<output_len && left_len)
	{
		c=p_buf->data[p_buf->read_ptr];


		if(c!= IAC)
		{
			p_buf->read_ptr=ADD_RING_PTR(p_buf->read_ptr,1);
			left_len--;

			if(c=='\r')
			{
				output[out_ptr++]='\n';

				/* if next character is still '\n', skip it */

				if( p_buf->data[p_buf->read_ptr] == '\n')
				{
					p_buf->read_ptr=ADD_RING_PTR(p_buf->read_ptr,1);
					left_len--;
				}
				continue;
			}

			output[out_ptr++]=c;

			continue;
		}


		/* handle IAC cases */

		if(left_len==1)  //at least two bytes
			break;

		c=p_buf->data[ADD_RING_PTR(p_buf->read_ptr,1)];

		if(c==IAC)  
		{
			//the real IAC character 
			left_len-=2;
			p_buf->read_ptr=ADD_RING_PTR(p_buf->read_ptr,2);
			output[out_ptr++]=c;

			continue;
		}

		if(left_len==2) 
			break;

		/* simply pass and ignore none SB/SE command, which is 3 bytes*/

		if(c!=SB)
		{
			left_len-=3;
			p_buf->read_ptr=ADD_RING_PTR(p_buf->read_ptr,3);
			continue;   

		}

		/* try to find IAC SE */

		n=find_iac_se(3,left_len-3);

		if(n<0)  //not found
			break;

		n++;
		left_len-=n;
		p_buf->read_ptr=ADD_RING_PTR(p_buf->read_ptr,n);

	} 

	return out_ptr;
}

int get_max_data_batch_len(void)
{
	return  TELNET_BUF_LEN/4;
}

const char * get_init_cmd_buf(void)
{
	return  init_iac_cmd;
}

int get_init_cmd_len(void)
{
        return sizeof(init_iac_cmd);
}

void save_data_to_buffer(char * data, int len)
{
	int i;

        struct ring_buf *p_buf=&g_ring_buf;

        init_ring_buf();

	for(i=0;i<len;i++)
	{
		p_buf->data[p_buf->write_ptr]=data[i];
		p_buf->write_ptr=ADD_RING_PTR(p_buf->write_ptr,1);
	}
}
