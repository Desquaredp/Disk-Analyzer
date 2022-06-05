#include "util.h"
#include<stdlib.h>
#include<string.h>
#include "logger.h"
#include <math.h>
#include<time.h>
/**
 * Converts a size in bytes to human-readable format.
 *
 * @param buf The buffer to write the converted string to
 * @param buf_sz Size of 'buf'
 * @param size Size in bytes to be converted to a human-readable string
 * @param decimals number of digits after the decimal place
 */
void human_readable_size(char *buf, size_t buf_sz, double size, unsigned int decimals)
{
    
    float temp = size;
    char mem[8][5] = { " B"," KiB", " MiB", " GiB", " TiB", " PiB", " EiB", " ZiB"};
    char pres[4];
    strcpy(pres, "%.");
    char tempchar[2] = {0};
    sprintf(tempchar,"%u", decimals);
    strcat(pres, tempchar);
    strcat(pres, "f");
    float temptemp = temp;
    for(int i = 0; i <= 7; i++){
        if((int)(temptemp / pow(1024,(i+1))) == 0){
            if(i == 0){
                sprintf(buf,pres, temp);
                strncat(buf, mem[i], 5);
                break;
            }else{
                temp = temp/pow(1024,i);
                sprintf(buf,pres, temp);
                strncat(buf, mem[i], 5);
                break;
            }
        }
    }
}

size_t simple_time_format(char *buf, size_t buf_sz, time_t time)
{
    struct tm *lt;
    lt = localtime(&time);
    strftime(buf, buf_sz, "%b %d %Y", lt); 

    return 0;
  
    }
