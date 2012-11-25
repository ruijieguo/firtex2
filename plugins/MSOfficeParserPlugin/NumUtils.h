#ifndef __NUMUTIL_H
#define __NUMUTIL_H


class NumUtils
{
public:
    static unsigned int getshort(unsigned char *buffer,int offset);
    static long int getlong(unsigned char *buffer,int offset);
    static unsigned long int getulong(unsigned char *buffer,int offset);
};

#endif
