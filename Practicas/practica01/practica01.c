/*
Autor :
Alex Gerardo Fernandez Aguilar - 314338097
*/
#include <stdio.h>

//funcion que calcula el area de un triangulo
float area (int base , int altura){
    return ((base*altura)/2);
}

//funcion que revisara si es primo y regresara 1 un si y 0 si no 
int es_primo(int numero){
    int numero, flag = 0;
    
    if (numero == 1 || numero < 0){
        return 0;
    }
    for (int i = 2; i <= numero / 2; ++i) {
        if (numero % i == 0) {
        flag = 1;
        break;
        }
    }
    return (flag == 0) ? 1 : 0;
}

//funcion que imprimira una lista n de fibonacci
void fibonacci (int numero){
    unsigned long long int f1 =1 , f2 =1 ,fibo = f1 + f2;

    switch (numero)
    {
    case 1:
        printf("%llu", f1);
        break;

    case 2:
        printf("%llu, %llu", f1, f2);
        break;
    
    default:
        printf("%llu, %llu", f1, f2);
        for (int i = 3; i <= numero; ++i) {
            printf(", %llu", fibo);
            f1 = f2;
            f2 = fibo;
            fibo = f1 + f2;
        }
        break;
    }
    printf("/n");
}

//funcion imprime quien gano
void piedra_papel_tijera (char jugador1[], char jugador2[]){
    if (jugador1 == jugador2){
        printf("EMPATE /n");
    }else if( (jugador1=="tijera" && jugador2=="papel" ) 
    || (jugador2=="tijera" && jugador1=="papel" ) ){
        printf("GANA LA TIJERA /n");
    }else if( (jugador1=="piedra" && jugador2=="tijeras" ) 
    || (jugador2=="piedra" && jugador1=="tijeras" ) ){
        printf("GANA LA PIEDRA /n");
    }else if( (jugador1=="papel" && jugador2=="piedra" ) 
    || (jugador2=="papel" && jugador1=="piedra" ) ){
        printf("GANA EL PAPEL /n");
    }
}

//main
void main (){
    //pruebas
    //Area de triangulo
    int a =3,b=4;
    printf("%.2f /n", ( area(a,b) ) );
    //segunda prueba
    a =12,b=10;
    printf("%.2f /n", ( area(a,b) ) );

    //prueba es Primo
    int p = 2;
    printf("%i /n", ( es_primo(p) ) );
    //segunda prueba
    p = 7001;
    printf("%i /n", ( es_primo(p) ) );

    //prueba fibonacci 
    int f = 2;
    fibonacci(f);
    //segunda prueba
    f = 55;
    fibonacci(f);

    //prueba piedra papel o tijera 
    char piedra[] ="piedra",papel[] ="papel",tijera[]="tijera";
    piedra_papel_tijera(piedra,papel);    
    //segunda prueba
    piedra_papel_tijera(tijera,tijera);

    }