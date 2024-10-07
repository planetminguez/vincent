/*
* Copyright 2020, @planetminguez
* gcc stuff.c -o program -g
*/
//Taking stuff out

str2hex(int buflen, unsigned char *buf, const char *str)
{
    unsigned char *ptr = buf;
    int seq = -1;
    while (buflen > 0) {
        int nibble = *str++;
        if (nibble >= '0' && nibble <= '9') {
            nibble -= '0';
        } else {
            nibble |= 0x20;
            if (nibble < 'a' || nibble > 'f') {
                break;
            }
            nibble -= 'a' - 10;
        }
        if (seq >= 0) {
            *buf++ = (seq << 4) | nibble;
            buflen--;
            seq = -1;
        } else {
            seq = nibble;
        }
    }
    return buf - ptr;
}

/////////////

#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
#include <sys/time.h>
#endif


#include <stdio.h>
// test again
int main(int argc, char **argv){
    
        printf("\t\tPlanetminguez\n");
        printf("\t\tChange System mount as a ro to rw \n");
        printf("\t\tChange System mount as a ro to rw....\n");
        return 0;
    }
//annnnnnn
