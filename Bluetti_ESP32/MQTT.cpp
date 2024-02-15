#include "BluettiConfig.h"
#include "MQTT.h"
#include "BWifi.h"
#include "BTooth.h"
#include "utils.h"
#include "display.h"
#include "config.h"

#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient mqttClient;  
PubSubClient client(mqttClient);
int publishErrorCount = 0;
unsigned long lastMQTTMessage = 0;
unsigned long previousDeviceStatePublish = 0;
unsigned long previousDeviceStateStatusPublish = 0;
unsigned long previousMqttReconnect = 0;

String map_field_name(enum field_names f_name){
   switch(f_name) {
      case DC_OUTPUT_POWER:
        return "dc_output_power";
        break; 
      case AC_OUTPUT_POWER:
        return "ac_output_power";
        break; 
      case DC_OUTPUT_ON:
        return "dc_output_on";
        break; 
      case AC_OUTPUT_ON:
        return "ac_output_on";
        break; 
      case AC_OUTPUT_MODE:
        return "ac_output_mode";
        break; 
      case POWER_GENERATION:
        return "power_generation";
        break;       
      case TOTAL_BATTERY_PERCENT:
        return "total_battery_percent";
        break; 
      case DC_INPUT_POWER:
        return "dc_input_power";
        break;
      case AC_INPUT_POWER:
        return "ac_input_power";
        break;
      case AC_INPUT_VOLTAGE:
        return "ac_input_voltage";
        break;
      case AC_INPUT_FREQUENCY:
        return "ac_input_frequency";
        break;
      case PACK_VOLTAGE:
        return "pack_voltage";
        break;
      case INTERNAL_PACK_VOLTAGE:
        return "internal_pack_voltage";
        break;
      case SERIAL_NUMBER:
        return "serial_number";
        break;
      case ARM_VERSION:
        return "arm_version";
        break;
      case DSP_VERSION:
        return "dsp_version";
        break;
      case DEVICE_TYPE:
        return "device_type";
        break;
      case UPS_MODE:
        return "ups_mode";
        break;
      case AUTO_SLEEP_MODE:
        return "auto_sleep_mode";
        break;
      case GRID_CHARGE_ON:
        return "grid_charge_on";
        break;
      case INTERNAL_AC_VOLTAGE:
        return "internal_ac_voltage";
        break;
      case INTERNAL_AC_FREQUENCY:
        return "internal_ac_frequency";
        break;
      case INTERNAL_CURRENT_ONE:
        return "internal_current_one";
        break;
      case INTERNAL_POWER_ONE:
        return "internal_power_one";
        break;
      case INTERNAL_CURRENT_TWO:
        return "internal_current_two";
        break;
      case INTERNAL_POWER_TWO:
        return "internal_power_two";
        break;
      case INTERNAL_CURRENT_THREE:
        return "internal_current_three";
        break;
      case INTERNAL_POWER_THREE:
        return "internal_power_three";
        break;
      case PACK_NUM_MAX:
        return "pack_max_num";
        break;
      case PACK_NUM:
        return "pack_num";
        break;
      case PACK_BATTERY_PERCENT:
        return "pack_battery_percent";
        break;
      case INTERNAL_DC_INPUT_VOLTAGE:
        return "internal_dc_input_voltage";
        break;
      case INTERNAL_DC_INPUT_POWER:
        return "internal_dc_input_power";
        break;
      case INTERNAL_DC_INPUT_CURRENT:
        return "internal_dc_input_current";
        break;
      case INTERNAL_CELL01_VOLTAGE:
        return "internal_cell01_voltage";    
        break;
      case INTERNAL_CELL02_VOLTAGE:
        return "internal_cell02_voltage";    
        break;
      case INTERNAL_CELL03_VOLTAGE:
        return "internal_cell03_voltage";    
        break;
      case INTERNAL_CELL04_VOLTAGE:
        return "internal_cell04_voltage";    
        break;
      case INTERNAL_CELL05_VOLTAGE:
        return "internal_cell05_voltage";    
        break;
      case INTERNAL_CELL06_VOLTAGE:
        return "internal_cell06_voltage";    
        break;
      case INTERNAL_CELL07_VOLTAGE:
        return "internal_cell07_voltage";    
        break;
      case INTERNAL_CELL08_VOLTAGE:
        return "internal_cell08_voltage";    
        break;
      case INTERNAL_CELL09_VOLTAGE:
        return "internal_cell09_voltage";    
        break;
      case INTERNAL_CELL10_VOLTAGE:
        return "internal_cell10_voltage";    
        break;
      case INTERNAL_CELL11_VOLTAGE:
        return "internal_cell11_voltage";    
        break;
      case INTERNAL_CELL12_VOLTAGE:
        return "internal_cell12_voltage";    
        break;
      case INTERNAL_CELL13_VOLTAGE:
        return "internal_cell13_voltage";    
        break;
      case INTERNAL_CELL14_VOLTAGE:
        return "internal_cell14_voltage";    
        break;
      case INTERNAL_CELL15_VOLTAGE:
        return "internal_cell15_voltage";    
        break;
      case INTERNAL_CELL16_VOLTAGE:
        return "internal_cell16_voltage";    
        break;     
      case LED_MODE:
        return "led_mode";
        break;
      case POWER_OFF:
        return "power_off";
        break;
      case ECO_ON:
        return "eco_on";
        break;
      case ECO_SHUTDOWN:
        return "eco_shutdown";
        break;
      case CHARGING_MODE:
        return "charging_mode";
        break;
      case POWER_LIFTING_ON:
        return "power_lifting_on";
        break;
      case AC_INPUT_POWER_MAX:
        return "ac_input_power_max";
        break;
      case AC_INPUT_CURRENT_MAX:
        return "ac_input_current_max";
        break;
      case AC_OUTPUT_POWER_MAX:
        return "ac_output_power_max";
        break;
      case AC_OUTPUT_CURRENT_MAX:
        return "ac_output_current_max";
        break;
      case BATTERY_MIN_PERCENTAGE:
        return "battery_min_percentage";
        break;
      case AC_CHARGE_MAX_PERCENTAGE:
        return "ac_charge_max_percentage";
        break;
      default:
        #ifdef DEBUG
          Serial.println(F("Info 'map_field_name' found unknown field!"));
        #endif
        return "unknown";
        break;
   }
  
}

