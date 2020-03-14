#include "swap.h"

void Swap(char *left, char *right)
{
	// ваш код здесь
    *right = *left + *right; //  = a +b
    *left = *right - *left;  // = 
    *right = *right - *left; //


}
