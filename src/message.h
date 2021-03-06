#ifndef MSG_H_
#define MSG_H_

#include "stdint.h"

//消息类型
#define MSG_TYPE_CMD       1
#define MSG_TYPE_RS232    (1 << 1)
#define MSG_TYPE_RS485    (1 << 2)
#define MSG_TYPE_FINGER   (1 << 3)
#define MSG_TYPE_USB      (1 << 4)
#define MSG_TYPE_ETHERNET (1 << 5)
#define MSG_TYPE_TIMER    (1 << 6)
#define MSG_TYPE_BUTTON   (1 << 7)
#define MSG_TYPE_MF       (1 << 8)
#define MSG_TYPE_HID      (1 << 9)
#define MSG_TYPE_PROCMSG  (1 << 10)
#define MSG_TYPE_CAN      (1 << 11)
#define MSG_TYPE_T200MS   (1 << 12)

//命令
#define News_Exit_Program   1
#define News_Exit_Input     3

#define MSG_SIZE      5U
#define MSG_PROC_SIZE 5

#define MSG_GET_MSG(Entry,MsgType) ((Entry.EnabledMsg) & (MsgType))

//message infomation with struct
typedef struct {
    int32_t Message;
    int32_t Param1,Param2;
    void *Object;
} TMsg,*PMsg;

typedef int (*PMsgProc)(PMsg msg);

//mission process
typedef struct {
    uint16_t mHead;
    uint16_t mEnd;
    uint32_t MaxPorc;
    uint32_t EnabledMsg;
    PMsgProc pgm;//message catch function
    PMsgProc MessageProcs[MSG_PROC_SIZE]; //消息处理函数数组
    TMsg     MsgQueue[MSG_SIZE];
} TProcBuf,*PProcBuf;

void InitMsg(PProcBuf Entry,PMsgProc GetMsg);

void EnableMsgType(PProcBuf Entry,uint32_t MsgType,int32_t Enabled); //registe new message mask
uint32_t SelectNewMsgMask(PProcBuf Entry,uint32_t newmsk);   //new message mask

int32_t DoMsgProcess(PProcBuf Entry, void *Obj, int32_t ExitCommand);//run process
int32_t RegMsgProc(PProcBuf Entry,PMsgProc MsgProc);             //registe message process function
int32_t UnRegMsgProc(PProcBuf Entry,int32_t index);                  //unregiste message process function

int32_t PostMessage(PProcBuf Entry,int32_t Msg,int32_t P1,int32_t P2);       //post message

#endif

