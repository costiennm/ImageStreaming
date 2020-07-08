// StreamingClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/core/types_c.h>
#include "opencv2/opencv.hpp"

#include <iostream>
#include <stdio.h>
#include <winsock2.h>

#define CV_IMWRITE_JPEG_QUALITY 1
#define CV_IMWRITE_PNG_COMPRESSION 16

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32")

using namespace std;

int key;
int weight = 640;
int height = 480;
int quality = 100;
int sendTime = 0;

SOCKET client;

void stream();

int main()
{
	// Khoi tao thu vien Winsock
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	// Tao socket ket noi
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Gan dia chi server
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(9000);

	system("pause");

	// Ket noi
	int ret = connect(client, (SOCKADDR*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		cout << "ERROR CONNECT: error code" << ret << endl;
		return 1;
	}

	// MENU
	int chon = 0, chon1 = 0, chon2 = 0;
	do
	{
		system("cls");
		cout << "\n\n";
		cout << "\t\t\t ======================MENU======================\n";
		cout << "\t\t\t |1. Kich thuoc                                 |\n";
		cout << "\t\t\t |2. Chat luong nen                             |\n";
		cout << "\t\t\t |3. Send-time                                  |\n";
		cout << "\t\t\t |4. Thoat                                      |\n";
		cout << "\t\t\t ================================================\n";
		cout << "Ban chon: ";
		cin >> chon;
		switch (chon)
		{
		case 1:
			do
			{
				system("cls");
				cout << "\n\n";
				cout << "\t\t\t =====================SIZE======================\n";
				cout << "\t\t\t |1. 480 x 640                                 |\n";
				cout << "\t\t\t |2. 360 x 480                                 |\n";
				cout << "\t\t\t |3. Quay lai                                  |\n";
				cout << "\t\t\t ===============================================\n";
				cout << "Ban chon: ";
				cin >> chon1;
				switch (chon1)
				{
				case 1:
					weight = 640;
					height = 480;
					stream();
					break;
				case 2:
					weight = 480;
					height = 360;
					stream();
					break;
				case 3:
					// Back
					break;
				default:
					cout << "Ban chon sai. Moi ban chon lai!\n";
					Sleep(2000);
					break;
				}
			} while (chon1 != 3);
			break;
		case 2:
			do
			{
				system("cls");
				cout << "\n\n";
				cout << "\t\t\t ================COMPRESS-QUALITY=================\n";
				cout << "\t\t\t |1. Low                                         |\n";
				cout << "\t\t\t |2. Normal                                      |\n";
				cout << "\t\t\t |3. High                                        |\n";
				cout << "\t\t\t |4. Quay lai                                    |\n";
				cout << "\t\t\t =================================================\n";
				cout << "Ban chon: ";
				cin >> chon2;
				switch (chon2)
				{
				case 1:
					// Low
					quality = 10;
					stream();
					break;
				case 2:
					// Normal
					quality = 50;
					stream();
					break;
				case 3:
					// High
					quality = 100;
					stream();
					break;
				case 4:
					// Back
					break;
				default:
					cout << "Ban chon sai. Moi ban chon lai!\n";
					Sleep(2000);
					break;
				}
			} while (chon2 != 4);
			break;
		case 3:
			cout << "Nhap vao Send-time (ms): ";
			cin >> sendTime;
			stream();
			break;
		case 4:
			cout << "GoodBye\n";
			Sleep(2000);
			break;
		default:
			cout << "Ban chon sai. Moi ban chon lai MENU!\n";
			Sleep(2000);
			break;
		}
	} while (chon != 4);

	closesocket(client);
	WSACleanup();
}

void stream()
{
	// Capture Webcam
	cv::VideoCapture cap(0);
	cv::Mat img, imgSend, frame;
	img = cv::Mat::zeros(720, 1280, CV_8UC1);
	if (!img.isContinuous())
	{
		img = img.clone();
	}
	int imgSize = img.total() * img.elemSize();
	int dataSend = 0;
	while (1)
	{
		cap >> img;
		resize(img, imgSend, cv::Size(weight, height), 0, 0, cv::INTER_CUBIC);
		std::vector<int> vec;
		vec.push_back(CV_IMWRITE_JPEG_QUALITY);
		vec.push_back(quality);
		cv::imwrite("C:\\TiennmS\\LTM\\BTL_ImageStreaming\\streaming\\stream.jpg", imgSend, vec);
		frame = cv::imread("C:\\TiennmS\\LTM\\BTL_ImageStreaming\\streaming\\stream.jpg", 1);

		vector<uchar> sendBuf(307200);
		imencode(".jpg", frame, sendBuf);

		// Set sendTime
		Sleep(sendTime);
		dataSend = send(client, (char*)sendBuf.data(), sendBuf.size(), 0);
		if (dataSend < 0)
		{
			cout << "ERROR SEND" << endl;
			break;
		}
		if (dataSend >= 0) {
			cv::imshow("Sender", frame);
		}
		if (key = cv::waitKey(30) >= 0)
		{
			string a;
			cv::destroyWindow("Sender");
			break;
		}
	}
}
