#include <iostream>
#include <vector>
#include "H-Manage/lib/login.hpp"
using namespace std;

int main()
{
  Login lg = *Login::get_instance();
  Login lg2 = *Login::get_instance();
  /*LoginData d1 = {"Name 1", "Address 1","Password 1"};
  LoginData d2 = {"Name 2", "Address 2","Password 2"};
  LoginData d3 = {"Name 3", "Address 3","Password 3"};
  LoginData d4 = {"Name 4", "Address 4","Password 3"};
  lg.register_hotel(&d1);
  lg.register_hotel(&d2);
  lg.register_hotel(&d3);
  lg.register_hotel(&d4);*/
  
  vector<string> hotels;
  lg.get_hotels(&hotels);
  lg2.get_hotels(&hotels);
  for (auto name : hotels)
    cout << "-" << name << "-";
  return 0;
}