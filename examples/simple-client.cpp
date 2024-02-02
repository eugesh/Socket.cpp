#pragma comment(lib, "Ws2_32.lib")

#include "Socket.h"
#include <iostream>

using namespace std;

int main() {

  try {
    SocketClient s("", 2000);

	string clientMessage;
    /*s.SendLine("Host: www.renenyffenegger.ch");
    s.SendLine("");*/

    while (1) {
	  getline(cin, clientMessage);
	  s.SendLine(clientMessage);
	  string l = s.ReceiveLine();
      if (l.empty()) break;
      cout << l;
      cout.flush();
    }

  } 
  catch (const char* s) {
    cerr << s << endl;
  } 
  catch (std::string s) {
    cerr << s << endl;
  } 
  catch (...) {
    cerr << "unhandled exception\n";
  }

  return 0;
}
