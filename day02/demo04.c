//fahrehiets to celcius
#include<stdio.h>
int main()
{
    int step = 20;
    int lower = 0;
    int upper = 300;
    float fahrenheit;
    float celsius;
    printf("%%", upper);
    fahrenheit = lower;
    while (fahrenheit <= upper)
    {
        celsius = (fahrenheit - 32) * 5 / 9;
        printf("%3.1f\t%6.1f\n", fahrenheit, celsius);
        fahrenheit += step;
    }
    return 0;
}
