

#include <stdio.h>
#include "fix16.h"

int main()
{
        fix16_t result = fix16_sdiv(0xfffe0000,0xffff0000);
        printf("%f/%f = %f\n", fix16_to_float(0xfffe0000), fix16_to_float(0xffff0000), fix16_to_float(result));
        return 0;
}
