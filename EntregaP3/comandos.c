#include <string.h>
#include "comandos.h"
#include "Eventos.h"
#include "UART0.h"

static char comando[DIM_COMANDO];

int checksum() {
  return (comando[0] - '0' + comando[1] - '0') % 8 == comando[2] - '0';
}

void buscar_comando(char leido) {
	comando[3]='\0';
  static int i = 3;
  uint32_t dato;
  switch (leido) {
  case INICIO_COMANDO: // Comienzo de un comando '#'
    i = 0;
    break;
  case FINAL_COMANDO: // Final de un comando '!'
    if (i == 3) {
      if (strcmp(comando,"PAS") == 0)
        dato = PASAR;
			
      else if (strcmp(comando,"NEW") == 0)
        dato = NUEVA_PARTIDA;
			
      else if (strcmp(comando,"RST") == 0)
        dato = ACABAR_PARTIDA;
			
      else if (comando[0] >= '0' & comando[1] >= '0' & comando[2] >= '0' &
               comando[0] < '8' & comando[1] < '8' & comando[2] < '8') {
								 
        if (checksum()) {
          dato = ((comando[1] - '0') << 8) |
                 (comando[0] - '0'); // Conversion a formato de evento
					
        } else {
          print("Checksum invalido");
          dato = COMANDO_FALLIDO;
        }
				
      } else
        dato = COMANDO_FALLIDO;
			
    } else
      dato = COMANDO_FALLIDO;
		
    cola_guardar_eventos(EV_COMANDO, dato);
    break;
		
  default: // Letra del comando
    if (i < DIM_COMANDO) {
      comando[i] = leido;
      i++;
    }
  }
}
