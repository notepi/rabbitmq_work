#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

#include "IPv4socket.h"

/*	�������ܣ������ͻ��˶�IPV4��socket������
	������1��������ip��ַ������ַΪNULLʱ���������С�
		  2�������Ķ˿�
	���أ�socket��fd			
*/
int CreatIpv4TcpCleintSocket(const char* pfcIp, int nPort)
{
	struct sockaddr_in sServerAddress;
    bzero(&sServerAddress, sizeof(sServerAddress));
    sServerAddress.sin_family = AF_INET;
    inet_pton(AF_INET, pfcIp, &sServerAddress.sin_addr);
    sServerAddress.sin_port = htons(nPort);

    int nSockFd = socket(PF_INET, SOCK_STREAM, 0);
    if(nSockFd < 0){
    		printf ("failed to creat socket!\n");
    		return -1;
    }
    if (connect(nSockFd, (struct sockaddr*)&sServerAddress, sizeof(sServerAddress) ) < 0){
	        printf("connection failed\n");
	        close(nSockFd);
	        return -1;
	}
	return nSockFd;
}


/*	�������ܣ�������������IPV4��socket������
	������1��������ip��ַ������ַΪNULLʱ���������С�
		  2�������Ķ˿�
		  3���������г�Ա��
	���أ�listen��fd			
*/

int CreatIpv4TcpServerSocket(const char* pfcIp, int nPort, int nBacklog)
{
	int nRet;
	int nSockFd = socket(AF_INET, SOCK_STREAM, 0);		// ����TCP��socket
	if(nSockFd < 0){
		printf("failed to call socket!\n");
		// �������ʧ�ܣ���ӡ����ԭ�򲢷���
		printf("errno value: %d, it means: %s",errno, strerror(errno));
		return -1;
	}
	
	// ����һ��IPv4 socket��ַ
	struct sockaddr_in sAddress;
	bzero( &sAddress, sizeof(sAddress) );				// �ṹ������
	sAddress.sin_family = AF_INET;						// TCP/IPv4Э����
	sAddress.sin_port = htons(nPort);					// ���˿�ת�������ֽڴ���sin_port
	if (NULL == pfcIp){									// ��ַΪ�����������
		sAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else{												// �����ض�IP
		inet_pton(AF_INET, pfcIp, &sAddress.sin_addr);	// ��ipת�������ֽ������sin_addr
	}
	
	nRet = bind(nSockFd, (struct sockaddr*)&sAddress, 	// ��socket
					sizeof(sAddress));
	if(nRet < 0){
		printf("failed to call bind!\n");
		// �����ʧ�ܣ���ӡ����ԭ�򲢷���
		printf("errno value: %d, it means: %s\n",errno, strerror(errno));
		return -1;
	}
	
	nRet = listen(nSockFd, nBacklog);					// ������backlog������
	if(nRet < 0){
		printf("failed to call listen!\n");
		// �������ʧ�ܣ���ӡ����ԭ�򲢷���
		printf("errno value: %d, it means: %s",errno, strerror(errno));
		return -1;
	}
	
	return nSockFd;										// ���ؼ���
}


/*	�������ܣ�����IPV4��socket���󶨷���
	������1��������ip��ַ������ַΪNULLʱ���������С�
		  2�������Ķ˿�
		  3���������г�Ա��
	���أ�listen��fd			
*/
int CreatIpv4UdpSocket( const char* pfcIp, int nPort)
{
	
	int nSockFd = socket(AF_INET, SOCK_DGRAM, 0);			// ����UDP��socket
	if(nSockFd < 0){
		printf("failed to call socket!\n");
		// �������ʧ�ܣ���ӡ����ԭ�򲢷���
		printf("errno value: %d, it means: %s",errno, strerror(errno));
		return -1;
	}
	
	// ����һ��IPv4 socket��ַ
	struct sockaddr_in sAddress;
	bzero(&sAddress, sizeof(sAddress));						// �ṹ������
	sAddress.sin_family = AF_INET;							// TCP/IPv4Э����
	if (NULL == pfcIp){										// ��ַΪ�����������
		sAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else{													// �����ض�IP
		inet_pton(AF_INET, pfcIp, &sAddress.sin_addr);		// ��ipת�������ֽ������sin_addr
	}
	sAddress.sin_port = htons(nPort);						// ���˿�ת�������ֽڴ���sin_port
	
	int nRet = bind (nSockFd, (struct sockaddr*)&sAddress, 	// ��socket
				sizeof(sAddress));
	if (nRet < 0){
		// ��ʧ�����˳�����ӡ����ԭ�򲢷���
		printf("errno value: %d, it means: %s",errno, strerror(errno));
		return -1;
	}
	
	return nSockFd;
	
}

/*���ļ����������óɷ�������*/
/*	�������ܣ����ļ����������óɷ������ģ��������ļ�������ԭʼ״̬
	������1���ļ�������
	���أ�ԭʼ�ļ�������			
*/
int SetNonblocking(int nFd)
{
	int nRet;
	int nOldOption = fcntl(nFd, F_GETFL);					// ��ȡ��fd�ı�־
	int nNewOption = nOldOption | O_NONBLOCK;				// fd����Ϊ������ģʽ
	nRet = fcntl(nFd, F_SETFL, nNewOption);					// fd����Ϊ������ģʽ 
	if (nRet < 0){
		printf("failed to call fcntl!\n");
		// �������ʧ�ܣ���ӡ����ԭ�򲢷���
		printf("errno value: %d, it means: %s",errno, strerror(errno));
		return -1;
	}
	return nOldOption;
}





