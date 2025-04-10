#include <utils/hello.h>
#include <kernel.h>
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s [archivo_pseudocodigo] [tamanio_proceso]\n", argv[0]);
        return 1;
    }
    char *archivo = argv[1];
    int tamanio_proceso = atoi(argv[2]);

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

    saludar("kernel");
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
