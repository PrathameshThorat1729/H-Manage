#include "H-Manage/login/login.hpp"

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
  this->is_logged = false;
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
  
  this->login_file = (data_dir + "login.dat").c_str();
  this->hotels_file = (data_dir + "hotels.dat").c_str();
}

LoginErrors Login::register_hotel(LoginData *login_data)
{
  vector<string> hotels;
  get_hotels(&hotels);
  
  if(std::find(hotels.begin(), hotels.end(), login_data->name) != hotels.end()){
    return HOTEL_ALREADY_REGISTERED;
  }
  
  FILE* login_data_file = fopen(this->login_file.c_str(), "ab");
  FILE* hotel_names_file = fopen(this->hotels_file.c_str(), "ab");
  
  if (!login_data_file || !hotel_names_file)
    return INTERNAL_ERROR;
    
  fwrite((char *)login_data, sizeof(LoginData), 1, login_data_file);
  fwrite(login_data->name, HOTEL_NAME_MAX+1, 1, hotel_names_file);
  fclose(login_data_file);
  fclose(hotel_names_file);
  
  return NO_ERROR;
}

LoginErrors Login::get_hotels(vector<string> *hotels)
{
  FILE* hotel_names_file = fopen(this->hotels_file.c_str(), "rb");
  
  if (!hotel_names_file)
    return INTERNAL_ERROR;
  
  char name[HOTEL_NAME_MAX+1] = {'\0'};
  while(1)
  {
    fread(&name, HOTEL_NAME_MAX+1, 1, hotel_names_file);
    if(feof(hotel_names_file))
      break;
    hotels->push_back(name);
  }
  fclose(hotel_names_file);
  
  return NO_ERROR;
}

LoginErrors Login::login(size_t id, char *hotel_name, char *password)
{
  this->is_logged = false;
  FILE *login_data_file = fopen(this->login_file.c_str(), "rb");
  
  if (!login_data_file)
    return INTERNAL_ERROR;
  
  LoginData _hotel;
  fseek(login_data_file, (id - 1) * sizeof(LoginData), SEEK_SET);
  fread((char *)&_hotel, sizeof(LoginData), 1, login_data_file);
  
  if (!strcmp(hotel_name, _hotel.name) &&
    !strcmp(password, _hotel.password))
  {
    strcpy(this->logged_hotel.name, _hotel.name);
    strcpy(this->logged_hotel.address, _hotel.address);
    this->is_logged = true;
    this->logged_id = id;
    return NO_ERROR;
  }
  
  fclose(login_data_file);
  
  return INVALID_CREDENTIALS;
}

LoginErrors Login::delete_logged_hotel()
{
  if (!this->is_logged)
    return INTERNAL_ERROR;
  
  FILE *temp_data_file = tmpfile();
  FILE *temp_hnames_file = tmpfile();
  size_t id = logged_id;
  if (!temp_data_file || !temp_hnames_file)
    return INTERNAL_ERROR;
  
  // Copy content to temp files 
  FILE *login_data_file = fopen(this->login_file.c_str(), "rb");
  FILE *hotel_names_file = fopen(this->hotels_file.c_str(), "rb");
  
  if (!login_data_file || !hotel_names_file)
    return INTERNAL_ERROR;
  
  LoginData _lgd;
  char _name[HOTEL_NAME_MAX+1]={'\0'};
  for (int i = 1; i < id; i++)
  {
    fread((char *)&_lgd, sizeof(LoginData), 1, login_data_file);
    fread(&_name, HOTEL_NAME_MAX+1, 1, hotel_names_file);
    
    fwrite((char *)&_lgd, sizeof(LoginData), 1, temp_data_file);
    fwrite(&_name, HOTEL_NAME_MAX+1, 1, temp_hnames_file);
  }
  
  // Skip Content of current logged ID
  fseek(login_data_file, id * sizeof(LoginData), SEEK_SET);
  fseek(hotel_names_file, id * (HOTEL_NAME_MAX + 1), SEEK_SET);
  while(1)
  {
    fread((char *)&_lgd, sizeof(LoginData), 1, login_data_file);
    fread(&_name, HOTEL_NAME_MAX+1, 1, hotel_names_file);
    
    if (feof(hotel_names_file) || feof(login_data_file))
      break;
    
    fwrite((char *)&_lgd, sizeof(LoginData), 1, temp_data_file);
    fwrite(&_name, HOTEL_NAME_MAX+1, 1, temp_hnames_file);
  }
  fclose(login_data_file);
  fclose(hotel_names_file);
  
  rewind(temp_hnames_file);
  rewind(temp_data_file);
  
  // Write temp content to permanent files 
  login_data_file = fopen(this->login_file.c_str(), "wb");
  hotel_names_file = fopen(this->hotels_file.c_str(), "wb");
  
  if (!login_data_file || !hotel_names_file)
    return INTERNAL_ERROR;
  
  while(1)
  {
    fread((char *)&_lgd, sizeof(LoginData), 1, temp_data_file);
    fread(&_name, HOTEL_NAME_MAX+1, 1, temp_hnames_file);
    
    if (feof(temp_hnames_file) || feof(temp_data_file))
      break;
    
    fwrite((char *)&_lgd, sizeof(LoginData), 1, login_data_file);
    fwrite(&_name, HOTEL_NAME_MAX+1, 1, hotel_names_file);
  }
  
  this->is_logged = false;
  this->logged_id = 0;
  
  strcpy(this->logged_hotel.name, "");
  strcpy(this->logged_hotel.address, "");
  strcpy(this->logged_hotel.password, "");
  
  fclose(login_data_file);
  fclose(hotel_names_file);
  fclose(temp_hnames_file);
  fclose(temp_hnames_file);
  
  return NO_ERROR;
}

bool Login::is_hotel_logged()
{
  return this->is_logged;
}

bool Login::get_logged_hotel_id(size_t *id)
{
  if (!this->is_logged)
    return false;
  
  *id =this->logged_id;
  return true;
}

bool Login::get_logged_hotel_name(string *name)
{
  if (!this->is_logged)
    return false;
  
  *name =this->logged_hotel.name;
  return true;
}

bool Login::get_logged_hotel_address(string *address)
{
  if (!this->is_logged)
    return false;
  
  *address =this->logged_hotel.address;
  return true;
}
