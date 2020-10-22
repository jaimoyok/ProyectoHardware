# ProyectoHardware
Practicas, 5ºsemestre

[Memoria](https://docs.google.com/document/d/1xSNtcLDT8_QaK9UT04fA6OzAKEv2U6cILV34ajthANU/edit?usp=sharing)

[Resumen ejecutivo](https://docs.google.com/document/d/17F31g74t_EruIJu3Af_9ThgFw-J-p17KdCblboxLC5o/edit?usp=sharing)

|Versión|Tiempo (us) | Nº Instrucciones | Tamaño (bytes)|
|:--|:---:|:--:|:--:|
|O0|9.967 + 4.700||52||23|
|O0 -otime|9.967 + 4.700||52||23|
|O1|8.933 + 3050||48||17|
|O1 -otime|8.933 + 3050||48||17|
|O2|8.017 + 3100||36||16|
|O2-otime|8.017 + 0 ||60||17|
|O3|8.017 + 3.100||36||16|
|O3-otime|7.117 + 0||54||17|
|arm_c|8.817 + (depende de la optimizacion)||38||(depende de la optimizacion)|
|arm_arm|**6.900**|**83**|43 (incluye ficha_valida)||0(no existe ficha_valida)|




# arm_c

* Quitar lr y usamos la cima como puntero para leer parametros
* Como longitud siempre es 0 nos ahorramos la lectura
* Longitud la guardamos solamente al final y no en todos los bucles
* Si si la ficha no es valida, casilla es siempre 0, por lo que para calcular el valor de ficha valida comparamos casilla con 0 el lugar de leer de memoria.
* Usar condiciones para ahorrar saltos
* Operaciones lectura escritura multiples

# arm_arm

* No apilamos pc porque lr no se modifica
* Como longitud siempre es 0 nos ahorramos la lectura
* Longitud la guardamos solamente al final y no en todos los bucles
* No utilizamos registro para guardar ficha valida lo que evita apilar
* Usar condiciones para ahorrar saltos
* Lecturas multiples
* Operaciones lectura escritura multiples
* Desplazamientos
