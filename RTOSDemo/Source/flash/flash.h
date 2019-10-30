/** @file 	flash.h
 **
 ** @brief 	Header de flash.c
 ** @date 	Mar 5, 2018
 * 	@author Roux, Federico G. (froux@citedef.gob.ar)
 **/




#ifndef FLASH_FLASH_H_
#define FLASH_FLASH_H_


/**	@defgroup IAP Definiciones correspondientes al manejo del IAP
 *  @brief 	Funciones, variables y ctes. de manejo de la memoria flash
 * 	@{
 * 	 */
/**
 * @brief 	Dirección de memoria de punto de entrada de la función IAP.
 * 			El código de esta región está en modo Thumb, así que al entrar
 * 			a esta función, el programa entrará en modo Thumb
 * 			UM10429 pag. 173
 */
#define IAP_LOCATION 			0x1FFF1FF1
/** @brief Numero de comando para invocar ISP */
#define IAP_COM_REINVOKE_ISP	57

/* =========================================================================================
 * 		Definiciones de tipo de datos :
 *  =========================================================================================
 */

/**
 * @typedef		(*IAP)
 * @brief 		definicion de tipo de dato: puntero a función para ingresar
 * 				comandos IAP.
 */
typedef void (*IAP) (uint32_t [], uint32_t []);

/**
 * @}
 */

#endif /* FLASH_FLASH_H_ */
