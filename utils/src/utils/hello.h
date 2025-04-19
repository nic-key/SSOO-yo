#ifndef UTILS_HELLO_H_
#define UTILS_HELLO_H_

#include <stdlib.h>
#include <stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>

extern t_log* logger;
typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

/**
* @brief Imprime un saludo por consola
* @param quien Módulo desde donde se llama a la función
* @return No devuelve nada
*/

//Funciones Generales
void saludar(char* quien);
int crear_conexion(char* ip, char* puerto);

//Funciones IO
void liberar_conexion(int socket_cliente);
void enviar_mensaje_handshake(char* mensaje, int socket);
void enviar_mensaje(char* mensaje, int socket_cliente);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
void* serializar_paquete(t_paquete* paquete, int bytes);

//Server-Kernel
void* recibir_buffer(int*, int);
int iniciar_servidor(char* puerto,t_log* logger);
int esperar_cliente(int socket_cliente,t_log* logger);
t_list* recibir_paquete(int);
void recibir_mensaje(int socket_cliente,t_log* logger);
char* handshake(int socket_cliente, t_log*logger);
int recibir_operacion(int);

#endif
