#include "network_connection.h"

NetworkConnection::NetworkConnection(int socket){
    this->socket = socket;
}

NetworkConnection::~NetworkConnection(){
    //wyczyszczenie bufora odbioru danych
    recv_packet.clear();
    recv_buffer.clear();
}

void NetworkConnection::addReceivedPacket(string packet){
    recv_packet.push_back(packet);
};


bool NetworkConnection::isReady(){
    return !recv_packet.empty();
}

string NetworkConnection::popPacket(){
    if(recv_packet.empty()){
        return nullptr;
    }
    string first = recv_packet.at(0);
    recv_packet.pop_front();
    return first;
}

string NetworkConnection::bufferPop(){
    string packet = "";
    for (char recvc : recv_buffer) {
        packet += recvc;
    }
    recv_buffer.clear();
    return packet;
}

void NetworkConnection::bufferAdd(char c){
    recv_buffer.push_back(c);
}

unsigned int NetworkConnection::bufferSize(){
    return (unsigned int) recv_buffer.size();
}