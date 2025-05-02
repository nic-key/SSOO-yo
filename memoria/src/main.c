#include <utils/hello.h>
#include <memoria.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

t_log *logger = NULL;
t_config *config;

void* manejar_cliente(void* socket_fd_void) { //El hilo que atiendo al cliente
    int socket_cliente = *(int*)socket_fd_void;
    free(socket_fd_void);

    log_info(logger, "Nuevo cliente conectado. FD: %d", socket_cliente);

    char* nombre = handshake(socket_cliente, logger);
    if (nombre != NULL) {
        log_info(logger, "Identificado cliente: %s", nombre);
        free(nombre);
    }

    //Logica para manejar peticiones

    close(socket_cliente);
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    saludar("memoria");

    //int conexion;
	char *PUERTO_ESCUCHA;
	char *TAM_MEMORIA;
    char *TAM_PAGINA;
    char *ENTRADAS_POR_TABLA;
    char *CANTIDAD_NIVELES;
    char *RETARDO_MEMORIA;
    char *PATH_SWAPFILE;
    char *RETARDO_SWAP;
    char *LOG_LEVEL;
    char *DUMP_PATH;

    config = iniciar_config();

    PUERTO_ESCUCHA = config_get_string_value(config, "PUERTO_ESCUCHA");
    TAM_MEMORIA = config_get_string_value(config, "TAM_MEMORIA");
    TAM_PAGINA = config_get_string_value(config, "TAM_PAGINA");
    ENTRADAS_POR_TABLA = config_get_string_value(config, "ENTRADAS_POR_TABLA");
    CANTIDAD_NIVELES = config_get_string_value(config, "CANTIDAD_NIVELES");
    RETARDO_MEMORIA = config_get_string_value(config, "RETARDO_NIVELES");
    PATH_SWAPFILE = config_get_string_value(config, "PATH_SWAPFILE");
    RETARDO_SWAP = config_get_string_value(config, "RETARDO_SWAP");
    LOG_LEVEL = config_get_string_value(config, "LOG_LEVEL");
    DUMP_PATH = config_get_string_value(config, "DUMP_PATH");

    logger = iniciar_logger(LOG_LEVEL);
    
    int socket_servidor = iniciar_servidor(PUERTO_ESCUCHA, logger);
    if (socket_servidor == -1) {
        log_error(logger, "Fallo al iniciar socket de escucha");
        return EXIT_FAILURE;
    }

    log_info(logger, "Memoria escuchando conexiones en puerto %s", PUERTO_ESCUCHA);

    // Loop principal de aceptación de clientes
    while (1) {
        int* socket_cliente = malloc(sizeof(int));
        *socket_cliente = esperar_cliente(socket_servidor, logger);

        if (*socket_cliente != -1) {
            pthread_t hilo_cliente;
            pthread_create(&hilo_cliente, NULL, manejar_cliente, socket_cliente);
            pthread_detach(hilo_cliente); // No bloquea ni hay que hacer join
        } else {
            free(socket_cliente);
        }
    }

    //Liberar recurso
    close(socket_servidor);
    log_info(logger, "Memoria finalizando...");
    log_destroy(logger);
    config_destroy(config);
    return 0;

}

t_log *iniciar_logger(char *LOG_LEVEL)
{
	t_log *nuevo_logger;
	if ((nuevo_logger = log_create("memoria.log", "System", 1, log_level_from_string(LOG_LEVEL))) == NULL)
	{
		printf("No se pudo crear el archivo de log \n");
		exit(EXIT_FAILURE);
	}
	return nuevo_logger;
}

t_config *iniciar_config(void)
{
	t_config *nuevo_config;
	if ((nuevo_config = config_create( "../memoria.config")) == NULL)
	{
		printf("No se pudo abrir el archivo de configuracion\n");
		exit(EXIT_FAILURE);
	}
	return nuevo_config;
}
