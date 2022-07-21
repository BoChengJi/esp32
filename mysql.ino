#include <WiFi.h>
#include <ModbusMaster.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#define ONE_WIRE_BUS 32
// instantiate ModbusMaster object
ModbusMaster node;

//IPAddress ip(192,168,23,62); 
IPAddress ip(87,87,87,10); 
OneWire oneWire(ONE_WIRE_BUS); // 建立 OneWire 物件
DallasTemperature DS18B20(&oneWire); // 建立 DS18B20 物件
uint8_t result;
uint16_t data[2];
IPAddress server_addr(192,168,23,87);  // change to you server ip, note its form split by "," not "."
int MYSQLPort =3306;   //mysql port default is 3306
char user[] = "root";              // Your MySQL user login username(default is root),and note to change MYSQL user root can access from local to internet(%)
char pass[] = "root";        // Your MYSQL password

const char ssid[]="auo_fmcs"; //修改為你家的WiFi網路名稱
const char pwd[]="auo_fmcs"; //修改為你家的WiFi密碼


char INSERT_SQL[] = "INSERT INTO tn.tn (temp) VALUES ('%f')";
char query[128];
float temperature;

WiFiClient client;            
MySQL_Connection conn((Client *)&client);


void setup()
{
  Serial.begin(115200);
  DS18B20.begin();
  
  

  
}
 
 
void loop()
{
    WiFi.mode(WIFI_STA); //設置WiFi模式
    while(WiFi.status()!=WL_CONNECTED){
    delay(2500);
    Serial.println("WiFi connecting");
    WiFi.begin(ssid,pwd);   
  }
  if (conn.connect(server_addr, 3306, user, pass)) {
     delay(1000);
  }
  else{
    Serial.println("Connection failed.");
  }  
  delay(2000);
  
  float temperature; //注意，溫度讀值帶小數，要用 float

  DS18B20.requestTemperatures();  //下指令開始轉換

  temperature = DS18B20.getTempCByIndex(0);  //讀取第一顆 DS18B20 的溫度
  //insert, change database name and values by string and char[]
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  //dtostrf(sensors.getTempCByIndex(0), 2, 2, temperature);
  sprintf(query, INSERT_SQL, temperature);
  //execute
  cur_mem->execute(query);
  delete cur_mem;
  Serial.println(temperature);
  
  delay(60000);
}
