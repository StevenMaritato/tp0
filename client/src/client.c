#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;
	t_log* logger;
	t_config* config;


	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */
	config = iniciar_config();

	if (config == NULL) {
		abort();
		// ¡No se pudo crear el config!
		// Terminemos el programa
	}

	valor = config_get_string_value(config, "CLAVE");
	log_info(logger, "CLAVE: %s", valor);

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	
	log_info(logger, "IP: %s", ip);
	log_info(logger, "Puerto: %s", puerto);

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	// log_info(logger, valor);
	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */

	// leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	size_t bytes;

	int32_t handshake = 1;
	int32_t result;
	t_log* loggerin = log_create("tp0.log", "Loggerin :)", true, LOG_LEVEL_INFO);

	bytes = send(conexion, &handshake, sizeof(int32_t), 0);
	bytes = recv(conexion, &result, sizeof(int32_t), MSG_WAITALL);

	if (result == 0) {
		log_info(loggerin, "Se comunicaron re bien");
	} else {
    	log_error(loggerin, "NOOOOO CUIDADO QUE ESTO EXPLOTA!!!");
	}		
	
	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	liberar_conexion(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create ("tp0.log", "El LOG", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("../cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	leido = readline("> ");

	while(*leido != '\0'){
		log_info(logger, "Se a leido %s", leido);
		free(leido);
		leido = readline("> ");
	}
	free(leido);
	// La primera te la dejo de yapa
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	// ¡No te olvides de liberar las lineas antes de regresar!
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	// Leemos y esta vez agregamos las lineas al paquete	
	char* leido;
	t_paquete* paquete;
	
	paquete = crear_paquete();

	leido = readline("> ");

	while (*leido != '\0'){
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
		leido = readline("> ");
	}
	
	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	config_destroy(config);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