//There is no reflection to do string to enum
//There are a couple of ways to work aroung it... but basically are just "case" statements
//Wapped them in a fuction
String map_command_value(String command_name, String value){
  String toRet = value;
  value.toUpperCase();
  command_name.toUpperCase(); //force case indipendence

  //on / off commands
  if(command_name == "POWER_OFF" || command_name == "AC_OUTPUT_ON" || command_name == "DC_OUTPUT_ON" || command_name == "ECO_ON" || command_name == "POWER_LIFTING_ON") {
    if (value == "ON") {
      toRet = "1";
    }
    if (value == "OFF") {
      toRet = "0";
    }
  }

  //See DEVICE_EB3A enums
  if(command_name == "LED_MODE"){
    if (value == "LED_LOW") {
      toRet = "1";
    }
    if (value == "LED_HIGH") {
      toRet = "2";
    }
    if (value == "LED_SOS") {
      toRet = "3";
    }
    if (value == "LED_OFF") {
      toRet = "4";
    }
  }

  //See DEVICE_EB3A enums
  if(command_name == "ECO_SHUTDOWN"){
    if (value == "ONE_HOUR") {
      toRet = "1";
    }
    if (value == "TWO_HOURS") {
      toRet = "2";
    }
    if (value == "THREE_HOURS") {
      toRet = "3";
    }
    if (value == "FOUR_HOURS") {
      toRet = "4";
    }
  }

  //See DEVICE_EB3A enums
  if(command_name == "CHARGING_MODE"){
    if (value == "STANDARD") {
      toRet = "0";
    }
    if (value == "SILENT") {
      toRet = "1";
    }
    if (value == "TURBO") {
      toRet = "2";
    }
  }


  return toRet;
}

// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  String topic_path = String(topic);
  topic_path.toLowerCase();//in case we recieve DC_OUTPUT_ON instead of the expected dc_output_on
  
  Serial.print("MQTT Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(" Payload: ");
  String strPayload = String((char * ) payload);
  Serial.println(strPayload);
  
  bt_command_t command;
  command.prefix = 0x01;
  command.field_update_cmd = 0x06;

  for (int i=0; i< sizeof(bluetti_device_command)/sizeof(device_field_data_t); i++){
      if (topic_path.indexOf(map_field_name(bluetti_device_command[i].f_name)) > -1){
            command.page = bluetti_device_command[i].f_page;
            command.offset = bluetti_device_command[i].f_offset;
            
			      String current_name = map_field_name(bluetti_device_command[i].f_name);
            strPayload = map_command_value(current_name,strPayload);
    }
  }
  Serial.print(" Payload - switched: ");
  Serial.println(strPayload);

  command.len = swap_bytes(strPayload.toInt());
  command.check_sum = modbus_crc((uint8_t*)&command,6);
  lastMQTTMessage = millis();
  
  sendBTCommand(command);
}

void subscribeTopic(enum field_names field_name) {
#ifdef DEBUG
  Serial.println("[MQTT] subscribe to topic: " +  map_field_name(field_name));
#endif
  char subscribeTopicBuf[512];
  ESPBluettiSettings settings = get_esp32_bluetti_settings();

  sprintf(subscribeTopicBuf, "bluetti/%s/command/%s", settings.bluetti_device_id, map_field_name(field_name).c_str() );
  client.subscribe(subscribeTopicBuf);
  lastMQTTMessage = millis();

}

void publishTopic(enum field_names field_name, String value){
  char publishTopicBuf[1024];
  ESPBluettiSettings settings = get_esp32_bluetti_settings();
 
#ifdef DEBUG
  Serial.println("[MQTT] publish topic for field: " +  map_field_name(field_name));
#endif
  
  //sometimes we get empty values / wrong vales - all the time device_type is empty
  if (map_field_name(field_name) == "device_type" && value.length() < 3){

    //Serial.println(F("[MQTT] Error while publishTopic! 'device_type' can't be empty, reboot device)"));
    ESP.restart();
    Serial.println(F("[MQTT] Error while publishTopic! 'device_type' can't be empty, restarting BlueTooth Stack)"));
   // btResetStack();
   
  } 
  
  sprintf(publishTopicBuf, "bluetti/%s/state/%s", settings.bluetti_device_id, map_field_name(field_name).c_str() ); 
  if (strlen(settings.mqtt_server) == 0){
    AddtoMsgView(String(millis()) +": " + map_field_name(field_name) + " -> " + value); 
    #ifdef DEBUG
      Serial.println("[MQTT] No MQTT server specified!");
    #endif
  }else{
    lastMQTTMessage = millis();
    if (!client.publish(publishTopicBuf, value.c_str() )){
      publishErrorCount++;
      #ifdef DEBUG
        Serial.println("[MQTT] Publish error: " + String(lastMQTTMessage) + ": publish ERROR! " + map_field_name(field_name) + " -> " + value);
      #endif
      AddtoMsgView(String(lastMQTTMessage) + ": publish ERROR! " + map_field_name(field_name) + " -> " + value);
    }
    else{
      #ifdef DEBUG
        Serial.println("[MQTT] Last Message: " + String(lastMQTTMessage) + ": " + map_field_name(field_name) + " -> " + value);
      #endif
      AddtoMsgView(String(lastMQTTMessage) + ": " + map_field_name(field_name) + " -> " + value);
    }
  }
  
  if (map_field_name(field_name) == "total_battery_percent") {
    disp_setBattery(value);
  } else if (map_field_name(field_name) == "dc_input_power") {
    disp_setWattIn(value);
  } else if (map_field_name(field_name) == "ac_output_power") {
    disp_setWattOut(value);
  }
 
}

void publishDeviceState(){
  char publishTopicBuf[1024];

  ESPBluettiSettings settings = get_esp32_bluetti_settings();
  sprintf(publishTopicBuf, "bluetti/%s/state/%s", settings.bluetti_device_id, "device" ); 
  String value = "{\"IP\":\"" + WiFi.localIP().toString() + "\", \"MAC\":\"" + WiFi.macAddress() + "\", \"Uptime\":" + millis() + "}";
  #ifdef DEBUG
    Serial.println("[MQTT] PublishingDeviceState: "+value);
  #endif
  if (!client.publish(publishTopicBuf, value.c_str() )){
    publishErrorCount++;
  }
  lastMQTTMessage = millis();
  previousDeviceStatePublish = millis();
 
}

