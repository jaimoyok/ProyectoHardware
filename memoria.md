---
title:  Memoria Técnica
subtitle: Proyecto Hardware
author:   
    - Jaime Yoldi Vigueras 779057
    - José Marín Díez 778148
date: Enero 2021 \newpage
toc: true
toc-title: Índice
fontsize: 12pt
numbersections: true
#geometry: margin=2.5cm
urlcolor: blue
documentclass: scrartcl
#Convertir a pdf: pandoc memoria.md -o memoria.pdf
---
\newpage

# Introducción

En este documento se presenta la memoria técnica correspondiente a las prácticas 2 y 3 de la asignatura Proyecto Hardware. A lo largo del documento se detalla el proceso de implementación del conocido juego Reversi. Partiendo del proyecto en lenguaje c previamente optimizado en la práctica 1 se realizan una serie de modificaciones y adaptaciones, añadiendo nuevas funcionalidades y permitiendo que este pueda ser ejecutado en un emulador de máquina real con sus periféricos, como pueden ser botones, leds, teclado, pantalla, etc. 

Algunas partes del código serán desarrolladas en lenguaje `ARM` y otras en `c` para finalmente ser todo ello ejecutado con la ayuda de un emulador del procesador *ARM LPC2105*. El entorno de desarrollo empleado es *uVision IDE*.

# Librerias
## Botones
### EINT0
### EINT1
## Temporizadores
### TIMER0
### TIMER1
## GPIO
## Cola de eventos
## Gestión de los eventos
### Máquina de estados
## Comandos
## Power Management
## Modificaciones Reversi8
    * Utilizacion del ARM_ARM
## Modificaciones Startup
## RTC
## SWI
    * TODO: Aumentar el valor de la tabla que nos dijo para que no desborde
## UART0
## RTC

Para medir el tiempo transcurrido durante la partida se hace uso de uno de
los dos contadores con funcionalidades especificas del procesador *ARM LPC2105*,
el *Real Time Clock* (RTC). Es necesario destacar que este contador, a diferencia 
de los otros no genera ninguna interrupción, ya que como se ha dicho su único 
propósito es proporcionar información sobre el tiempo transcurrido y 
consumiendo poca energía.

Este es iniciado al comienzo del juego y está en funcionamiento en todo
momento incluso cuando el procesador está suspendido, en powerdown o idle.
Antes de esto es necesario configurarlo para adaptarlo a la frecuencia a
la que trabaja el procesador (60MHz), para ello se modifica el 
*Prescaler Integer register* y el *Prescaler Fraction register* de la 
siguiente manera.

```c
    PREINT = 0x726;
    PREFRAC = 0x700;
```

Para que el RTC comience a contar es necesario habilitarlo, para ello se activa 
el bit 0 del *Clock Control Register*, además de poner a 0 la cuenta de minutos
y segundos.

```c
	CCR=0x01;
```

Para obtener el tiempo transcurrido se hacen funciones `RTC_leer_segundos`
y `RTC_leer_minutos` que se encarga de leer del registro `CTIME0` los bits 
correspondientes y devolver el tiempo para cada caso.

## WD

Para ... se va a utilizar el otro contador específico, el watchdog (WD) ...
