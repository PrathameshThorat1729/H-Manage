#ifndef HMANAGE_LIB_LOGIN_HPP_
#define HMANAGE_LIB_LOGIN_HPP_

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

typedef enum {
  HOTEL_ALREADY_REGISTERED,
  INTERNAL_ERROR,
  INVALID_CREDENTIALS,
  NO_ERROR
} LoginErrors;

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
  bool is_logged;
  size_t logged_id = 0; // 0 if not logged
  LoginData logged_hotel;
  Login(){};
  void setup_login();
  
public:
  static Login *get_instance();
  
  LoginErrors register_hotel(LoginData *login_data);
  LoginErrors get_hotels(vector<string> *hotels);
  
  LoginErrors login(size_t id, char *hotel_name, char *password);
  LoginErrors delete_logged_hotel();
  
  bool is_hotel_logged();
  bool get_logged_hotel_id(size_t *id);
  bool get_logged_hotel_name(string *name);
  bool get_logged_hotel_address(string *address);
};

#endif // HMANAGE_LIB_LOGIN_HPP_