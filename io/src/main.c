#include <commons/log.h>
#include <io.h>
#include <utils/hello.h>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Error: Uso incorrecto.\n");
        printf("Formato correcto: %s <nombre_interfaz>\n", argv[0]);
        return 1; // Retorna código de error
    }
    char *nombre_interfaz = argv[1];
    printf("[IO] Interfaz inicializada: %s\n", nombre_interfaz);


    saludar("io");


    int conexion;
	char *IP_KERNEL;
	char *PUERTO_KERNEL;
	char *LOG_LEVEL;

	t_log *logger;
	t_config *config;
    
    bool apagar_interfaz = true;

    config = iniciar_config();
    
    IP_KERNEL = config_get_string_value(config, "IP_KERNEL");
    PUERTO_KERNEL = config_get_string_value(config, "PUERTO_KERNEL");
    LOG_LEVEL = config_get_string_value(config, "LOG_LEVEL");

    logger = iniciar_logger(LOG_LEVEL);

    // Handshake con el kernel
    conexion = crear_conexion(IP_KERNEL,PUERTO_KERNEL);
    if (conexion == -1) {
    log_error(logger, "No se pudo establecer conexión con el Kernel");
    return EXIT_FAILURE;
    }
    enviar_mensaje_handshake(nombre_interfaz,conexion);
    log_trace(logger, "[IO] Enviando nombre de interfaz al kernel: %s", nombre_interfaz);
    //enviara al kernel el nombre de la interfaz y quedara esperando peticiones del mismo
    
    
    while(apagar_interfaz) {
         printf("[IO] Esperando peticiones del kernel...\n");
        // Espera peticiones del kernel
  
        log_info(logger,  "## PID: <PID> - Inicio de IO - Tiempo: <TIEMPO_IO>");
        usleep(3000000);
        log_info(logger,  "## PID: <PID> - Fin de IO");
        apagar_interfaz = false;
    }

    // Liberar recursos y cerrar la interfaz
    close(conexion);
    log_info(logger, "[IO] Finalizando interfaz...");
    log_destroy(logger);
    config_destroy(config);
    return 0;
}

t_log *iniciar_logger(char *LOG_LEVEL)
{
	t_log *nuevo_logger;
	if ((nuevo_logger = log_create("io.log", "System", 1, log_level_from_string(LOG_LEVEL))) == NULL)
	{
		printf("No se pudo crear el archivo de log \n");
		exit(EXIT_FAILURE);
	}
	return nuevo_logger;
}

t_config *iniciar_config(void)
{
	t_config *nuevo_config;
	if ((nuevo_config = config_create( "../io.config")) == NULL)
	{
		printf("No se pudo abrir el archivo de configuracion\n");
		exit(EXIT_FAILURE);
	}
	return nuevo_config;
}
