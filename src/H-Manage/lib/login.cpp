#include "H-Manage/lib/login.hpp"

// Singleton Implementation for DLL
/* https://stackoverflow.com/questions/21333935/using-a-singleton-across-dll-boundary/31181493#31181493 */
Login *Login::get_instance()
{
  static Login *_instance = nullptr;
  static bool is_setup_done = false;
  
  if (_instance == NULL)
    _instance = new Login();
  if (is_setup_done == false)
  {
    _instance->setup_login();
    is_setup_done = true;
  }
  
  return _instance;
}

void Login::setup_login()
{
  char file_seprator = filesystem::path::preferred_separator;
  string data_dir;
  
// Getting Data Storing Directory ( OS dependent )
#ifdef _WIN32
  data_dir = getenv("USERPROFILE");
#else
  data_dir = getenv("HOME");
#endif

  data_dir += file_seprator;
  data_dir += ".hmanage";
  data_dir += file_seprator;

  if (!filesystem::exists(data_dir))
  {
    try {
      filesystem::create_directory(data_dir);
    } catch (...) {
      exit(1);
    }
  }
  
  this->login_file = data_dir + "login.dat";
  this->hotels_file = data_dir + "hotels.dat";
}

bool Login::register_hotel(LoginData *login_data)
{
  FILE* login_data_file = fopen(this->login_file.c_str(), "ab");
  FILE* hotel_names_file = fopen(this->hotels_file.c_str(), "ab");
  
  if (!login_data_file || !hotel_names_file)
    return false;
    
  fwrite((char *)login_data, sizeof(LoginData), 1, login_data_file);
  fwrite(login_data->name, HOTEL_NAME_MAX+1, 1, hotel_names_file);
  fclose(login_data_file);
  fclose(hotel_names_file);
  
  return true;
}

void Login::get_hotels(vector<string> *hotels)
{
  FILE* hotel_names_file = fopen(this->hotels_file.c_str(), "rb");
  
  if (!hotel_names_file)
    return;
  
  char name[HOTEL_NAME_MAX+1] = {'\0'};
  while(1)
  {
    fread(&name, HOTEL_NAME_MAX+1, 1, hotel_names_file);
    if(feof(hotel_names_file))
      break;
    hotels->push_back(name);
  }
  fclose(hotel_names_file);
}