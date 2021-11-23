//incluir la libreria
      #include <ArduinoJson.h>
      #include <Arduino.h>
      #include "DHT.h"
      #include <TimeLib.h>
      #include <Servo.h>
      #define DHTTYPE DHT11 

//variables
        #define LED 13 // led pin
        #define PIR 8 //pin pir
        #define echoPin 6 // Echo Pin
        #define trigPin 5 // Trigger Pin
        

        const int PINDHT = 3;    //pin sensor de humedad
        int cooler = 10; //Ventilador
        int val_pir = 0; //se guarda el valor del PIR
        int maximumRange = 200; // Maximo rango a detectar en cm
        int minimumRange = 0; // Minimo rango a detectar en cm
        int duration, distancia; // Duración usada para calcular la distancia      
        float h = 0;// se decalara variable para guardar la humedad
        float t = 0;// se declara variable para guadar la temperatura
        Servo servoMotor;
        DHT dht(PINDHT, DHTTYPE);
        
        //Vriables Tiempo
         int d , m, y, ho, mi, s;

//configuracion


        void setup() {
     
               setTime(9,19,40,21,05,2020); // parametrizar fecha y hora
               Serial.begin(9600); //inicializamos la comunicacion serial
                   pinMode(cooler, OUTPUT);         
                    pinMode(PINDHT, INPUT);
                      pinMode (PIR, INPUT);// pinMode (LED, OUTPUT);//configuracion de las salidas y las entradas
                       pinMode(trigPin, OUTPUT);//Trig es la salida del ultrasonido
                        pinMode(echoPin, INPUT);//Echo es la entrada de el sonido regresando al sensor.
                     //Serial.println("Se esta Iniciando el sensor PIR");
                     dht.begin(); // se inicia el sensor DHT
                     servoMotor.attach(9);
                     digitalWrite(cooler, LOW);
        
        }


      void loop() {
        //esperamos dos segundos entre medidas
            
            servoMotor.write(0);
            delay(2000);


                    /*mediante el ciclo de Trig y Echo es usado para determinar 
                       la distancia del objeto mas cercano emitiendo sonidos
                      y calculando el tiempo que tardan en regresar*/
                      time_t T= now();//captura de fecha y hora del computador

                              d=day(T);
                              m=month(T);
                              y=year(T);
                              
                              ho=hour(T);
                              mi=minute(T);
                              s=second(T);
                              
                   digitalWrite(trigPin, LOW); 
                   delayMicroseconds(2); 
                   digitalWrite(trigPin, HIGH);
                   delayMicroseconds(10); 
                   digitalWrite(trigPin, LOW);
                   duration = pulseIn(echoPin, HIGH);
                   distancia = duration/58.2;
                   h = dht.readHumidity();// DHT11, Leemos la humedad relativa
                   t = dht.readTemperature();//leemos la temperatura en grados centigrados (por defecto)
                   val_pir = digitalRead(PIR);
                   String FechayHora = String(y)+"-"+String(m)+"-"+String(d)+" "+String(ho)+":"+String(mi)+":"+String(s);
                   
                   

                   ///

                      //JSON

                         
                       
                     
                      if(val_pir==HIGH){

                                          //Serial.println("movimiento");
                                    StaticJsonDocument <300> doc;
                                    
                                        doc ["PIR"] = val_pir;
                                        doc ["Temperatura"]=t;
                                        doc ["Humedad"]=h;
                                        doc ["Distancia"]=distancia;
                                        doc ["Fecha"]=FechayHora;
                                        doc ["MENSAJE"]="movimiento";

                                        //Condicion del ultrasonido cuando este se encuentre a menos de 100cm
          
          if(distancia > 14){
            
          doc ["Mensaje2"]="La puerta esta abierta";
          
            }else{
                  doc ["Mensaje2"]="La puerta esta cerrada";
                 }
              
          //condicion DHT11
          if(h>70){
                  doc ["Mensaje3"]="Humedad mas de 70 %";
                  }else{
                        doc ["Mensaje3"]="La humedad esta normal";
                       } 
            
          if(t>30){
            
                doc ["Mensaje4"]="Temperatura mas de 30°";
                digitalWrite(cooler, LOW);
                
            }else{
                  doc ["Mensaje4"]="La temperatura es normal";
                  digitalWrite(cooler, HIGH);
              digitalWrite(LED, LOW);
              } 

             if(h = 9 && mi==20 || h == 16 && mi == 20 ){

                if ( s==40 || s == 41 || s == 42 || s == 41 ||s == 43 ){
                    servoMotor.write(40);
                    delay(1000); //abrir
                    servoMotor.write(0);
                    delay(500); //cerrar 
                }      
    
              }    
                                          
                                            serializeJsonPretty(doc, Serial); //imprimir datos en Json
                                              Serial.println();
                                            digitalWrite(LED, HIGH);
                                                delay(4000);
                                            digitalWrite(LED, LOW);
                                                delay(3000);
      
                                       
                        
                        }else{
                          
                          StaticJsonDocument <300> doc;
                                         
                                        doc ["PIR"] = val_pir;
                                        doc ["Temperatura"]=t;
                                        doc ["Humedad"]=h;
                                        doc ["Distancia"]=distancia;
                                        doc ["Fecha"]=FechayHora;
                                        doc ["MENSAJE"]="sin movimiento";

                                        //Condicion del ultrasonido cuando este se encuentre a menos de 100cm
          
          if(distancia > 10){
            
          doc ["Mensaje2"]="La puerta esta abierta";
          
            }else{
              doc ["Mensaje2"]="La puerta esta cerrada";
              }
              
          //condicion DHT11
          if(h>70){
            doc ["Mensaje3"]="Humedad mas de 70 %";
            }else{
              doc ["Mensaje3"]="La humedad esta normal";
              } 
            
          if(t>30){
            
                doc ["Mensaje4"]="Temperatura mas de 30°";
                digitalWrite(cooler, LOW);
                
            }else{
                  doc ["Mensaje4"]="La temperatura es normal";
                  digitalWrite(cooler, HIGH);
              digitalWrite(LED, LOW);
              } 

    if(h = 9 && mi==20 || h == 16 && mi == 20 ){


        if (s == 41 || s == 42 || s == 43 || s == 44 ||s == 45 || s == 46 || s == 47){
           doc ["Alarma"]="Se realizo la alimentacion";
            servoMotor.write(40);
            delay(1000); //abrir
            servoMotor.write(0);
            delay(500); //cerrar
            
          }      
    
     }  
               serializeJsonPretty(doc, Serial); //imprimir datos en Json
                                              Serial.println();
                                            digitalWrite(LED, HIGH);
                                                delay(4000);
                                            digitalWrite(LED, LOW);
                                                delay(3000);
      
                          }

                                                        
}
