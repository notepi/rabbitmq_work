#ifndef __SIGNAL_H__
#define __SIGNAL_H__
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include "IPv4socket.h"
using namespace std;

//�����źŴ����й�
struct PROCESS_SIGNAL_DATE
{
	int m_gnPipeFd[2];//���ڱ��洴���ı����׽���
	int m_nWrFd;
	int m_nRdFd;
};


void SigSet(int nSigNum, void (*OnSigHandler)(int sig));
int  MainProcessSignalProcessing(int nSockFd);
void MainProcessSigHandler (int nSigNum);
void ProcessSigInit(struct PROCESS_SIGNAL_DATE *pfsSigData);



#endif

