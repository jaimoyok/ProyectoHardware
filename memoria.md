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

En este documento se presenta la memoria técnica correspondiente a las prácticas 2 y 3 de la asignatura Proyecto Hardware. A lo largo del documento se detalla el proceso de implementación del conocido juego Reversi. Partiendo del proyecto en lenguaje c previamente obtimizado en la práctica 1 se realizan una serie de modificaciones y adaptaciones, añadiendo nuevas funcionalidades y permitiendo que este pueda ser ejecutado en un emulador de máquina real con sus perifericos, como pueden ser botones, leds, teclado, pantalla, etc. 

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
## WT
