#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int contOctal = 0, contHexa = 0, contDecimal = 0, contOtra = 0;

//tabla de transicion
int tabla[8][7] = {
    // 0   [1-7]   [8-9]   [a-fA-F]    [xX]    +-   [cualquierOtraCosa]
    {1, 2, 2, 7, 7, 6 ,7}, // 0  => estado inicial
    {7, 5, 7, 7, 3, 7 ,7}, // 1  => octal o hexadecimal
    {2, 2, 2, 7, 7, 7 ,7}, // 2  => decimal, final
    {4, 4, 4, 4, 7, 7 ,7}, // 3  => intermedio al hexadecimal, NO final
    {4, 4, 4, 4, 7, 7 ,7}, // 4  => hexadecimal final
    {5, 5, 7, 7, 7, 7 ,7}, // 5  => octal final
	{2, 2, 2, 7, 7, 7 ,7}, // 6  => estado intermedio decimal signado
    {7, 7, 7, 7, 7, 7 ,7}  // 7  => estado de rechazo
};

int main(void)
{
    int estadoFinal;
    char str[250] = "0xFF&127&0159&0xaBb1&0Xx&0&010&09&127A&120&+999&-523&-4Afz";
    const char s[2] = "&";
    char* token;
    token = strtok(str, s);

    //mientras que haya una palabra para leer
    while (token != NULL)
	{
        estadoFinal = obtenerEstadoFinalDeCadena(token, tabla);

        guardarEstadoFinal(estadoFinal);

        token = strtok(NULL, s);
    }

    imprimirResultadosFinales();
}


int obtenerEstadoFinalDeCadena(char *cadena, int tabla[][7])
{
    int i = 0, fila = 0, columna = 0;
    int longitud = strlen(cadena); // obtengo longitud de la cadena
    char caracter = toupper(cadena[i]); //convierto los caracteres en mayuscula

    while(i < longitud)
    {
        switch(caracter)
        {
            // 0
            case '0': columna = 0;
                      fila = tabla[fila][columna];
                      break;

            // [1-7]
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7': columna = 1;
                      fila = tabla[fila][columna];
                      break;

            // [8-9]
            case '8':
            case '9': columna = 2;
                      fila = tabla[fila][columna];
                      break;

            // [a-fA-F]
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F': columna = 3;
                      fila = tabla[fila][columna];
                      break;

            // [xX]
            case 'X': columna = 4;
                      fila = tabla[fila][columna];
                      break;

            //[+,-]
			case '+':
			case '-': columna = 5;
					   fila = tabla[fila][columna];
					   break;
			//cualquier otra cosa
            default: columna = 6;
                     fila = 7;
                     break;
        }

        i++; // avanzo al siguiente caracter
        caracter = toupper(cadena[i]);
    }
	return fila;
}

void guardarEstadoFinal(int estadoFinal)
{
    // Segun el estado final, se identifica el tipo de constante y se le suma uno al contador
    switch (estadoFinal)
    {
		case 1:
		case 5: /* Octal */
			contOctal++;
		break;

		case 2:
		case 6: /* Decimales */
			contDecimal++;
        break;

		case 4: /* Hexadecimales */
			contHexa++;
		break;

		default: /* Estado de Rechazo */
			contOtra++;
        break;
    }
}

void imprimirResultadosFinales()
{
    printf("La cantidad de octales son : %d\n", contOctal);
    printf("La cantidad de hexadecimales son : %d\n", contHexa);
	printf("La cantidad de decimales son : %d\n", contDecimal);
	printf("La cantidad de palabras no reconocidas son : %d\n", contOtra);
}

