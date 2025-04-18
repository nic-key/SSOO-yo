#ifndef MEMORIA_H_
#define MEMORIA_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<utils/hello.h>


t_log* iniciar_logger(char *LOG_LEVEL);
t_config* iniciar_config(void);

#endif /*END MEMORIA_H_*/
