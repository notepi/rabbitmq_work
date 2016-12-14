#include <stdio.h>
#include <libgen.h>
#include <poll.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "epoll.h"
#include "IPv4socket.h"
#include "signal.h"
#include <iostream>

using namespace std;

struct PROCESS_SIGNAL_DATE g_sMainProcessSigDate;
#define BUFFER_SIZE 1024

/*	�������ܣ���fd��ӵ�epoll����
	������1��������epoll  2����������fd
		  3���Ƿ����ó�etģʽ��1-�� 0-��
	���أ���			
*/
void AddFdToEpoll(int nEpollFd, int nListenFd, int nEnable)
{
	struct epoll_event sEv;
	sEv.events = EPOLLIN;
	sEv.data.fd= nListenFd;
	if(nEnable){
		sEv.events |= EPOLLET;
	}
	epoll_ctl(nEpollFd, EPOLL_CTL_ADD, nListenFd, &sEv);
	SetNonblocking(nListenFd);
}

/*	�������ܣ���fd��epoll������ɾ��
	������1��������epoll  2����������fd
	���أ�socket��fd			
*/
void DeleteFdFromEpoll (int nEpollFd, int nListenFd)
{
	struct epoll_event sEv;// ���ã�ֻ����������
	epoll_ctl(nEpollFd, EPOLL_CTL_DEL, nListenFd, &sEv);
}



void lt( epoll_event* pfsEvents, int nNumber, int nEpollFd, int nListenFd )
{
	char gcBuff[ BUFFER_SIZE ];
	struct epoll_event sEv;
	int nRet;
	for(int i=0; i < nNumber ; i++){//��ѯ�������¼�
		int nSockFd = pfsEvents[i].data.fd;
		if(nSockFd == g_sMainProcessSigDate.m_nRdFd && (pfsEvents[i].events & EPOLLIN )){//�źŴ���
			if(!MainProcessSignalProcessing(nSockFd)){
				continue;
			}  
		}
		else if(nListenFd == nSockFd ){//���µ�����
			struct sockaddr_in sClientAddress;
		    socklen_t sklCleintAddrLen = sizeof(struct sockaddr);
    		int nConnFd = accept(nListenFd, (struct sockaddr*)&sClientAddress, &sklCleintAddrLen);
    		if(nConnFd < 0){//��ȡʧ��
    			printf("failed to accept!\n");
    			continue;
    		}
    		
			AddFdToEpoll(nEpollFd, nConnFd, 0);//����¼���ע
			cout << "comes a new user" << endl;
		}
		else if (pfsEvents[i].events & EPOLLRDHUP)//�����ӶϿ�
		{
			cout << "a client left\n" << endl;
			DeleteFdFromEpoll(nEpollFd, nSockFd);
			close(nSockFd);
		}
		else if(pfsEvents[i].events & EPOLLIN){//������
            memset(gcBuff, '\0', sizeof(gcBuff));
            nRet = recv(nSockFd, gcBuff, BUFFER_SIZE-1, 0 );            
            if(nRet <= 0 ){//����
                if((errno != EAGAIN) && (errno != EWOULDBLOCK) ){//����������Ϊ����δ����
                	DeleteFdFromEpoll(nEpollFd, nSockFd);
                	printf("a client lesft!\n");
                    close(nSockFd);
                    continue;
                }
            }
			printf( "get %d bytes of client data :%s from: %d\n", nRet, gcBuff, nSockFd );
        }
	}
}


void et(epoll_event* pfsEvents, int nNumber, int nEpollFd, int nListenFd)
{
	char gcBuff[BUFFER_SIZE];
	struct epoll_event sEv;
	int nRet;
	for(int i=0; i < nNumber ; i++){//��ѯ�������¼�
		int nSockFd = pfsEvents[i].data.fd;
		if(nListenFd == pfsEvents[i].data.fd ){//���µ�����
			struct sockaddr_in sClientAddress;
		    socklen_t sklCleintAddrLen = sizeof(struct sockaddr);
    		int nConnFd = accept(nListenFd, ( struct sockaddr* )&sClientAddress, &sklCleintAddrLen);
    		if(nConnFd < 0){//��ȡʧ��
    			printf("failed to accept!\n");
    			continue;
    		}
    		
			AddFdToEpoll(nEpollFd, nConnFd, 1);//����¼���ע
			cout << "comes a new user\n" << endl;
		}
		else if(pfsEvents[i].events & POLLRDHUP)//�����ӶϿ�
		{
			cout << "a client left\n" << endl;
			DeleteFdFromEpoll(nEpollFd, nSockFd);
			close(nSockFd);
		}
		else if(pfsEvents[i].events & EPOLLIN){//������
    		while(1)
    		{
	            memset(gcBuff, '\0', sizeof(gcBuff));
	            nRet = recv(nSockFd, gcBuff, BUFFER_SIZE-1, 0 );            
	            if(nRet < 0 ){//����
	                if( (errno == EAGAIN) || (errno == EWOULDBLOCK) ){//����δ����
	                    continue;
	                }
	                //���ӳ�������
					DeleteFdFromEpoll(nEpollFd, nSockFd);//ɾ����ע
                    close(nSockFd );
                    break;	                
	            }
				printf("get %d bytes of client data :%s! from: %d\n", nRet, gcBuff, nSockFd);
			}
        }
	}
}