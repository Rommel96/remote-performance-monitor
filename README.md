# Remote performance monitor

TCP Server-Client implementation in C
Application for remote monitoring of the state of a computer.

### Server: ###

The server program maintains several client connections at the same time, and has the following use (which should be shown when using the -h option):

**monitord** transmits the data on the use and performance of the server to all the connected clients.

**Usage**

```bash
  monitord  [-c] [<port>]
            [-l <log_file>] [<port>]
            [-h]
```

**Options:**

```bash
  -h             Help, show this screen.

  -c             Console mode.

  -l <log_file>  Log file to use [default: log.txt].
```

When establishing a connection, the program sends the following information to the client every second:


* Versión del sistema operativo
* Uptime
* Promedio carga en 1 min, 5 min y 15 min
* Número de procesadores
* Porcentaje de uso de cada procesador
* Memoria total
* Memoria libre
* Número de procesos
* Número de procesos en ejecución


### Client: ###

**monitorc** connects to a remote monitor service (server) and displays server usage and performance information.


**Usage**
```bash
  monitord  [<ip>] [<port>]
            [-h]
```

**Options:**
```bash
  -h             Help, show this screen.
```
