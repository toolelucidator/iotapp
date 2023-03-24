//This example shows how to send JSON payload FCM to a recipient via HTTPv1 API.
//This new API requires OAuth2.0 authentication.

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Galaxy Z Flip 3"
#define WIFI_PASSWORD "holamundo$"

/* 2. Define the Firebase project host name (required) */
#define FIREBASE_HOST "functions2-f3ef6-default-rtdb.firebaseio.com"

/** 3. Define the Service Account credentials (required for token generation)
*
* This information can be taken from the service account JSON file.
*
* To download service account file, from the Firebase console, goto project settings,
* select "Service accounts" tab and click at "Generate new private key" button
*/
#define FIREBASE_PROJECT_ID "functions2-f3ef6"
#define FIREBASE_CLIENT_EMAIL "firebase-adminsdk-1mvm0@functions2-f3ef6.iam.gserviceaccount.com";

const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDMaHFE327cl63h\n3Tn6NAXE03KgNZ+yVULYoDaDnZMz7nrug+WEopfZAbjCklOyFPFAN0bRHeoj4fCF\npY/a3RSUvUSDSOfnAKW377Hx/B0y+1+pyJsMYT4W47JT2inzPrv7ecnDVluhi1NP\n0zeeqZsc2zrIMp2WChUtRFHstaggqW8pUB++ibotyMkUZlyxfqNh50ycQ+jUCw9i\nAahazihoF6ad+VLBnU6hL2uuE/IsDKaxmQNhFLHrbWrMvt85i48WFAs/0V9TZZJz\njiCGwg2oHp9qPmvkkaWU/xv6yvqGp3m7mNY56QRnTSLi0cZ0CAexhX1yXhlC1m7g\n7m9w7KJjAgMBAAECggEAGpz2h7W3eg0IMeywYb0S4IokgGtlkWhnEvaOYnB0++vu\nzCMWvOrBGRs6R6BEOV60p7G1YgCiaBhAooXocYxwjjUI4zbuH+CxCCkIYq3PzMAh\nt7n18JSwlXZWf3MGHIoXG39tyiWc5IQpTVlTV9r+kQaSDq1YYHKLnfXT3MfXTfkb\nKOZs3Sxqbq5UrVrGju4sXCKPJM6WR8t5k4ZePfUmLOJ4E9U8r5USyf6WeSxP03Z1\nlB+EgJtNLO/lwjjS1C7HHLI9gG99KESmjNQTZV+jiLTL1smYjo0so+ipD+h6oX1N\n5qWn6DnVYbVwPp0FFIVcv+IGPTIr271V0ruOuWqJEQKBgQDk11fZhSQROUTyzgSx\nHT3eFSRg/ghxixAMlqGgoDWLp9qC83GRGn3ZenopHl70ss2KEaq0ckpAdvaEHCNp\nEQK3nVN2vPnp9ieCSPyYa1FXlzfwK4DyECu5rc1Rj02sJ6ZcMvzJ9GPPl2dHZWIb\n5HH0Dy3/Pv08MYoOwmko0YttrQKBgQDkqsTq2V8IAnkiq5sY6p4ox0Num3rrFSnu\n9oAKKMhzugn8iLVN8G+SnxidHZgyqh8tlq1ibjoEuCM2tmkGre/PljvnYJbYerDE\nfvnu3yoeUuV31U8RIxejvrZaGBOkR11NBNAD7zisDh4+QZLWk/5JZqdlQxt3bBwQ\nl0njNKIyTwKBgD84Amv0LR+Y+6Q8J+2qD2Jf4tnss/ZyHW/s2R/fwosfRu1wRYp5\nkBTZkg/z9PWSw9eFw+vvuvWOJ9IdV+b5rQ9+pgUidcQdnZKPiUCuPR1+iBSQYCH/\nN6UnD5adiS6rEXldGJrOKhi6UezWZEEiXhy3aNSf2SOc5+/l41lEcklNAoGBANKf\nDOQ4thS9mnA52pE3wdhTuiPOKIkSJF7MRCGXcRNPX9Vw4nbYqRxHWLeXQ9Ie6rrm\nF+wbFWu1ZBIIMHMLkWEGw0SarjbVw64qOvIwMi5eMqiPHNXvGd4kUvhnRuhBJ4pC\njYuiM/QeyPKVGbh19noLiNJ33nvAY5VQlKknmOJ3AoGBAM4ITcL0yLyrO1B3R4v+\n+JgEa/0sFxwn6XeR/MKsYL0Hudb1NN0aylT4J59lfHiJxEwDKwgUGv+MWaRMRyEH\nfDzIRKamxNC2JfkrLEbeYlQLg2Q1ppjseE0CpfGmmdidfjEjVi0BPvjvIxnUtRrf\nATBDDYYWcEiSlAnz0FH/5Jb/\n-----END PRIVATE KEY-----\n";
/* 4. Define the ID token for client or device to send the message */
#define DEVICE_REGISTRATION_ID_TOKEN "etvdHD0LQMGY5RRKSyZo7L:APA91bFpeOSpD1tqDY5xe3Fxj1bKo-rjtKkYCO0m_v2dMBs7le3iMC7zoVbSsHUSf9U6oO0L4LEXwhZJwvSp-KD8La5xq3FkAYXNGXh9h2pssHkEipN3f8ZwejLUS-rujiICRlaXCMPy"

