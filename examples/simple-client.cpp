#pragma comment(lib, "Ws2_32.lib")

#include "Socket.h"
#include <iostream>

using namespace std;

int main() {

  try {
    int er;
    SocketClient s("", 2000, er);

	string clientMessage;
    /*s.SendLine("Host: www.renenyffenegger.ch");
    s.SendLine("");*/

    while (1) {
	  getline(cin, clientMessage);
      int statusCode;
      s.SendLine(clientMessage, statusCode);
      string l = s.ReceiveLine(statusCode);
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
