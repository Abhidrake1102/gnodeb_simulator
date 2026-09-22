
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>

using namespace std;

int main () {
	
	int server_gnodeb, client_gnodeb;
	server_gnodeb = socket(AF_INET, SOCK_STREAM, 0);

	if (server_gnodeb < 0){
		cerr << "Failed to create socket\n";
		return 1;
	}

	sockaddr_in server_gnodeb_addr, client_gnodeb_addr;
	server_gnodeb_addr.sin_family = AF_INET;
	server_gnodeb_addr.sin_port = htons(5050);

	int result = inet_pton(AF_INET, "127.0.0.1", &server_gnodeb_addr.sin_addr);

	if(result == 1) {
		cout << "Address given to server success\n";
	}
	else if (result == 0){
		cout << "incorredt address given\n";
	}
	else{
		cerr << "Error occurred\n";
	}

	if(bind(server_gnodeb, (sockaddr*)&server_gnodeb_addr, sizeof(server_gnodeb_addr)) < 0) {
		cerr <<  "failed to bind socket\n";
		return 1;
	}

	socklen_t client_gnodeb_len = sizeof(client_gnodeb_addr);

	listen(server_gnodeb, 5);

	cout << "Listening to port 5050\n";

	client_gnodeb = accept(server_gnodeb, (sockaddr*)&client_gnodeb_addr, &client_gnodeb_len);

	if(client_gnodeb < 0) {
		cerr << "Accept failed\n";
		return 1;
	}

	char buffer[1024] = {0};

	recv(client_gnodeb, buffer, sizeof(buffer), 0);

	const char* reply = "received response from UE";

	send(client_gnodeb, reply, sizeof(reply), 0);

	close(client_gnodeb);
	close(server_gnodeb);

	return 0;
}
