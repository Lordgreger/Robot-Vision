#pragma once
/* 
UR Simple Connect by Mathias Gregersen
This header contains functionalities that simplifies the workflow when working with UR robots through TCP and URScript.
*/

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib") //Winsock Library

namespace URSC {
	class URRobot {
		private:
			bool hasSocket = false;
			bool isConnected = false;
			WSADATA wsa;
			SOCKET s;

			std::string qstr(float q1, float q2, float q3, float q4, float q5, float q6) {
				std::string output = "[";
				output += std::to_string(q1) + ",";
				output += std::to_string(q2) + ",";
				output += std::to_string(q3) + ",";
				output += std::to_string(q4) + ",";
				output += std::to_string(q5) + ",";
				output += std::to_string(q6) + "]";
				return output;
			}

			std::string pstr(float q1, float q2, float q3, float q4, float q5, float q6) {
				std::string output = "p" + qstr(q1, q2, q3, q4, q5, q6);
				return output;
			}

			std::string movevar(float a, float v, float t, float r) {
				std::string output = "";
				output += "a=" + std::to_string(a) + ",";
				output += "v=" + std::to_string(v) + ",";
				output += "t=" + std::to_string(t) + ",";
				output += "r=" + std::to_string(r);
				return output;
			}

			std::string movex(std::string type, std::string q, std::string var) {
				return type + "(" + q + "," + var + ")";
			}

			std::string getx(std::string cmd) {
				if (!Send(cmd + "\n"))
					return "err";
				return Receive();
			}

		public:
			bool Connect(PCSTR ip, u_short port) {
				if (!hasSocket) {
					if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
						std::cout << "WSAStartup failed: " << WSAGetLastError() << std::endl;
						return false;
					}

					s = socket(AF_INET, SOCK_STREAM, 0);
					if (s == INVALID_SOCKET) {
						std::cout << "Failed creating socket: " << WSAGetLastError() << std::endl;
						return false;
					}

					hasSocket = true;
				}

				struct sockaddr_in server;
				inet_pton(AF_INET, ip, &server.sin_addr.s_addr);
				server.sin_family = AF_INET;
				server.sin_port = htons(port);

				if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
					std::cout << "Connection failed: " << WSAGetLastError() << std::endl;
					return false;
				}

				isConnected = true;
				return true;
			}

			bool Send(std::string msg) {
				if (!isConnected) {
					std::cout << "Failed sending msg: No connection established, try using Connect() first?" << std::endl;
					return false;
				}

				if (send(s, msg.c_str(), strlen(msg.c_str()), 0) < 0) {
					std::cout << "Failed sending msg: " << WSAGetLastError() << std::endl;
					return false;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				return true;
			}

			std::string Receive() {
				char server_reply[16000];
				int recv_size;
				if ((recv_size = recv(s, server_reply, 16000, 0)) == SOCKET_ERROR) {
					std::cout << "Failed receiving: " << WSAGetLastError() << std::endl;
					return "err";
				}
				std::cout << recv_size << std::endl;
				
				server_reply[recv_size] = '\0';
				for (size_t i = 0; i < recv_size; i++) {
					std::cout << server_reply[i];
				}
				std::cout << std::endl;
				for (size_t i = 0; i < recv_size; i++) {
					std::cout << std::hex << static_cast<int>(server_reply[i]) << " ";
				}
				return std::string(server_reply);
			}

			bool movej(float q1, float q2, float q3, float q4, float q5, float q6, bool pose = true, float a = 1.4f, float v = 1.05f, float t = 0.f, float r = 0.f) {
				std::string q;
				if (pose) { 
					q = pstr(q1, q2, q3, q4, q5, q6); 
				}
				else { 
					q =  qstr(q1, q2, q3, q4, q5, q6); 
				}
				std::cout << pose << " " << q << std::endl;
				std::string msg = movex("movej", q, movevar(a, v, t, r));
				return Send(msg + "\n");
			}

			bool movel(float q1, float q2, float q3, float q4, float q5, float q6, float a = 1.4f, float v = 1.05f, float t = 0.f, float r = 0.f) {
				std::string msg = movex("movel", pstr(q1, q2, q3, q4, q5, q6), movevar(a, v, t, r));
				return Send(msg + "\n");
			}

			std::string force() {
				return getx("force()");
			}

			std::string get_actual_joint_positions() {
				return getx("get_actual_joint_positions()");
			}
	};
}