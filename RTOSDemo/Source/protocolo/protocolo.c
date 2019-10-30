///*
// * protocolo.c
#include <protocolo/protocolo.h>														// Estructura de datos asociada al canal de recepcion

/************************************************************************************
 *
 *		\function 	int PAQ_Clasificar_Recibido (t_paq_buf_cmp paq_buf_cmp)
 *
 *		\brief 		agrego información de clasificación al paquete recibido
 *
 ************************************************************************************/

int PAQ_Clasificar_Recibido (t_paq_buf_cmp paq_buf_cmp)/*(t_cola msjReceived)*/
{

	// 1) Copio lo almacenado en la cola de datos al buffer
	// if(PAQ_Extraer_Info_Cola() != PAQ_EXTRAER_INFO_COLA_OK)
//	if(PAQ_Extraer_Info_Cola(msjReceived) == PAQ_EXTRAER_INFO_COLA_ERROR)															// Copio de la cola de datos a los campos
//	{
//		if(canal_rx.cola->estado_buffer == BUFFER_TIMEOUT)																// Si hubo un error al copiar los campos pregunto si fue por timeout
//			return PAQ_CLASIFICAR_RECIBIDO_TIMEOUT;																		// Retorno el error para hacer la respuesta correcta
//		else
//			return PAQ_CLASIFICAR_RECIBIDO_PAQ_NO_EXTRAIDO;																// Si no fue por timeout, es por un error interno. También lo informo
//	}

	// 2) Chequeo que el ID me corresponda a esta terminal
	if(t0.ID != paq_buf_cmp.bytes_campos.DST) return PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_DST;
	// 3) Compruebo que el checksum este bien
	if(PAQ_Validar_Checksum(paq_buf_cmp) != PAQ_VALIDAR_CHECKSUM_VALIDO) return PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_CHK;
	// 4) Compruebo que el comando esté OK
	if(PAQ_Validar_Comando(paq_buf_cmp) != PAQ_VALIDAR_COMANDO_VALIDO) return PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_CMD;
	// 5) Si esta bien, devuelvo OK
	return PAQ_CLASIFICAR_RECIBIDO_OK;
}


/************************************************************************************
 *
 *		\function 	t_paq_buf_cmp PAQ_Armar_Respuesta_Estandar (t_paq_buf_cmp paq_buf_cmp,uint8_t cmd_respuesta)
 *
 *		\brief 		Armo el paquete que voy a responder (que no sea dato)
 *
 *		\paramin	cmd_respuesta 	campo comando de respuesta al frame recibido
 *
 ************************************************************************************/

t_paq_buf_cmp PAQ_Armar_Respuesta_Estandar (t_paq_buf_cmp paq_buf_cmp,uint8_t cmd_respuesta)
{
	t_paq_buf_cmp reply_paq ;

	reply_paq.bytes_campos.ORG = t0.ID;															// Asigno mi ID al campo origen
	reply_paq.bytes_campos.DST = paq_buf_cmp.bytes_campos.ORG;								// Asigno el ID de quien me envio datos al DESTINO
	reply_paq.bytes_campos.CMD  = cmd_respuesta;													// Guardo el comando con los bits invertidos
	reply_paq.bytes_campos.D3 = 0x00;
	reply_paq.bytes_campos.D2 = 0x00;
	reply_paq.bytes_campos.D1 = 0x00;
	reply_paq.bytes_campos.D0 = 0x00;
	reply_paq.bytes_campos.CHK = PAQ_Calcular_Checksum (reply_paq);


//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_DES])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_ORG])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_CMD])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_D3])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_D2])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_D1])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_D0])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_CHK])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//
//	canal_tx.paq_pendiente = 1;																			// Levanto el flag de dato pendiente

	return reply_paq;
}

/************************************************************************************
 *
 *		\function 	t_paq_buf_cmp PAQ_Armar_Respuesta_CMD_DAT (t_paq_buf_cmp paq_buf_cmp,t_paq_dato dato)
 *
 *		\brief 		Armo el paquete que voy a responder.
 *		\brief 		Caso 4 : Recibí un pedido de datos y respondo con el dato pedido
 *
 ************************************************************************************/

t_paq_buf_cmp PAQ_Armar_Respuesta_CMD_DAT (t_paq_buf_cmp paq_buf_cmp,t_paq_dato dato)
{
	t_paq_buf_cmp reply_paq ;

	reply_paq.bytes_campos.ORG = t0.ID;															// Asigno mi ID al campo origen
	reply_paq.bytes_campos.DST = paq_buf_cmp.bytes_campos.ORG;								// Asigno el ID de quien me envio datos al DESTINO
	reply_paq.bytes_campos.CMD  = CMD_DAT;														// Guardo el comando con los bits invertidos
	reply_paq.bytes_campos.D3 = dato.bytes.b0;
	reply_paq.bytes_campos.D2 = dato.bytes.b1;
	reply_paq.bytes_campos.D1 = dato.bytes.b2;
	reply_paq.bytes_campos.D0 = dato.bytes.b3;
	reply_paq.bytes_campos.CHK = PAQ_Calcular_Checksum (reply_paq);

//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_DES])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_ORG])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_CMD])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_D3])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_D2])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_D1])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_D0])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//	if(Escribir_Cola (canal_tx.cola, (uint8_t)canal_tx.buf_cmp.buffer[PQ_BYTE_CHK])) return PAQ_EXTRAER_INFO_COLA_ERROR;
//
//		canal_tx.paq_pendiente = 1;																			// Levanto el flag de dato pendiente

	return reply_paq;
}

