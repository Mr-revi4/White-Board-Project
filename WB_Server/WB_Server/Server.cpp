#include "Server.h"
#include "Board.h"
#pragma comment(lib, "sfml-network.lib")

class Board;
Server::Server(){
	getUsersInfo(info_of_usres);
	
	all_boards.resize(100);
	
}
Server::Server(unsigned int p) {
	port = p;
	getUsersInfo(info_of_usres);
}
void Server::initListen() { // �������������� ��������� �����
	if (listener.listen(port) == sf::Socket::Done) {
		cout << "Start server" << endl;
		running = true;
	}
	else {
		////// ���������� �� �����������
		int ex;
		cout << "Some error with init server \n Press some key" << endl;
		cin >> ex;
		return;
		////// ���������� �� �����������
	}
	selector.add(listener);

}
bool Server::authorization(shared_ptr <sf::TcpSocket> & client_socket, shared_ptr <Client> & client) {// ����������� �� �������. True ���� �������, false �����. 
	
	char name_size[1];
	char pass_size[1];
	std::size_t received = 0;
	char * name_c = nullptr;
	char * pas = nullptr;
	int iname_size = 0;
	int ipass_size = 0;

	
	
	if (client_socket->receive(name_size, sizeof(name_size), received) == sf::Socket::Done) {
		iname_size = atoi(name_size) + 1;
		name_c  = new char[iname_size];
		client_socket->receive(name_c, iname_size, received);
	}
	
	if (client_socket->receive(pass_size, sizeof(pass_size), received) == sf::Socket::Done) {
		ipass_size = atoi(pass_size) + 1;
		pas = new char[ipass_size];
		client_socket->receive(pas, ipass_size, received);
	}
	


	/*cout << "iname_size: " << iname_size << endl;
	cout << "Name: " << name_c << endl;
	cout << "ipass_size: " << ipass_size << endl;
	cout << "Pass: " << pas << endl;*/

	for (auto it = info_of_usres.begin(); it != info_of_usres.end(); ++it) // ����� ����� � ��
		if (it->first == name_c)
			if (it->second == pas) {
				client->setStatus(USER_ONLINE);
				client->setName(name_c);
				cout << "Welcom " << it->first << endl;
				return true;
			}

	delete[] name_c;
	delete[] pas;
	
	return false;
}

void Server::startListening() {
	char answer[1];
	initListen();
	while (running) {
		
		if (selector.wait(sf::Time(sf::seconds(0.3f)))){ // �������� ������������ ������
			
			if (selector.isReady(listener)) {// ���� ��������� ����� ����� ���������
				
				shared_ptr <sf::TcpSocket> client_socket(new sf::TcpSocket); // ���������� �����
				shared_ptr <Client> client(new Client); // ���������� � ������� � ��� ���������.
			
				
				if (listener.accept(*client_socket) == sf::Socket::Done) { // ���� ������ ������� ������������� � ������
					cout << "some one try to connecnt" << endl;

					if (authorization(client_socket, client)) {
						users.push_back(make_pair(client_socket, client));
						selector.add(*client_socket);
						
						answer[0] = server_ok_code ;
						client_socket->send(answer, 1);
					}
					else {
						
						answer[0] = wrong_pass_code;
						client_socket->send(answer, 1);
					}
					
				}
			}
			else {
				for (auto it = users.begin(); it != users.end(); ++it) {
					sf::TcpSocket & client = *it->first;
					if (selector.isReady(client) && it->second->getStatus() == USER_ONLINE) {
						char query_code[1];
						size_t rec;
						if (client.receive(query_code, 1, rec) == sf::Socket::Done){

							cout << "New query, code: " << (int)query_code[0] << endl;

							if ((int)query_code[0] == new_board_code && BOARD_CNT < 100) {
								answer[0] = server_ok_code;
								client.send(answer, 1);

								all_boards[BOARD_CNT] = new Board(it->second, it->first, &listener);
								all_boards[BOARD_CNT]->setBoard_ID(BOARD_CNT);
								BOARD_CNT++;

								selector.remove(client);
							}
							if ((int)query_code[0] == connect_board_code) {//����������� � �����
								char name_size[1];
								size_t received;
								if (client.receive(name_size, sizeof(name_size), received) == sf::Socket::Done) {// ������ ��� ���������� �����
									int iname_size = atoi(name_size) + 1;
									char * name_c = new char[iname_size];
									client.receive(name_c, iname_size, received);
									for (int i = 0; i < all_boards.size(); ++i) {//���� �������� ����� ���, ������������ � �����
										if (all_boards[i]->getCreaterName() == name_c) { 
											all_boards[i]->addUser(it->first);
										}
									}
								}
							}
						}

					}
				}
			}
		}
		
	}
	
}
Server::~Server()
{
}
