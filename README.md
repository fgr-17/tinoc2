**Proyecto: TINOC2**

Autores: 
	- GARCIA, Pablo (pegarcia@citedef.gob.ar)
	- ROUX, Federico (froux@citedef.gob.ar)
	
Resumen: Repositorio de firmware del proyecto TINOC2 basado en FreeRTOS. Desarrollado sobre la IDE LPCXpresso 8.2.2 y compilado sobre el microcontrolador LPC1102 montado sobre el TINOCv1 y la base TINOC_LPCLink.

Historial:

* 15-08-2017: Inicialización del repositorio con un blinky basado en CMSIS baremetal y un blinky basado en FreeRTOS. Usando en ambos casos el led conectado en el P0_11

* 18-08-2017: 
	- se creo el archivo LPC1102_IOCON.h con las equivalencias de los pines nombrados con letra y nro. a numero de puerto y pin.
	- se creo la documentación en doxygen para el CMSIS y el archivo LPC1102_IOCON.h
	
	corregir : 
	- ver donde queda mejor el archivo LPC1102_IOCON.h
	- ver IOCON_SCK_LOC UM10429 pág.67

	
* 22-08-2017:
	- Se creó el archivo LPC1102_IOCON.c en la carpeta /src del CMSISv2p00_LPC11xx
	
* 01-09-2017:
	- Se creo la carpeta /hardware en el proyecto RTOSDemo. 
	- Se crearon los archivos tinoc_lpclink.c y tinoc_lpclink.h para el manejo del hardware asociado al pcb específico.
	- Se usan directamente las macros definidas en LPC1102_IOCON.h en lugar de funciones por simplicidad.
	
* 22-09-2017:
	- Se agrego la función IAP para reinvocar ISP desde un pin. 
	- No se pudo aún resetear el target desde el LPCLink sin acceder al core
	
* 27-09-2017:
	- Corregido los bits de mascara de modo LPC_IOCON_MODE.
	- corregido el seteo de ADMODE para los pines con funcion digital. Tienen que tener este bit en cero.
	- Modificada ReinvokeISP con reseteo de los pines de SWD
	
* 28-09-2017:

	- bug corregido en la definición del pin declarado como entrada ISP. No tenìa el bit ADMODE en alto.
	- se quitaron las definiciones de los pines SWD. Revisar como setear los pines SWD como en el estado inicial. Copiar eso y reproducirlo al comienzo del programa.

* 12-10-2017 / 13-10-2017:

	- agregado module_config.h (configuracion de tipo de modulo) y y driver_config.h (configuraciones de drivers)
	- agregado driver de UART (uart.c y uart.h) para manejo de el serie RS485.
	- agregado driver de timer (timer32.c y timer32.h). 
	- modificado el main-blinky.c para que inicie el modulo serie a 9600 de baudrate e imprima un caracter al mismo rate que el parpadeo del led.


--------------------------------------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------

* 15.01.2018:

	- Repositorio recuperado despues de una falla que no permitía volver a programar las placas. Se empezó de cero. La primer versión del repositorio tiene el ejemplo del 1102 y la aplicación base del FreeRTOS. 
	- Se modificó el proyecto del freeRTOS para usar la librería del ejemplo LPCX1102_cmsis2_systick. Ambos proyectos se pueden descargar al Tinoc y funcionan como un blinky


* 17.01.2018:

	- se agregó el archivo ./hardware/hardware_aplicacion.h con la configuracion especifica para una aplicacion. Deberia levantar del archivo IOCON y del tinoc_lpclink.h
	- se corrigieron los nombres de las constantes de mascara de los registros IOCON. Les faltaba "MAS" al final del nombre.
	- se agrego el archivo ./hardware/ardware_aplicacion.c con las funciones de inicializacion de pines y de parpadeo.
	- se paso todo el manejo del hardware de GPIO a las definiciones de IOCON y tinoc_lpclink.
	- Falta quitar el main_blinky y pasar todo al main comun.
	- se programó la placa dos veces para verificar.
	- falta generar el doxygen


* 18.01.2018:

	- Se genera el doxygen separado para cada uno de los proyectos del workspace. Falta arreglar el doxygen del proyecto ppal del freeRTOS.

* 26.02.2018 :

	- Se reestructuró el proyecto FreeRTOS DEMO que tenía dos mains alternativos para trabajar según el seteo de una variable. El primer main era "main_blinky" que fue donde se hizo todo el trabajo, y el segundo main era un main con más opciones de configuración. Se eliminaron ambos main y se traslado todo al main ppal. Se chequeó el funcionamiento correcto, salvo por el seteo del parámetro inicial de la tarea "pvParameters", que por alguna razón se copia mal desde TaskCreate a la tarea.

* 05.03.2018 :

	- Se mejoro la documentación en doxygen 
	- Se creo la carpeta "flash" y los archivos flash.c y flash.h para todas las operaciones de este tema
	
* 09.03.2018 :

	- Finalizada la documentación en doxygen
	- se agrego documentacion en pdf generada desde doxygen pero no quedo totalmente bien
	- correcciones menores de algunos defines

* 13.03.2018 :

	- se agrego documentacion de variables de UART.c
	- se corrigieron algunos errores de inclusión de archivos
	- se modificaron la variable tx_libre y timeout_pendiente a bool. 

* 14.03.2018 :

	- se modificaron las definiciones de las funciones Escribir y Leer cola Inline, por la version del compilador cambia la forma de definir el atributo __attribute__((always_inline)).
	- se agregaron algunos comentarios y definiciones.
	- se agregaron los archivos gpio.h y gpio.c
	- se agrego archivo de pendientes al raiz


