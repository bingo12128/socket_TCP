#include <stdio.h>  
#include <winsock2.h>  //winsockͷ�ļ�����������socket����

#pragma comment(lib,"ws2_32.lib") //��һ�����ļ����ӵ�Ŀ���ļ��У���ws2_32.lib���ļ����뵽��������  
                                  //ws2_32.lib��Ӧws2_32.dll���ṩ��bind��accept�����API��֧��
int main(int argc, char* argv[]) //����������������������
{
	//��ʼ��WSA  
	WORD sockVersion = MAKEWORD(2, 2); //WORD���֣���΢��SDK�е����ͣ���2byte���޷���������0~65535��
	                                   //MAKEWORD(2,2)��ʾ����Winsock2.2�汾
	WSADATA wsaData; //����ṹ�������洢 ��WSAStartup�������ú󷵻ص� Windows Sockets���ݡ�������Winsock.dllִ�е����ݡ�
	if (WSAStartup(sockVersion, &wsaData) != 0) //WSA����������
	{
		return 0;
	}

	//�����׽���  
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//��IP�Ͷ˿�  
	sockaddr_in sin; //��IP�����ַ��Ϣ�洢
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8080); //htons�ӱ����ֽ�˳��ת��Ϊ�����ֽ�˳��
	sin.sin_addr.S_un.S_addr = INADDR_ANY; //���뱾�ص�ַ��INADDR_ANYָ����ַΪ0.0.0.0;���ָ��ip��ַΪͨ���ַ(INADDR_ANY)����ô�ں˽��ȵ��׽���������(TCP)�������׽����Ϸ������ݱ�ʱ��ѡ��һ������IP��ַ��
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//��ʼ����  
	if (listen(slisten, 5) == SOCKET_ERROR) //����������Ӹ���Ϊ5��Ĭ��Ϊ20
	{
		printf("listen error !");
		return 0;
	}

	//ѭ����������  
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	while (true)
	{
		printf("�ȴ�����...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr)); //��һ��ʮ���������ֽ���ת��Ϊ���ʮ����IP��ʽ���ַ�����

		//��������  
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			printf(revData);
		}

		//��������  
		const char * sendData = "��ã�TCP�ͻ��ˣ�\n";
		send(sClient, sendData, strlen(sendData), 0); //flags����Ϊ0����write()����
		closesocket(sClient);  
	}

	closesocket(slisten); //�ر��׽���
	WSACleanup();  //�رռ��ص��׽��ֿ�
	return 0;
}
