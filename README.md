# ProyectoHardware
Practicas, 5ºsemestre

[Memoria](https://docs.google.com/document/d/1xSNtcLDT8_QaK9UT04fA6OzAKEv2U6cILV34ajthANU/edit?usp=sharing)

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
