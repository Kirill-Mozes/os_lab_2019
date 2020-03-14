#include "revert_string.h"

void RevertString(char *str)
{
	// your code here
     char a;
    int leng = 0;
    while((*(str+leng)!= 0)) 
    { 
        ++leng;
    }

    int i;
    for( i=0;i<leng/2;i++)
       {      char temp=str[i];
             str[i]=str[leng-1-i];
             str[leng-1-i]=temp;
       }
}