/* 5. Define the Firebase Data object */
FirebaseData fbdo;

/* 6. Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* 7. Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long lastTime = 0;

int count = 0;

void sendMessage();
int contador = 0;
void setup()
{
pinMode(2,OUTPUT);
Serial.begin(115200);

WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("Connecting to Wi-Fi");
while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(300);
}
Serial.println();
Serial.print("Connected with IP: ");
Serial.println(WiFi.localIP());
Serial.println();

/* Assign the project host (required) */
config.host = FIREBASE_HOST;

/* Assign the sevice account credentials and private key (required) */
config.service_account.data.client_email = FIREBASE_CLIENT_EMAIL;
config.service_account.data.project_id = FIREBASE_PROJECT_ID;
config.service_account.data.private_key = PRIVATE_KEY;

Firebase.begin(&config, &auth);

Firebase.reconnectWiFi(true);

#if defined(ESP8266)
//Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
fbdo.setBSSLBufferSize(1024, 1024);
#endif

//Set the size of HTTP response buffers in the case where we want to work with large data.
fbdo.setResponseSize(1024);

//sendMessage();
}

void loop()
{

//Verificar que la conexión a firebase exista


if(Firebase.ready()){


//Obtener un valor entero desde Firebase
if(Firebase.RTDB.getString(&fbdo,"/test/relay")){//Colección
Serial.print("Obteniendo Valor Entero, int = ");
Serial.println(fbdo.stringData());

if(1 == fbdo.stringData().toInt()) {
digitalWrite(2,HIGH);//CONECTAR UN LED EN D4
}
else {
digitalWrite(2,LOW);
}
}
else{
Serial.print("Error: ");
Serial.println(fbdo.errorReason());
}



/******************Subir datos desde el ESP8266***************************/
if(Firebase.RTDB.setInt(&fbdo,"/test/temperatura",contador)){
contador = contador +=1;
Serial.println(contador);
Serial.println("Valor guardado en Firebase");

}
else{
Serial.print("Error subiendo datos: ");
Serial.println(fbdo.errorReason());
}
delay(3000);
}



if (millis() - lastTime > 60 * 500)
{
lastTime = millis();

sendMessage();
}
}

void sendMessage()
{

Serial.println("------------------------------------");
Serial.println("Send Firebase Cloud Messaging...");

//Read more details about HTTP v1 API here https://firebase.google.com/docs/reference/fcm/rest/v1/projects.messages
FCM_HTTPv1_JSON_Message msg;

msg.token = DEVICE_REGISTRATION_ID_TOKEN;

msg.notification.body = "Hola desde un Esp8266";
msg.notification.title = "Esto usa FCM";

FirebaseJson json;
String payload;

//all data key-values should be string
//json.add("temp", "28");
//json.add("unit", "celsius");
json.add("data", "data");
json.toString(payload);
msg.data = payload.c_str();

if (Firebase.FCM.send(&fbdo, &msg)) //send message to recipient
{

Serial.println("PASSED");
Serial.println(Firebase.FCM.payload(&fbdo));
Serial.println("------------------------------------");
Serial.println();
}
else
{
Serial.println("FAILED");
Serial.println("REASON: " + fbdo.errorReason());
Serial.println("------------------------------------");
Serial.println();
}

count++;
}
