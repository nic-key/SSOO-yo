#ifndef IO_H_
#define IO_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include <utils/hello.h>


t_log* iniciar_logger(char *LOG_LEVEL);
t_config* iniciar_config(void);

#endif /* IO_H_ */
