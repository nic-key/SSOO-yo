#include <utils/hello.h>
#include <memoria.h>
int main(int argc, char* argv[]) {
    saludar("memoria");


    t_log *logger;
	t_config *config;

    int conexion;
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

    /*Conexion a kernel*/
    int socket_memoria_kernel = iniciar_servidor(PUERTO_ESCUCHA,logger);
    if (socket_memoria_kernel == -1) {
    log_error(logger, "Fallo al iniciar Socket Memoria");
    return EXIT_FAILURE;
    }

    log_info(logger, "Memoria listo para recibir al peticiones");
    int cliente_kernel_fd = esperar_cliente(socket_memoria_kernel,logger);
    if (cliente_kernel_fd != -1){
    log_info(logger, "Se ha conectado el kernel a la memoria");
    }
    
    /*Conexion CPU*/
    int socket_memoria_cpu = iniciar_servidor(PUERTO_ESCUCHA,logger);
    if (socket_memoria_cpu == -1) {
    log_error(logger, "Fallo al iniciar Socket cpu");
    return EXIT_FAILURE;
    }

    log_info(logger, "Memoria listo para recibir al peticiones");
    int cliente_cpu_fd = esperar_cliente(socket_memoria_cpu,logger);
    if (cliente_cpu_fd != -1){
    log_info(logger, "Se ha conectado el cpu a la memoria");
    }

    //Liberar recurso
    close(socket_memoria_kernel);
    close(socket_memoria_cpu);
    close(cliente_kernel_fd);
    
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
