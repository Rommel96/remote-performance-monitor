# Monitor remoto de rendimiento

Aplicación cliente-servidor para el monitoreo remoto del estado de una computadora.

### Uso ###

Servidor:

El programa servidor mantiene varias conexiones de clientes al mismo tiempo, y tiene el siguiente uso (el cual debe mostrarse al usar la opción -h):

./monitord –h

monitord transmite los datos de uso y rendimiento del servidor hacia todos los clientes que se enuentren conectados.

Uso específico del servidor (código):

  monitord [-c] [<port>]
  monitord [-l <log_file>] [<port>]
  monitord -h

Opciones:

  -h             Help, show this screen.
  -c             Console mode.
  -l <log_file>  Log file to use [default: log.txt].
  
Al establecer una conexión, el programa envía la siguiente información al cliente cada segundo:


* Versión del sistema operativo
* Uptime
* Promedio carga en 1 minuto, 5 minutos y 15 minutos
* Número de procesadores
* Porcentaje de uso de cada procesador
* Memoria total
* Memoria libre
* Número de procesos
* Número de procesos en ejecución

Cliente:

El programa cliente realiza lo siguiente:

/monitorc –h

monitorc (cliente) se conecta a un servicio remoto monitord (servidor) y muestra los datos de uso y rendimiento del servidor.

Uso específico del cliente (código):

  monitord [<ip>] [<port>]
  monitorc -h
 
Opciones:

  -h             Help, show this screen.


### Integrantes ###

* Ronny Martinez
* Rommel Saquicela
