#include "packet.h"

/*
 * StuffData byte stuffs “length” bytes of data at the location pointed to by “ptr”,
 * writing the output to the location pointed to by “dst”.
 * http://www.stuartcheshire.org/papers/cobsforton.pdf
 */
#define FinishBlock(X) (*code_ptr = (X), code_ptr = dst++, code = 0x01 )

void StuffData(const unsigned char *ptr, unsigned long length, unsigned char *dst)
{
    const unsigned char *end = ptr + length;
    unsigned char *code_ptr = dst++;
    unsigned char code = 0x01;
    while (ptr < end)
    {
        if (*ptr == 0) FinishBlock(code);
        else
        {
            *dst++ = *ptr;
            code++;
            if (code == 0xFF) FinishBlock(code);
        }
        ptr++;
    }
    FinishBlock(code);
}

/*
 * UnStuffData decodes “length” bytes of data at the location pointed to by “ptr”,
 * writing the output to the location pointed to by “dst”.
 */
void UnStuffData(const unsigned char *ptr, unsigned long length, unsigned char *dst)
{
    const unsigned char *end = ptr + length;
    while (ptr < end)
    {
        int i, code = *ptr++;
        for (i=1; i<code; i++) *dst++ = *ptr++;
        if (code < 0xFF) *dst++ = 0;
    }
}
