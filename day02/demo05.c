//in reverse order  
#include <stdio.h>
int main()
{
    float fahrenheit;
    for (float i = 300; i > 0; i = i - 20)
    {
        fahrenheit = (i * 9.0 / 5.0) + 32;
        printf("%.2f \t %.2f\n ", i, fahrenheit);
    }
    return 0;
}