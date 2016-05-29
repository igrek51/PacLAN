#include "network_task.h"
#include "network.h"

NetworkTask::NetworkTask(int task){
    this->task = task;
    param_i = 0;
    param_i2 = 0;
    param_s = "";
    param_c = NULL;
}

NetworkTask::~NetworkTask(){
    if(param_c!=NULL){
        delete[] param_c;
    }
}

void Network::task_exec(NetworkTask *task){
    if(task->task == TASK_OPEN_SERVER){
        open_server_socket();
    }else if(task->task == TASK_CLOSE_SERVER){
        disconnect_socket(0);
    }else if(task->task == TASK_CONNECT){
        connect_socket(task->param_s);
    }else if(task->task == TASK_CLOSE){
        disconnect_socket(0);
    }else if(task->task == TASK_SEND_TO_SERVER){
        if(!client){
            network_error("Niepodłączony do serwera");
            return;
        }
        send_packet(0, task->param_c, task->param_i);
    }else if(task->task == TASK_SEND_TO_CLIENT){
        if(!server){
            network_error("Brak utworzonego serwera");
            return;
        }
        int receiver_i = task->param_i;
        if(receiver_i<1 || receiver_i>=(int)sockets.size()){
            network_error("Numer odbiorcy spoza listy klientów");
            return;
        }
        send_packet(receiver_i, task->param_c, task->param_i2);
    }else{
        network_error("Nieprawidłowy numer zadania");
    }
}

void Network::addtask_open_server(){
    NetworkTask *task = new NetworkTask( TASK_OPEN_SERVER );
    tasks.push_back(task);
}

void Network::addtask_close_server(){
    NetworkTask *task = new NetworkTask( TASK_CLOSE_SERVER );
    tasks.push_back(task);
}

void Network::addtask_connect_client(string ip){
    NetworkTask *task = new NetworkTask( TASK_CONNECT );
    task->param_s = ip;
    tasks.push_back(task);
}

void Network::addtask_close_client(){
    NetworkTask *task = new NetworkTask( TASK_CLOSE );
    tasks.push_back(task);
}

void Network::addtask_send_to_server(char *msg, int len){
    NetworkTask *task = new NetworkTask( TASK_SEND_TO_SERVER );
    task->param_c = msg;
    task->param_i = len;
    tasks.push_back(task);
}

void Network::addtask_send_to_client(int client_nr, char *msg, int len){
    NetworkTask *task = new NetworkTask( TASK_SEND_TO_CLIENT );
    task->param_i = client_nr;
    task->param_c = msg;
    task->param_i2 = len;
    tasks.push_back(task);
}
