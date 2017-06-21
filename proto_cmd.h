#pragma once

#pragma pack(4)

/*
TCPͨ�ţ�IP: , port:
���ݰ�Э�飺
4�ֽ����ݰ��ܳ���(�������Լ�)       1�ֽ������  2�ֽ��������ݳ���       ��������
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
	char errorCode;   //0 Ϊ�ɹ�
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
	char errorCode;   //0 Ϊ�ɹ�
	char msg[256];
};

/*
���緢������ ReqOpenDoor ��Ϊ�кܶ�������Ҫ��ʶ����Ĺ��ܣ����Զ�����һ�����ܺ� k_CMD_REQ_OPEN_DOOR =1

���ڵ���������ô�����ݷ��������

1���������ݣ������͵Ŀ��ţ���������� 123�� �� copy �����У�����ʵ�ʴ�С�� 16������zhifuz
2��2�ֽ����ݳ��ȣ����㷽ʽ��sizeof(��Ϣ����), ��ReqOpenDoor �Ĵ�С16
3��1�ֽ�����ţ����㷽ʽ sizeof(char)
4���������ݵ��ܴ�С


*/