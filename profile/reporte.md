profile_me_1.c

Primero tuve que resolver el tema de la asignación de espacio al stack. Con ulimit -s unlimited se solucionó. Después probé hacer el profiling con gprof. Encontré que la función first_assign tarda unas 3 veces más que la second_assign. Esto es poruqe en la primera, está queriendo acceder al "vector largo de la matriz" pegando saltos muy grandes (de tamaño SIZE). En la otra función, sigue el orden secuencial de la asignación de memoria con lo cual el acceso es mucho más rápido. Probé, además, con distintas optimizaciones a ver cómo afectaba el tiempo de cálculo:

----------------------------------------------------------------------------------
-o0

real	0m12.117s
user	0m11.793s
sys	0m0.256s

time   seconds   seconds    calls  ns/call  ns/call  name    
 73.70      7.68     7.68 144000000    53.33    53.33  first_assign
 16.29      9.38     1.70                             main
 10.32     10.45     1.07 144000000     7.46     7.46  second_assign
  0.14     10.47     0.02                             frame_dummy

----------------------------------------------------------------------------------
-01

real	0m9.801s
user	0m9.517s
sys	0m0.268s

 time   seconds   seconds    calls  ns/call  ns/call  name    
 87.62      6.99     6.99 144000000    48.56    48.56  first_assign
  8.77      7.69     0.70                             main
  3.85      8.00     0.31 144000000     2.13     2.13  second_assign
  0.51      8.04     0.04                             frame_dummy
----------------------------------------------------------------------------------
-02

real	0m0.009s
user	0m0.000s
sys	0m0.002s

Each sample counts as 0.01 seconds.
 no time accumulated
----------------------------------------------------------------------------------

A partir de la optimización O2 ya no puedo medir el tiempo. Además encontramos que si no se le pide ningún output al programa es mucho más rápido que pidiendole, por ejemplo, que imprimima algún elemento del medio de la matriz. Es decir, la optimización de nivel 2 es suficientemente astuta para darse cuenta que si no le pido algún output del programa, no es necesario hacer todo el cálculo!

Haciendo lo mismo con perf stat obtengo información complementaria, como la cantidad de instrucciones y la cantidad de stalled cycles. Si no optimizo el código veo:

 Performance counter stats for './profile_me_1.e':

      12086,420666 task-clock (msec)         #    0,998 CPUs utilized          
             1.850 context-switches          #    0,153 K/sec                  
                11 cpu-migrations            #    0,001 K/sec                  
             2.271 page-faults               #    0,188 K/sec                  
    44.349.423.499 cycles                    #    3,669 GHz                    
    29.332.074.087 stalled-cycles-frontend   #   66,14% frontend cycles idle   
    23.033.231.780 stalled-cycles-backend    #   51,94% backend  cycles idle   
    43.444.275.322 instructions              #    0,98  insns per cycle        
                                             #    0,68  stalled cycles per insn
     4.480.802.955 branches                  #  370,730 M/sec                  
           102.420 branch-misses             #    0,00% of all branches        

      12,111829521 seconds time elapsed

Donde tengo una animalada de instrucciones (aclaración: cambié SIZE en el código por 12000) y ciclos parados. Si pongo -O2

 Performance counter stats for './profile_me_1.e':

          0,680833 task-clock (msec)         #    0,096 CPUs utilized          
                 8 context-switches          #    0,012 M/sec                  
                 0 cpu-migrations            #    0,000 K/sec                  
               138 page-faults               #    0,203 M/sec                  
         1.380.018 cycles                    #    2,027 GHz                    
           940.268 stalled-cycles-frontend   #   68,13% frontend cycles idle   
           761.348 stalled-cycles-backend    #   55,17% backend  cycles idle   
           865.211 instructions              #    0,63  insns per cycle        
                                             #    1,09  stalled cycles per insn
           168.061 branches                  #  246,846 M/sec                  
            11.423 branch-misses             #    6,80% of all branches        

       0,007115753 seconds time elapsed

y veo que la cantidad se reduce muchisimo! :)

profile_me_2.c

Primero hubo que aregar un chequeo de la cantidad de argumentos pasados por la línea de comandos para evitar errores por olvidar poner la cantidad de argumentos. Encontramos cierta inconsistencia entre lo que sale de time y gprof

real	0m6.369s
user	0m6.185s
sys	0m0.176s
  
%   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
100.71      1.61     1.61                             main

Aunque no pueda compararse 1 a 1 los tiempos, dan muy distintos! No sé por qué esta diferencia. 
También vimos que no hay grandes diferencias con las distintas optimizaciones. (Aclaración: comentamos la parte de la exponencial que es la que, para dim muy grandes da NaN y, entiendo, que para el programa le resulta mucho más rápido lidiar con NaNs que hacer operaciones con números!)



