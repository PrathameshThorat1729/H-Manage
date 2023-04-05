#ifndef SRC_HMANAGE_LIB_LOGIN_HPP_
#define SRC_HMANAGE_LIB_LOGIN_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
using namespace std;

#define HOTEL_NAME_MAX 20
#define HOTEL_ADDRESS_MAX 50
#define HOTEL_PASSWORD_MAX 21

typedef struct {
  char name[HOTEL_NAME_MAX+1];
  char address[HOTEL_ADDRESS_MAX+1];
  char password[HOTEL_PASSWORD_MAX+1];
} LoginData;

class Login
{
private:
  string login_file;
  string hotels_file;
  Login(){};
  void setup_login();
  
public:
  static Login *get_instance();
  bool register_hotel(LoginData *login_data);
  void get_hotels(vector<string>* hotels);
  
};

#endif // SRC_HMANAGE_LIB_LOGIN_HPP_