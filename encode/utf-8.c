#include <mydebug.h>
#include <encode.h>
#include <stdio.h>
#include <unistd.h>

static int Utf8GetEncode(unsigned int * onefontencode,unsigned char * current_addr);

static T_EncodeOpr gt_utf8opr={
    .name="utf-8",
    .GetEncode=Utf8GetEncode,
};

static int Utf8GetEncode(unsigned int * onefontencode,unsigned char * current_addr)
{
    int i;
    int bytes=0;
    unsigned char headbyte=0;
    unsigned int encode_tmp=0;
    headbyte=current_addr[0];
    if(!((headbyte >> 7)&0x1))
    {
        *onefontencode = headbyte;
        bytes = 1;
        return bytes;
    }else{
            while((headbyte >> 7) & 0x1)
            {
                headbyte = headbyte << 1;
                bytes++; 
            }
    }

    for(i=0 ; i<bytes ;i++)
    {
        if(0==i)
        {
            switch (bytes)
            {
                case 2: encode_tmp |= (current_addr[i] & 0x1f);break;
                case 3: encode_tmp |= (current_addr[i] & 0x0f);break;
                case 4: encode_tmp |= (current_addr[i] & 0x07);break;
                case 5: encode_tmp |= (current_addr[i] & 0x03);break;
                case 6: encode_tmp |= (current_addr[i] & 0x01);break;
            }
            continue;
        }
        encode_tmp = encode_tmp << 6;
        encode_tmp |= (current_addr[i] & 0x3f);
    }
    *onefontencode = encode_tmp;
    return bytes;
}

int Utf8Register()
{
    return EncodeRgister(&gt_utf8opr);
}

