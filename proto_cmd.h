#pragma once

#pragma pack(4)

/*
TCP通信，IP: , port:
数据包协议：
4字节数据包总长度(不包含自己)       1字节命令号  2字节数据内容长度       数据内容
|--------------------------------|-----------|------------------|---------------------------------
*/

const char k_CMD_REQ_OPEN_DOOR = 1;
struct ReqOpenDoor
{
	char serialNum[16];
};

const char k_CMD_RSP_OPEN_DOOR = 2;
struct RspOpenDoor
{
	char errorCode;   //0 为成功
	char msg[16];
};


const char k_CMD_ADD_CARD_SERIAL = 3;
struct ReqAddCard
{
	char cardSerialNum[16];
};

const char k_CMD_RMV_CARD_SERIAL = 4;
struct ReqRemoveCard
{
	char cardSerialNum[16];
};

const char k_CMD_RSP_MSG = 5;
struct RspMessage
{
	char cmdId;   
	char errorCode;   //0 为成功
	char msg[256];
};

/*
比如发送请求： ReqOpenDoor 因为有很多请求，需要标识请求的功能，所以定义了一个功能号 k_CMD_REQ_OPEN_DOOR =1

现在的问题是怎么把数据发个服务端

1、数据内容：即发送的卡号，如果卡号是 123， 则 copy 数组中，数组实际大小是 16，但是zhifuz
2、2字节数据长度：计算方式即sizeof(消息类型), 即ReqOpenDoor 的大小16
3、1字节命令号：计算方式 sizeof(char)
4、发生数据的总大小


*/