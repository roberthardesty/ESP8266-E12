//IPAdresses, Subnets, Gateways
IPAddress HOME_STATIC_IP(192,168,1,60);
IPAddress HOME_STATIC_GATEWAY(192,168,1,1);
IPAddress HOME_STATIC_SUBNET(255,255,255,0);
//SSID's, and Passwords
char * HOME_SSID = "davidjohnson";
char * HOME_PASS = "ytsedrah";
//Known Clients/Vendors

//Enums
typedef enum NetworkMode{
  HOME_MODE, WORK_MODE, MONITOR_MODE
};
//Structs?
