// StreamingServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/core/types_c.h>

#include <stdio.h>
#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32")

int main()
{
	// Khoi tao thu vien Winsock
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	// Khai bao dia chi server
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(9000);

	// Tao socket cho ket noi
	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Gan dia chi voi socket
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));

	// Chuyen sang trang thai cho ket noi
	listen(listener, 5);

	// Chap nhan ket noi va truyen nhan du lieu
	while (1)
	{
		std::cout << "Waiting for new client\n";
		SOCKET client = accept(listener, NULL, NULL);
		std::cout << "New client accepted: " << client << std::endl;

		// streaming
		cv::Mat img, frame;
		img = cv::Mat::zeros(720, 1280, CV_8UC1);
		uchar* iptr = img.data;
		if (!img.isContinuous())
		{
			img = img.clone();
		}
		int imgSize = img.total() * img.elemSize();
		int dataRecv = 0;
		int key;
		std::vector<uchar> recvBuf(307200);
		while (1)
		{
			dataRecv = recv(client, (char*)recvBuf.data(), recvBuf.size(), 0);
			if (dataRecv == -1)
			{
				std::cout << "ERROR RECEIVE" << std::endl;
				//cv::destroyWindow("Receiver");
				break;
			};

			frame = cv::imdecode(recvBuf, 1);
			if (frame.empty())
			{
				//std::cout << "";
				//cv::destroyWindow("Receiver");
				break;
			}
			else {
				cv::imshow("Receiver", frame);
				key = cv::waitKey(30);
			}
		}
		closesocket(client);
		closesocket(listener);
		WSACleanup();
		break;
	}
}