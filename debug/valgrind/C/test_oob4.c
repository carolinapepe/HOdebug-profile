#include <stdlib.h>
#include <stdio.h>

#define DIM 10000000

void mysub(float **a, int dim)
{
  int j;
  
  (*a) = (float *)malloc(sizeof(float)*dim);
  
  for(j=0; j<dim; j++)
    {
      (*a)[j] = 7.;
    }
  
}
     


int main(int argc, char *argv[])
{
  float *a;
  int i;
  int mydim = DIM;
  int last;

  printf("Insert last \n");
  scanf("%d",&last);
  for(i=0; i<last; i++)
    {
      mysub(&a, mydim);
      if( i == last -1)
	printf("a = %f \n", a[0]);
    free(a);
    }
/* por cada vez que llamo a malloc , libero la memoria correspondiente*/
/* el print lo hago en el ultimo paso del for para seguir con la idea de quedarme
 con el ultimo valor de a. */
  return(EXIT_SUCCESS);
}