/************************************************************************************
 *
 *		\function 	uint8_t PAQ_Calcular_Checksum (t_paq_buf_cmp)
 *
 *		\brief 		Calculo el byte de checksum de un paquete
 *
 ************************************************************************************/

uint8_t PAQ_Calcular_Checksum (t_paq_buf_cmp paq_buf_cmp)
{
	uint8_t suma_checksum = 0;
	uint8_t i;

	for (i = 0; i < PQ_LARGO_BYTES - 1;i++)
		suma_checksum ^= paq_buf_cmp.buffer[i];

	//p_canal->buf_cmp.bytes_campos.CHK = suma_checksum;

	return suma_checksum;
}

/************************************************************************************
 *
 *		\function 	PAQ_Validar_Checksum (t_canal canal)
 *
 *		\brief 		Compruebo si el frame tiene el checksum bien
 *
 ************************************************************************************/

int PAQ_Validar_Checksum (t_paq_buf_cmp paq_buf_cmp)
{
	uint8_t suma_checksum = 0;
	uint8_t i;

	for (i = 0; i < PQ_LARGO_BYTES ; i++)
		suma_checksum ^= paq_buf_cmp.buffer[i];
//descomentar!
//	if(suma_checksum)
//		return PAQ_VALIDAR_CHECKSUM_NO_VALIDO;
//	else
		return PAQ_VALIDAR_CHECKSUM_VALIDO;

}

/************************************************************************************
 *
 *		\function 	PAQ_Validar_Comando (t_canal canal)
 *
 *		\brief 		Compruebo si el frame tiene el comando bien
 *
 ************************************************************************************/

int PAQ_Validar_Comando (t_paq_buf_cmp paq_buf_cmp)
{
	switch(paq_buf_cmp.bytes_campos.CMD)
	{
	case CMD_DAT:
	case CMD0:
	case CMD1:
	case CMD2:
	case CMD3:
	case CMD4:
	case CMD5:
	case CMD6:
	case CMD7:
	case CMD8:
	case CMD9:
	case CMD10:
	case CMD11:
	case CMD12:
	case CMD13:
	case CMD14:
	case CMD15:
	case CMD16:
	case CMD17:
	case CMD18:
	case CMD19:
	case CMD20:
		return PAQ_VALIDAR_COMANDO_VALIDO;

	default:
		return PAQ_VALIDAR_COMANDO_NO_VALIDO;
	}
}
//
//
///************************************************************************************
// *
// *		\function 	void PAQ_Rutina_Principal (void)
// *
// *		\brief 		Rutina principal
// *
// ************************************************************************************/
//
//void PAQ_Rutina_Principal (void)
//{
//
//	PAQ_Enviar_Cola_Salida();															// Pongo en el THR si tengo algo para enviar
//
//	if(!canal_rx.cola->datos_nuevos)	return;											// Si no tengo ningún paquete para analizar, salgo
//	canal_rx.cola->datos_nuevos = 0;													// Bajo el flag de datos pendientes
//
//	global_paq_clasificado = PAQ_Clasificar_Recibido();									// Analizo que el paquete no tenga errores y lo clasifico
//
//	switch(global_paq_clasificado)
//	{
//	case PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_DST :										// Lo que recibí no era para mí
//		return;
//
//	case PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_CHK :										// Recibí con un error de checksum
//		PAQ_Armar_Respuesta_Estandar(CMD_ECK);
//		return;
//
//	case PAQ_CLASIFICAR_RECIBIDO_TIMEOUT :												// Recibí el frame incompleto (menos de 8 bytes)
//		PAQ_Armar_Respuesta_Estandar(CMD_ETO);
//		return;
//
//	case PAQ_CLASIFICAR_RECIBIDO_PAQ_NO_EXTRAIDO :										// Recibí algo pero no lo puedo ver por error interno
//		PAQ_Armar_Respuesta_Estandar(CMD_EIN);
//		return;
//
//	case PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_CMD :										// Recibí un comando no válido
//		PAQ_Armar_Respuesta_Estandar(CMD_ENE);
//		return;
//
//	case PAQ_CLASIFICAR_RECIBIDO_OK	:													// El paquete fue recibido correctamente.
//		break;
//
//	default :
//		PAQ_Error(1);
//		return;
//	}
//
//	global_paq_clasificado = PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_ECX;							// Pongo por default en fuera de contexto
//
//	/************ AGREGO TODAS LAS TAREAS QUE RECIBAN Y ENVIEN COMANDOS ****************/
//	ME_SATFIg ();																			// Llamo a la tarea ppal y puede modificar global_paq_clasificado y global_dato
//	/***********************************************************************************/
//
//	switch(global_paq_clasificado)
//	{
//	case PAQ_ENVIAR_DATO :
//		PAQ_Armar_Respuesta_CMD_DAT(global_dato);
//		return;
//
//	case PAQ_CLASIFICAR_RECIBIDO_OK	:													// El paquete fue recibido correctamente.
//		PAQ_Armar_Respuesta_Estandar(CMD_OK);											// Falta ver si está en contexto, solo comprueba frame
//		break;
//
//	case PAQ_CLASIFICAR_RECIBIDO_PAQ_ERROR_ECX :
//		PAQ_Armar_Respuesta_Estandar(CMD_ECX);
//		break;
//
//	default :
//		PAQ_Error(2);
//		return;
//	}
//}
//
///************************************************************************************
// *
// * \function void PAQ_Error (void)
// *
// * \brief bloqueo el sistema en caso de error inesperado
// *
// ************************************************************************************/
//
//void PAQ_Error (int error_sel)
//{
//	PAQ_error_sel = error_sel;
//	while(1);
//}
