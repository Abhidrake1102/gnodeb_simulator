
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <thread>
#include "message.h"

// adding a thread amf Function to let the AMF and gNodeB connection to be a seperate entity 
// Idea is to run the program as long as UE connects and runs.

void amf_work() {
	int gnodeb_client = socket(AF_INET, SOCK_STREAM, IPPROT_SCTP);
	
	if(gnodeb_client < 0){
		cerr << "Failed to create amf socket\n";
		return 1;
	}
	
	sockaddr_in amf_server_addr;
	memset(&amf_server_addr, 0, sizeof(amf_server_addr));
	amf_server_addr.sin_family = AF_INET;
	amf_server_addr.sin_port = htons(38412);
	amf_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if( connect(gnodeb_client, (sockaddr*)&amf_server_addr, sizeof(amf_server_addr)) < 0){
		cout <<  "Failed to connect to server AMF\n";
		close(gnodeb_client);
		return 1;
	}
	
	cout << "amf server connected\n";
	
	
	while (true) {
		string ng_setup_req = "NG_SETUP_REQUEST|1001|GNB-NOIDA";
		
		send(gnodeb_client, ng_setup_req.c_str(), ng_setup_req.size(), 0);
		
		char receivedbuffer[1024];
		
		int recvLen = recv(gnodeb_client, receivedbuffer, sizeof(receivedbuffer), 0);
		
		if(recvLen > 0){
			string receviedMessage(receivedbuffer, recvLen);
			cout << "received message from AMF\n";
			cout << "temporarily closing the connection to end the thread\n";
			break;
		}
	}
	close(gnodeb_client);
}

using namespace std;

int main () {
	
	thread amf_thread(amf_work);
	int gnodeb_server, client_ue;
	gnodeb_server = socket(AF_INET, SOCK_STREAM, 0);

	if (gnodeb_server < 0){
		cerr << "Failed to create socket\n";
		return 1;
	}

	sockaddr_in gnodeb_sever_addr, client_ue_addr;
	memset(&gnodeb_sever_addr, 0, sizeof(gnodeb_sever_addr));
	gnodeb_sever_addr.sin_family = AF_INET;
	gnodeb_sever_addr.sin_port = htons(5050);
	
	// can give any address as well without inet_pton implementation
	int result = inet_pton(AF_INET, "127.0.0.1", &gnodeb_sever_addr.sin_addr); 

	if(result == 1) {
		cout << "Address given to server success\n";
	}
	else if (result == 0){
		cout << "incorredt address given\n";
	}
	else{
		cerr << "Error occurred\n";
	}

	if(bind(gnodeb_server, (sockaddr*)&gnodeb_sever_addr, sizeof(gnodeb_sever_addr)) < 0) {
		cerr <<  "failed to bind socket\n";
		return 1;
	}

	socklen_t client_ue_len = sizeof(client_ue_addr);

	listen(gnodeb_server, 5); // can we make use of return value 0,-1??

	cout << "Listening to port 5050\n";
	while (true) {
		client_ue = accept(gnodeb_server, (sockaddr*)&client_ue_addr, &client_ue_len);
	
		if(client_ue < 0) {
			cerr << "Accept failed\n";
			continue;
		}

		while(true) {
			char buffer[1024] = {0};

			ssize_t bytes = recv(client_ue, buffer, sizeof(buffer), 0);
			
			if(bytes > 0) {
				cout << "recevied data from ue\n";				
			}else if (bytes == 0){
				cout << "ue is disconnected\n";
				break;
			}else {
				cerr << "error occured while receiving data\n";
			}
			
			// decode the message and set a flag
			// carry flag to send response

			const char* reply = "received response from UE";

			send(client_ue, reply, sizeof(reply), 0);

			close(client_ue);
		}
	}
	close(gnodeb_server);
	
	amf_thread.join();

	return 0;
}
