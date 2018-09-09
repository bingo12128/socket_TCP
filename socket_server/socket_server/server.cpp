#include <stdio.h>  
#include <winsock2.h>  //winsock头文件，包含各种socket函数

#pragma comment(lib,"ws2_32.lib") //将一个库文件链接到目标文件中，将ws2_32.lib库文件加入到本工程中  
                                  //ws2_32.lib对应ws2_32.dll，提供对bind、accept等相关API的支持
int main(int argc, char* argv[]) //定义参数个数，与参数数组
{
	//初始化WSA  
	WORD sockVersion = MAKEWORD(2, 2); //WORD（字）是微软SDK中的类型，是2byte的无符号整数（0~65535）
	                                   //MAKEWORD(2,2)表示调用Winsock2.2版本
	WSADATA wsaData; //这个结构被用来存储 被WSAStartup函数调用后返回的 Windows Sockets数据。它包含Winsock.dll执行的数据。
	if (WSAStartup(sockVersion, &wsaData) != 0) //WSA的启动命令
	{
		return 0;
	}

	//创建套接字  
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//绑定IP和端口  
	sockaddr_in sin; //对IP网络地址信息存储
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8080); //htons从本机字节顺序转换为网络字节顺序
	sin.sin_addr.S_un.S_addr = INADDR_ANY; //填入本地地址，INADDR_ANY指定地址为0.0.0.0;如果指定ip地址为通配地址(INADDR_ANY)，那么内核将等到套接字已连接(TCP)或已在套接字上发出数据报时才选择一个本地IP地址。
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//开始监听  
	if (listen(slisten, 5) == SOCKET_ERROR) //设置最大连接个数为5，默认为20
	{
		printf("listen error !");
		return 0;
	}

	//循环接收数据  
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	while (true)
	{
		printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr)); //将一个十进制网络字节序转换为点分十进制IP格式的字符串。

		//接收数据  
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			printf(revData);
		}

		//发送数据  
		const char * sendData = "你好，TCP客户端！\n";
		send(sClient, sendData, strlen(sendData), 0); //flags设置为0，与write()类似
		closesocket(sClient);  
	}

	closesocket(slisten); //关闭套接字
	WSACleanup();  //关闭加载的套接字库
	return 0;
}
