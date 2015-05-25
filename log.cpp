#include <fstream>
#include <ctime>
#include <sstream>
#include <windows.h>
#include "log.h"
#include "config.h"

string get_time(){
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    stringstream ss2;
    if(timeinfo->tm_hour<10) ss2<<"0";
    ss2<<timeinfo->tm_hour<<":";
    if(timeinfo->tm_min<10) ss2<<"0";
    ss2<<timeinfo->tm_min<<":";
    if(timeinfo->tm_sec<10) ss2<<"0";
    ss2<<timeinfo->tm_sec;
    return ss2.str();
}

void log_clear(){
	fstream plik;
    plik.open(Config::log_filename.c_str(),fstream::out|fstream::trunc);
	plik.close();
}

void log(string l){
	fstream plik;
    plik.open(Config::log_filename.c_str(),fstream::out|fstream::app);
    if(!plik.good()){
				plik.close();
        return;
    }
    plik<<get_time()<<" - "<<l<<endl;
	plik.close();
}

void log(int l){
    stringstream ss;
    ss<<l;
    log(ss.str());
}

void error(string e){
    log(e);
    MessageBox(NULL,e.c_str(),"Błąd",MB_OK|MB_ICONERROR);
}


void ss_clear(stringstream &sstream){
    sstream.str("");
    sstream.clear();
}

int round_to_int(double d){
    int c = (int)d;
    if(d-c >= 0.5)
        return c+1;
    return c;
}

bool has_extension(string filename, string ext){
    if(ext.length()>filename.length())
        return false;
    filename = filename.substr(filename.length()-ext.length(),ext.length());
    if(filename==ext)
        return true;
    return false;
}

char* string_to_char(string s){
    char *result = new char [s.length()+1];
    for(unsigned int i=0; i<s.length(); i++){
        result[i] = s[i];
    }
    result[s.length()] = 0;
    return result;
}
