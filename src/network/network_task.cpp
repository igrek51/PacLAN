#include "network_task.h"
#include "network.h"

NetworkTask::NetworkTask(int task) {
    this->task = task;
    param_i = 0;
    param_i2 = 0;
    param_s = "";
    param_c = nullptr;
}

NetworkTask::~NetworkTask() {
    if (param_c != nullptr) {
        delete[] param_c;
    }
}

void Network::task_exec(NetworkTask* task) {
    if (task->task == TASK_OPEN_SERVER) {
        openServerSocket();
    } else if (task->task == TASK_CLOSE_SERVER) {
        disconnect(0);
    } else if (task->task == TASK_CONNECT) {
        connectTo(task->param_s);
    } else if (task->task == TASK_CLOSE) {
        disconnect(0);
    } else if (task->task == TASK_SEND_TO_SERVER) {
        if (!client) {
            error("Niepodłączony do serwera");
            return;
        }
        sendData(0, task->param_c, task->param_i);
    } else if (task->task == TASK_SEND_TO_CLIENT) {
        if (!server) {
            error("Brak utworzonego serwera");
            return;
        }
        int receiver_i = task->param_i;
        if (receiver_i < 1 || receiver_i >= (int) connections.size()) {
            error("Numer odbiorcy spoza listy klientów");
            return;
        }
        sendData(receiver_i, task->param_c, task->param_i2);
    } else {
        error("Nieprawidłowy numer zadania");
    }
}

void Network::addtask_open_server() {
    NetworkTask* task = new NetworkTask(TASK_OPEN_SERVER);
    tasks.push_back(task);
}

void Network::addtask_close_server() {
    NetworkTask* task = new NetworkTask(TASK_CLOSE_SERVER);
    tasks.push_back(task);
}

void Network::addtask_connect_client(string ip) {
    NetworkTask* task = new NetworkTask(TASK_CONNECT);
    task->param_s = ip;
    tasks.push_back(task);
}

void Network::addtask_close_client() {
    NetworkTask* task = new NetworkTask(TASK_CLOSE);
    tasks.push_back(task);
}

void Network::addtask_send_to_server(char* msg, int len) {
    NetworkTask* task = new NetworkTask(TASK_SEND_TO_SERVER);
    task->param_c = msg;
    task->param_i = len;
    tasks.push_back(task);
}

void Network::addtask_send_to_client(int client_nr, char* msg, int len) {
    NetworkTask* task = new NetworkTask(TASK_SEND_TO_CLIENT);
    task->param_i = client_nr;
    task->param_c = msg;
    task->param_i2 = len;
    tasks.push_back(task);
}
