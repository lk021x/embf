#ifndef OBJCOMM_H
#define OBJCOMM_H

#include <stdint.h>

#define PKG_DATA_BUF_SIZE   128 //window size

#define PKG_ER_OK           0
#define PKG_ER_CHECK        1
#define PKG_ER_SERIAL       2
#define PKG_ER_DIR          4
#define PKG_ER_LENGTH       8
#define PKG_ER_CONSUMMATION 16
typedef struct {
    uint16_t addr;    //package address
    uint8_t cyp:1;    //cryption
    uint8_t dir:1;    //direction
    uint8_t err:6;    //error
    uint8_t ver;      //ver
}TObjc_Head;

typedef struct {
    uint16_t obj;
    uint16_t opera;
}TObjc_obj_opera;

typedef __packed struct ott{
    uint32_t year:14;
    uint32_t mon:4;
    uint32_t mday:5;
    uint32_t wday:4;
    uint32_t hour:5;
    uint32_t min:6;
    uint32_t sec:6;
    uint32_t usec:20;
}TObjc_Time;

#define OBJC_TRUE  1
#define OBJC_FALSE 0
typedef uint8_t Objc_bool;

#define OBJC_NULL   0
#define OBJC_INT8   1
#define OBJC_UINT8  2
#define OBJC_INT16  3
#define OBJC_UINT16 4
#define OBJC_INT32  5
#define OBJC_UINT32 6
#define OBJC_INT64  7
#define OBJC_UINT64 8
#define OBJC_FLOAT  9
#define OBJC_DOUBLE 10
#define OBJC_BOOL   11
#define OBJC_BITS   12
#define OBJC_DTIME  13
typedef union {
    __packed struct a{
        uint16_t  type:6;
        uint16_t  len:10;
        Objc_bool var[];
    }varb;
    __packed struct b{
        uint16_t  type:6;
        uint16_t  len:10;
        int8_t    var[];
    }var8i;
    __packed struct c{
        uint16_t  type:6;
        uint16_t  len:10;
        uint8_t   var[];
    }var8u;
    __packed struct d{
        uint16_t  type:6;
        uint16_t  len:10;
        int16_t   var[];
    }var16i;
    __packed struct e{
        uint16_t  type:6;
        uint16_t  len:10;
        uint16_t  var[];
    }var16u;
    __packed struct f{
        uint16_t  type:6;
        uint16_t  len:10;
        int32_t   var[];
    }var32i;
    __packed struct g{
        uint16_t  type:6;
        uint16_t  len:10;
        uint32_t  var[];
    }var32u;
    __packed struct h{
        uint16_t  type:6;
        uint16_t  len:10;
        int64_t   var[];
    }var64i;
    __packed struct i{
        uint16_t  type:6;
        uint16_t  len:10;
        uint64_t  var[];
    }var64u;
    __packed struct j{
        uint16_t  type:6;
        uint16_t  len:10;
        float    var[];
    }var32f;
    __packed struct k{
        uint16_t  type:6;
        uint16_t  len:10;
        double   var[];
    }var64d;
    __packed struct m{
        uint16_t  type:6;
        uint16_t  len:10;
        TObjc_Time var[];
    }varti;
}TObjc_Variant;

typedef struct {
    uint16_t type:6;
    uint16_t len:10;
}TObjc_Data;

typedef struct {
    uint32_t addr;
    uint32_t serial_pkg;
}objc_sys_data;

typedef struct {
    uint16_t obj;
    uint16_t opera;
    uint8_t  *param1;
    uint8_t len;
    uint8_t *sbuf;//send buffer
    uint8_t slen;
}TExCallParam;
typedef int (*ExCallBack)(TExCallParam *ap,uint8_t *err);

int objc_analyst(uint8_t *in_buf,int len,uint8_t *out_buf,int slen,ExCallBack ex_callback);

extern objc_sys_data g_osd;

#endif

