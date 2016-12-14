
extern struct PROCESS_SIGNAL_DATE g_sMainProcessSigDate;//�������ź��йر���
#include "signal.h"
/*****************************************************
 * ����	 : SigSet
 *
 * ����  : ���ý����źź�Ķ���
 *
 * ����  : 1�����ܵ��ź�  1�����͸����׽���
 *			 
 *
 * ����ֵ: ��
 *****************************************************/
void SigSet(int nSigNum, void (*OnSigHandler)(int sig))
{
	struct sigaction sAct;
	memset( &sAct, '\0', sizeof(sAct));
	sAct.sa_handler = OnSigHandler;
	sAct.sa_flags |= SA_RESTART;
	sigfillset( &sAct.sa_mask );
	assert(sigaction(nSigNum, &sAct, NULL) != -1 );
}

/*****************************************************
 * ����	 : SIGUSR1Handler
 *
 * ����  : ����SIGUSR1�ź�
 *
 * ����  : ��
 *			 
 *
 * ����ֵ: ��
 *****************************************************/
void SIGUSR1Handler()
{
	cout << "get signals SIGUSRl" << endl;
}

/*****************************************************
 * ����	 : SIGPIPEHandler
 *
 * ����  : ����SIGHUP�ź�
 *
 * ����  : ��
 *			 
 *
 * ����ֵ: ��
 *****************************************************/
void SIGPIPEHandler()
{
	cout << "get signals SIGPIPE" << endl;
}

/*****************************************************
 * ����	 : SIGHUPHandler
 *
 * ����  : ����SIGHUP�ź�
 *
 * ����  : ��
 *			 
 *
 * ����ֵ: ��
 *****************************************************/
void SIGHUPHandler()
{
	cout << "get signals SIGHUP" << endl;
}

/*****************************************************
 * ����	 : ��������ͳһ�����ź�����
 *
 * ����  : ����SIGUSR1�ź�
 *
 * ����  : ��
 *			 
 *
 * ����ֵ: �Ƿ�ɹ�����  0-�� 1-��
 *****************************************************/
int MainProcessSignalProcessing(int nSockFd)
{
	char gcSignals[1024];
	memset(gcSignals, 0x00, sizeof(gcSignals));

	int nRet = recv(nSockFd, gcSignals, sizeof(gcSignals), 0);
	if(nRet == -1 ){//��ȡʧ��
		return 0;
	}
	else if( nRet == 0 ){//δ��������
		return 0;
	}
	else{
		for (int j=0; j < nRet; j++){
			switch (gcSignals[j]){
				case SIGUSR1:
					SIGUSR1Handler();
					break;
				case SIGHUP:
					SIGHUPHandler();
					break;
				case SIGPIPE:
					SIGPIPEHandler();
					break;
				default:

				break;
			}
		}
	}
}

/*****************************************************
 * ����	 : SigHandler
 *
 * ����  : �����źź��͸��׽���
 *
 * ����  : 1�����ܵ��ź�  1�����͸����׽���
 *			 
 *
 * ����ֵ: ��
 *****************************************************/
void MainProcessSigHandler (int nSigNum)
{
	// ����ԭ����errno����֤����������
	int nSaveErrno = errno;
	int nSig = nSigNum;
	send(g_sMainProcessSigDate.m_nWrFd, (char *)&nSig, sizeof(char), 0);
	cout << "receive signals" << endl;
	errno = nSaveErrno;
}
/*****************************************************
 * ����	 : MainProcessSigInit
 *
 * ����  : �����źŴ����ʼ��
 *
 * ����  : 1.�ӷ��׽���
 *			 
 *
 * ����ֵ: ��
 *****************************************************/
void ProcessSigInit(struct PROCESS_SIGNAL_DATE *pfsSigData)
{
	int nRet = socketpair(PF_UNIX, SOCK_STREAM, 0, pfsSigData->m_gnPipeFd);
	assert(nRet != -1 );
	
	SetNonblocking(pfsSigData->m_gnPipeFd[1]);
	SetNonblocking(pfsSigData->m_gnPipeFd[0]);
	pfsSigData->m_nRdFd = pfsSigData->m_gnPipeFd[0];
	pfsSigData->m_nWrFd = pfsSigData->m_gnPipeFd[1];
	
	SigSet(SIGUSR1, MainProcessSigHandler);	//��׽SIGUSR1�ź�
	SigSet(SIGHUP, MainProcessSigHandler);	//��׽SIGHUP�źţ��ն��˳�
	SigSet(SIGPIPE, MainProcessSigHandler);	//��׽SIGPIPE�źţ�����Ĺܵ���д
	
}