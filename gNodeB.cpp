
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>

using namespace std;

int main () {
	
	int gnodeb_server_to_ue, client_gnodeb_of_ue;
	gnodeb_server_to_ue = socket(AF_INET, SOCK_STREAM, 0);

	if (gnodeb_server_to_ue < 0){
		cerr << "Failed to create socket\n";
		return 1;
	}

	sockaddr_in ue_server_address, ue_client_addr;
	ue_server_address.sin_family = AF_INET;
	ue_server_address.sin_port = htons(5050);

	int result = inet_pton(AF_INET, "127.0.0.1", &ue_server_address.sin_addr);

	if(result == 1) {
		cout << "Address given to server success\n";
	}
	else if (result == 0){
		cout << "incorredt address given\n";
	}
	else{
		cerr << "Error occurred\n";
	}

	if(bind(gnodeb_server_to_ue, (sockaddr*)&ue_server_address, sizeof(ue_server_address)) < 0) {
		cerr <<  "failed to bind socket\n";
		return 1;
	}

	socklen_t client_gnodeb_of_ue_len = sizeof(ue_client_addr);

	listen(gnodeb_server_to_ue, 5);

	cout << "Listening to port 5050\n";

	client_gnodeb_of_ue = accept(gnodeb_server_to_ue, (sockaddr*)&ue_client_addr, &client_gnodeb_of_ue_len);

	if(client_gnodeb_of_ue < 0) {
		cerr << "Accept failed\n";
		return 1;
	}

	char buffer[1024] = {0};

	recv(client_gnodeb_of_ue, buffer, sizeof(buffer), 0);

	const char* reply = "received response from UE";

	send(client_gnodeb_of_ue, reply, sizeof(reply), 0);

	close(client_gnodeb_of_ue);
	close(gnodeb_server_to_ue);

	return 0;
}
