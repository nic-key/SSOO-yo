#include <cpu.h>
int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Error: Uso incorrecto.\n");
        printf("Formato correcto: %s <nombre_interfaz>\n", argv[0]);
        return 1; // Retorna código de error
    }
    char *nombre_interfaz = argv[1];
    saludar("cpu");

    int conexion_kernel,conexion_memoria;
    char *IP_MEMORIA;
    char *PUERTO_MEMORIA;
    char *IP_KERNEL;
    char *PUERTO_KERNEL_DISPATCH;
    char *PUERTO_KERNEL_INTERRUPT;
    char *PUERTO_KERNEL_LIST;
    char *ENTRADAS_TLB;
    char *REEMPLAZO_TLB;
    char *ENTRADAS_CACHE;
    char *REEMPLAZO_CACHE;
    char *RETARDO_CACHE;
    char *LOG_LEVEL;

    t_log *logger;
    t_config *config;

    config = iniciar_config();

    IP_MEMORIA = config_get_string_value(config, "IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(config, "PUERTO_MEMORIA");
    IP_KERNEL = config_get_string_value(config, "IP_KERNEL");
    PUERTO_KERNEL_DISPATCH = config_get_string_value(config, "PUERTO_KERNEL_DISPATCH");
    PUERTO_KERNEL_INTERRUPT = config_get_string_value(config, "PUERTO_KERNEL_INTERRUPT");
    PUERTO_KERNEL_LIST = config_get_string_value(config, "PUERTO_KERNEL_LIST");
    ENTRADAS_TLB = config_get_string_value(config, "ENTRADAS_TLB");
    REEMPLAZO_TLB = config_get_string_value(config, "REEMPLAZO_TLB");
    ENTRADAS_CACHE = config_get_string_value(config, "ENTRADAS_CACHE");
    REEMPLAZO_CACHE = config_get_string_value(config, "REEMPLAZO_CACHE");
    RETARDO_CACHE = config_get_string_value(config, "RETARDO_CACHE");
    LOG_LEVEL = config_get_string_value(config, "LOG_LEVEL");

    logger = iniciar_logger(LOG_LEVEL);

    // Handshake con el kernel
    conexion_kernel = crear_conexion(IP_KERNEL, PUERTO_KERNEL_DISPATCH);
    if (conexion_kernel == -1)
    {
        log_error(logger, "No se pudo establecer conexión con el Kernel");
        return EXIT_FAILURE;
    }
    enviar_mensaje_handshake(nombre_interfaz, conexion_kernel);
    log_trace(logger, "[CPU] Enviando nombre de interfaz al kernel: CPU 01");
    // enviara al kernel el nombre de la interfaz y quedara esperando peticiones del mismo

    //Conexion con la memoria
    conexion_memoria = crear_conexion(IP_MEMORIA,PUERTO_MEMORIA);
    if (conexion_memoria == -1)
    {
        log_error(logger, "No se pudo establecer conexión con la memoria");
        return EXIT_FAILURE;
    }
    enviar_mensaje_handshake(nombre_interfaz, conexion_memoria);
    log_trace(logger, "[CPU] Enviando nombre de interfaz a la memoria: CPU 01");

    close(conexion_kernel);
    close(conexion_memoria);
    log_destroy(logger);
    config_destroy(config);

    return 0;
}

t_log *iniciar_logger(char *LOG_LEVEL)
{
    t_log *nuevo_logger;
    if ((nuevo_logger = log_create("CPU.log", "System", 1, log_level_from_string(LOG_LEVEL))) == NULL)
    {
        printf("No se pudo crear el archivo de log \n");
        exit(EXIT_FAILURE);
    }
    return nuevo_logger;
}

t_config *iniciar_config(void)
{
    t_config *nuevo_config;
    if ((nuevo_config = config_create("../cpu.config")) == NULL)
    {
        printf("No se pudo abrir el archivo de configuracion\n");
        exit(EXIT_FAILURE);
    }
    return nuevo_config;
}
