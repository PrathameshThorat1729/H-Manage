#include <iostream>
#include <vector>
#include "H-Manage/login/login.hpp"
using namespace std;

int main()
{
  Login lg = *Login::get_instance();
 
  vector<string> hotels;
  lg.get_hotels(&hotels);
  
  for (auto i : hotels)
    cout << "-" << i << endl;
  
  return 0;
}