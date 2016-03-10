## Floating point exception

1 - ¿Qué función requiere agregar `-DTRAPFPE`? ¿Cómo pueden hacer que el
programa *linkee* adecuadamente?
2 - Para cada uno de los ejecutables, ¿qué hace agregar la opción `-DTRAPFPE` al compilar? ¿En qué se diferencian 
los mensajes de salida con y sin esa opción?

1 -Si quiero llamar a la funcion   set_fpe_x87_sse_(); necesito definir TRAPFPE (con -DTRAPFPE) para que incluya el .h correspondiente. Tengo que ponerle el path a donde está el header e incluir (tambien con path completo) la rutina .c que tiene la función en cuestión. La línea de compilación queda:

gcc -o test_fpe3.e test_fpe3.c ./fpe_x87_sse/fpe_x87_sse.c -lm -DTRAPFPE -I./fpe_x87_sse

2 - Define una macro (TRAPFPE) que, en caso de estar definida, incluye un header 
#ifdef TRAPFPE
#include "fpe_x87_sse.h"
#endif 
 y llama a una función
#ifdef TRAPFPE
  set_fpe_x87_sse_();
#endif
La diferencia está en que si cumplo alguno de los 3 casos que pretende chequear (FE_OVERFLOW o FE_DIVBYZERO o FE_INVALID) me avisa de la expeción de punto flotante y corta la corrida mientras que si no agrego el flag, me da NaN o inf como respuesta.

## Segmentation Fault

1 - ¿Devuelven el mismo error que antes?
2 - Averigüen qué hicieron al ejecutar la sentencia `ulimit -s unlimited`. Algunas pistas
son: abran otra terminal distinta y fíjense si vuelve al mismo error, fíjense la diferencia
entre `ulimit -a` antes y después de ejecutar `ulimit -s unlimited`, googleen, etcétera.
3 - La "solución" anterior, ¿es una solución en el sentido de debugging?

1, 2 y 3- No, el error se va. Con ulimit -s TAMAÑO le cmbio el tamaño al stack. En particular si TAMAÑO = unlimited le doy todo el espacio que necesite. Con ulimit -a puedo ver ese cambio en la variable (de entorno) stack size. Que el programa se rompa indica que hay un error (un seg fault es claramente un error). Solucionar los errores es debuggear. Pero el cambio que hay q hacer no es en el código así que no me decido. :S

## Valgrind

El error era un memory leak

==7115== LEAK SUMMARY:
==7115==    definitely lost: 0 bytes in 0 blocks
==7115==    indirectly lost: 0 bytes in 0 blocks
==7115==      possibly lost: 40,000,000 bytes in 1 blocks
==7115==    still reachable: 0 bytes in 0 blocks
==7115==         suppressed: 0 bytes in 0 blocks

Esto surje porque llamo a malloc muchas veces pero en el programa original libero la memoria una sola vez. Necesito liberarla tantas veces como llame a malloc. 
Para eso lo agregué al for. El print en pantalla lo pongo en un if (el último paso) para conservar la idea del programa. Como acá a[0] es siempre 7, no era necesario. Pero si en cada loop a dependiera, digamos, del anterior, sí era necesario ese if para quedarme siempre con el último valor de a, que es lo que imprimía originalmente. 

## Funny

Al definir el macro DEBUG se ocupa de armar un string (y le aloja la memoria!) que luego imprime en pantalla. Resulta sospechoso que al string le asigna 1024 lugares cuando según la asignación    
   a[i+1000] = a[i];
siempre van a "sobrarme" 1000 espacios respecto de mydim. En el caso que el sistema decida que la asignación de memoria sea continua, entonces los 1024 del char me van a "alcanzar" para cubrir los 1000 que me quiero pasar. En mi caso, el sistema decidió que no. Si en lugar de 1024 se hubieran asignado 900, ya no alcanza en ningún sistema operativo. 

