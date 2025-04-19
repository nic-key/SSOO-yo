#ifndef KERNEL_H_
#define KERNEL_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<utils/hello.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// Define pcb_t or include the appropriate header file
//TCB
typedef enum {
    NEW,
    READY,
    EXEC,
    BLOCKED,
    SUSPENDED_BLOCKED,
    SUSPENDED_READY,
    EXIT
} estado_proceso;

typedef struct {
    int cantidad;
    double tiempo_total;
} metrica_estado;

typedef struct {
    int pid;
    int pc;
    int tamanio_memoria;
    estado_proceso estado_actual;
    metrica_estado metricas[7];
    time_t tiempo_entrada_estado;
} pcb_t;


t_log* iniciar_logger(char *LOG_LEVEL);
t_config* iniciar_config(void);
const char* nombre_estado(estado_proceso estado);
pcb_t* crear_pcb(int pid, int tamanio_memoria);
void cambiar_estado(pcb_t* pcb, estado_proceso nuevo_estado);
void destruir_pcb(pcb_t* pcb);
void* conexionIO(t_log *logger);
void* conexionCPU(t_log *logger);

#endif /*END KERNEL_H_*/
