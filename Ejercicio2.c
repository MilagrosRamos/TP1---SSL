#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

int resultado = 0, encontrado = 0;

int tabla[5][4] = {
    // [0-9]   [+,-] * [cualquierOtraCosa]
    {2, 1, 4, 4}, // 0  => estado inicial
    {2, 4, 4, 4}, // 1 => estado intermedio
    {2, 3, 3, 4}, // 2  => estado final
    {2, 3, 4, 4}, // 3 => estado intermedio en el que debo hacer la suma, resta o multiplicacion
    {4, 4, 4, 4}, //4 => estado de rechazo
};

int cuentita(char op, int lhs, int rhs);

int main(void)
{
    int estadoFinal;
    bool valido;
    char str[250] = "15+4*5=-6=9+2*7-9=-98=-3+89+-2*2=-2*2";
    const char s[2] = "="; // = lo tomamos como el centinela
    char* token;
    token = strtok(str, s);

    //mientras que haya una palabra para leer
    while (token != NULL)
	{
	    estadoFinal = obtenerValor(token, tabla);

        if(Verifica(estadoFinal)) //si es decimal realiza la operacion
        {
            resultado = realizarCuenta(token);
            printf("%s=%d \n", token, resultado);
        }else  printf("%s no es una operacion admitida \n", token);

        token = strtok(NULL, s);
    }
}


int obtenerValor(char *cadena, int tabla[][4])
{
    int i = 0, fila = 0, columna = 0;
    int longitud = strlen(cadena); // obtengo longitud de la cadena
    char caracter = toupper(cadena[i]);

    while(i < longitud)
    {
        switch(caracter)
        {
            // [0-9]
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': columna = 0;
                      fila = tabla[fila][columna];
                      break;

            // +
			case '+':
			case '-':  columna = 1;
					   fila = tabla[fila][columna];
					   break;
			// *
			case '*': columna = 2;
                      fila = tabla[fila][columna];
                      break;

			//cualquier otra cosa
            default: columna = 3;
                     fila = 4;
                     break;
        }

        i++; // avanzo al siguiente caracter
        caracter = toupper(cadena[i]);
    }

	return fila;
}

int Verifica(int estadoFinal)
{
    switch (estadoFinal)
    {
		case 2: /* Decimales*/
			return 1;
		break;

		default: /* Estado de Rechazo */
		    return 0;
        break;
    }

}

int realizarCuenta(char *str)
{
    char *p;
    int lhs, rhs, longitud,n,m,valor1, valor2;
    int r = 0, i = 0, j=0,encontrado = 0,encontradoSinCaracter = 0, largo = 0;
    char aux[200]= "", destino [100],op;

    /*busco si hay alguna multiplicacion, ya que es lo que tiene que realizar primero */
    int longi = strlen(str);
    while(j < longi){
        if(str[j] == '*') {
           encontrado = 1;


           valor1 = str[j-1] - '0';
           valor2 = str[j+1] - '0';
           r = cuentita('*', valor1, valor2);

           for(n=0; n < (j-1); n++)
           		strncat(aux,&str[n],1);

           itoa(r,destino,10);
           largo = strlen(destino);
           strncat(aux,destino,largo);

           for(m = (j+2); m < strlen(str); m++)
            strncat(aux, &str[m],1);

           j++;

        }else j++;
    }

    //encontrado es 1 cuando hay una multiplicacion, entonces ahi tomo la cadena armada en esa parte
    if(encontrado == 1) lhs = strtol(aux, &p, 10);
    else lhs = strtol(str, &p, 10);

    while(*p == ' ') p++;
    op = *p++; // obtiene los signos si es q lo tiene



    if(op == '\0'){ // si op que es el obtiene los operandos esta vacio estamos en la situacion ejemplo: +1 , -15
      rhs = 0;
      encontradoSinCaracter = 1;
    }
    else rhs = strtol(p, &p, 10);
    r = cuentita(op,lhs,rhs);

    longitud = strlen(p);
    while(longitud > 0 && encontradoSinCaracter != 1) {
        op = *p++;
        longitud--;
        rhs = strtol(p, &p, 10);
        longitud--;
        r = cuentita(op,r,rhs);

    }
    return r;
}

int cuentita (char op, int lhs, int rhs){
    int r = 0;
    switch(op) {
            case '*':
                r = lhs * rhs;
                break;
            case '+':
                r = lhs + rhs;
                break;
            case '-':
                r = lhs - rhs;
                break;

            default: /*cuando no hay ningun operando entre medio de los operadores, ejemplo: -9, +6*/
                r = lhs;
                break;
        }

    return r;
}