void publishDeviceStateStatus(){
  char publishTopicBuf[1024];

  ESPBluettiSettings settings = get_esp32_bluetti_settings();
  sprintf(publishTopicBuf, "bluetti/%s/state/%s", settings.bluetti_device_id, "device_status" ); 
  String value = "{\"MQTTconnected\":" + String(isMQTTconnected()) + ", \"BTconnected\":" + String(isBTconnected()) + "}"; 
  #ifdef DEBUG
    Serial.println("[MQTT] PublishingDeviceStateStatus: "+value);
  #endif
  if (!client.publish(publishTopicBuf, value.c_str() )){
    publishErrorCount++;
  }
  lastMQTTMessage = millis();
  previousDeviceStateStatusPublish = millis();
 
}

void initMQTT(){

    enum field_names f_name;
    ESPBluettiSettings settings = get_esp32_bluetti_settings();
    Serial.println("[MQTT] init MQTT");
    if (strlen(settings.mqtt_server) == 0){
      Serial.println("[MQTT] No MQTT server configured");
      return;
    }
    Serial.print("[MQTT] Connecting to MQTT at: ");
    Serial.print(settings.mqtt_server);
    Serial.print(":");
    Serial.println(F(settings.mqtt_port));
    
    client.setServer(settings.mqtt_server, atoi(settings.mqtt_port));
    client.setCallback(callback);

    bool connect_result;
    const char connect_id[] = "Bluetti_ESP32";
    if (settings.mqtt_username) {
        connect_result = client.connect(connect_id, settings.mqtt_username, settings.mqtt_password);
    } else {
        connect_result = client.connect(connect_id);
    }
    
    if (connect_result) {
        
      Serial.println(F("[MQTT] Connected to MQTT Server... "));

      // subscribe to topics for commands
      for (int i=0; i< sizeof(bluetti_device_command)/sizeof(device_field_data_t); i++){
        subscribeTopic(bluetti_device_command[i].f_name);
      }

      publishDeviceState();
      publishDeviceStateStatus();
    }

    
      
};

void handleMQTT(){
    ESPBluettiSettings settings = get_esp32_bluetti_settings();
    if (strlen(settings.mqtt_server) == 0){
      return;
    }
    if ((millis() - lastMQTTMessage) > (MAX_DISCONNECTED_TIME_UNTIL_REBOOT * 60000)){ 
      Serial.println(F("MQTT is disconnected over allowed limit, reboot device"));
      ESP.restart();
    }
      
    if ((millis() - previousDeviceStatePublish) > (DEVICE_STATE_UPDATE * 60000)){ 
      publishDeviceState();
    }
    if ((millis() - previousDeviceStateStatusPublish) > (DEVICE_STATE_STATUS_UPDATE * 60000)){ 
      publishDeviceStateStatus();
    }
    if (!isMQTTconnected() && publishErrorCount > 5){
      if ((millis() - previousMqttReconnect) > 5000)
      {
        previousMqttReconnect = millis();
        Serial.println(F("[MQTT] lost connection, try to reconnect"));
        #ifdef DISPLAYSSD1306
            disp_setMqttStatus(false);
        #endif
        client.disconnect();
        lastMQTTMessage=0;
        previousDeviceStatePublish=0;
        previousDeviceStateStatusPublish=0;
        publishErrorCount=0;
        AddtoMsgView(String(millis()) + ": MQTT connection lost, try reconnect");
        initMQTT();
      }
    }
    
    client.loop();
}

bool isMQTTconnected(){
    if (client.connected()){
      return true;
    }
    else
    {
      return false;
    }  
}

int getPublishErrorCount(){
    return publishErrorCount;
}
unsigned long getLastMQTTMessageTime(){
    return lastMQTTMessage;
}
unsigned long getLastMQTTDeviceStateMessageTime(){
    return previousDeviceStatePublish;
}
unsigned long getLastMQTTDeviceStateStatusMessageTime(){
    return previousDeviceStateStatusPublish;
}
