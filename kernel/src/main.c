#include <utils/hello.h>
#include <kernel.h>
int main(int argc, char* argv[]) {
    /*
    if (argc != 3) {
        printf("Uso: %s [archivo_pseudocodigo] [tamanio_proceso]\n", argv[0]);
        return 1;
    }
    char *archivo = argv[1];
    int tamanio_proceso = atoi(argv[2]);
    */

   //Para correr sin necesidad de los parametros
    char *archivo = NULL;
    int tamanio_proceso = 0;

    if (argc == 3) {
        archivo = argv[1];
        tamanio_proceso = atoi(argv[2]);

        // Validar archivo
        FILE* test = fopen(archivo, "r");
        if (!test) {
            printf("No se pudo abrir el archivo de pseudocódigo: %s\n", archivo);
            return EXIT_FAILURE;
        }
        fclose(test);
        printf("Modo PROCESO: Archivo: %s | Tamaño: %d\n", archivo, tamanio_proceso);
    } else {
        printf("Modo SERVIDOR: sin proceso. Solo escuchando conexiones.\n");
    }

    int conexion;
	char *IP_MEMORIA;
	char *PUERTO_MEMORIA;
    char *PUERTO_ESCUCHA_DISPATCH;
    char *PUERTO_ESCUCHA_INTERRUPT;
    char *PUERTO_ESCUCHA_IO;
    char *ALGORITMO_PLANIFICACION;
    char *ALGORITMO_COLA_NEW;
    char *ALFA;
    char *TIEMPO_SUSPENSION;
	char *LOG_LEVEL;

	t_log *logger;
	t_config *config;


    config = iniciar_config();
    
    IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(config, "PUERTO_MEMORIA");
    PUERTO_ESCUCHA_DISPATCH = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    PUERTO_ESCUCHA_INTERRUPT = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");
    PUERTO_ESCUCHA_IO = config_get_string_value(config, "PUERTO_ESCUCHA_IO");
    ALGORITMO_PLANIFICACION = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    ALGORITMO_COLA_NEW = config_get_string_value(config, "ALGORITMO_COLA_NEW");
    ALFA = config_get_string_value(config, "ALFA");
    TIEMPO_SUSPENSION = config_get_string_value(config, "TIEMPO_SUSPENSION");
    LOG_LEVEL = config_get_string_value(config, "LOG_LEVEL");


    logger = iniciar_logger(LOG_LEVEL);

    /*Conexion Memoria*/
    conexion = crear_conexion(IP_MEMORIA,PUERTO_MEMORIA);
    if (conexion == -1) {
    log_error(logger, "No se pudo establecer conexión con la Memoria");
    return EXIT_FAILURE;
    }
    else {
    log_info(logger, "Kernel se ha conectado a la memoria");
    }


    /*Conexion IO*/
    int socket_io = iniciar_servidor(PUERTO_ESCUCHA_IO,logger);
    if (socket_io == -1) {
    log_error(logger, "Fallo al iniciar servidor IO");
    return EXIT_FAILURE;
    }

    log_info(logger, "Kernel listo para recibir al peticiones");
    int cliente_IO_fd = esperar_cliente(socket_io,logger);

    saludar("kernel");

    //Handshake IO

    char* nombre_io = handshake(cliente_IO_fd, logger);
    if (nombre_io != NULL) {
        log_info(logger, "IO conectado: %s", nombre_io);
        free(nombre_io);
    }
    // Liberar recursos
    //close(conexion_memoria);
    close(socket_io);
    //close(socket_dispatch);
    //close(socket_interrupt);
    close(cliente_IO_fd);

    log_info(logger, "Kernel finalizando...");
    log_destroy(logger);
    config_destroy(config);
    return 0;
}


t_log *iniciar_logger(char *LOG_LEVEL)
{
	t_log *nuevo_logger;
	if ((nuevo_logger = log_create("kernel.log", "System", 1, log_level_from_string(LOG_LEVEL))) == NULL)
	{
		printf("No se pudo crear el archivo de log \n");
		exit(EXIT_FAILURE);
	}
	return nuevo_logger;
}

t_config *iniciar_config(void)
{
	t_config *nuevo_config;
	if ((nuevo_config = config_create( "../kernel.config")) == NULL)
	{
		printf("No se pudo abrir el archivo de configuracion\n");
		exit(EXIT_FAILURE);
	}
	return nuevo_config;
}
