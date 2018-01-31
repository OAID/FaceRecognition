#ifndef LOG_H__
#define LOG_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;

#define BIT(_b) (1UL << (_b))
#define BIT64(_b) (1ULL << (_b))
#define BIT_MASK(_bw) ((1UL << (_bw)) - 1)
#define BIT64_MASK(_bw) ((1ULL << (_bw)) - 1)

#define BIT_GET(_raw, _b) (!!((_raw) & BIT(_b)))
#define BIT64_GET(_raw, _b) (!!((_raw) & BIT64(_b)))
#define BIT_SET(_raw, _b, _v) \
{ \
    uint8_t __v = _v;           \
    if (__v) (_raw) |= BIT(_b); \
    else (_raw) &= (~(BIT(_b))); \
}

#define BIT_RANGE_MASK(_bs, _bw) (BIT_MASK(_bw) << (_bs))

#define FIELD_GET(_raw, _bs, _bw, _val) \
    _val = ((_raw) & BIT_RANGE_MASK(_bs, _bw)) >> (_bs)

#define FIELD_SET(_raw, _bs, _bw, _val) \
    _raw = ((_raw) & (~(BIT_RANGE_MASK(_bs, _bw)))) | (((_val) & BIT_MASK(_bw))<< (_bs))

#define BIT_NCOPY(src, dst, offs, n) FIELD_GET((src), (offs), (n), (dst))

/*This order of the enum mustn`t be changed*/
typedef enum
{
	LOG_NULL = 0,
    LOG_VERB,
    LOG_STAT,
    LOG_INFO,
    LOG_DEBUG,
    LOG_CONFIG,
    LOG_EVENT,
    LOG_WARN,
    LOG_ALARM,
    LOG_ERROR,
    LOG_EMERG,
    LOG_PERFIX,    //output the perfix info "__FUNCTION__" and "__LINE__"
    LOG_MAX
} LOG_E;

/*This order of the enum mustn`t be changed*/
extern char LOG_e_str[][30];
extern uint32_t g_LOG_config;
extern void log_config_init();
extern void log_config_get(string &str);
extern void log_config_set(string &str);


#define aid_log(_type, _fmt, para...) \
    { \
        if (BIT_GET(g_LOG_config, _type)) \
        { \
        	if(BIT_GET(g_LOG_config, LOG_PERFIX)) \
        	{\
        		char * str = LOG_e_str[_type]; \
	   	   		fprintf(stderr, "[%7s] %s():%d - " _fmt, str+4, __FUNCTION__, __LINE__, ##para); \
        	}\
	   	   	else \
				fprintf(stderr, _fmt, ##para); \
        } \
    }

#define log_emerg(_fmt, para...)    aid_log(LOG_EMERG,  _fmt, ##para)
#define log_alarm(_fmt, para...)    aid_log(LOG_ALARM,  _fmt, ##para)
#define log_config(_fmt, para...)   aid_log(LOG_CONFIG, _fmt, ##para)
#define log_error(_fmt, para...)    aid_log(LOG_ERROR,  _fmt, ##para)
#define log_warn(_fmt, para...)     aid_log(LOG_WARN,   _fmt, ##para)
#define log_event(_fmt, para...)    aid_log(LOG_EVENT,  _fmt, ##para)
#define log_info(_fmt, para...)     aid_log(LOG_INFO,   _fmt, ##para)
#define log_debug(_fmt, para...)    aid_log(LOG_DEBUG,  _fmt, ##para)
#define log_verb(_fmt, para...)     aid_log(LOG_VERB,   _fmt, ##para)
#define log_stat(_fmt, para...)     aid_log(LOG_STAT,   _fmt, ##para)
#define log_printf(_fmt, para...)   fprintf(stderr, _fmt, ##para);

#endif /* !LOG_H__ */
