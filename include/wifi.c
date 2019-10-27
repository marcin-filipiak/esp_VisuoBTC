////////////////skanowanie sieci/////////////////
void scan () {


  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  Serial.println("Skanowanie rozpoczete");


  int n = WiFi.scanNetworks();
  Serial.println("Skanowanie zakonczone");
  if (n == 0)
    Serial.println("");
  else
  {
    Serial.print(n);
    Serial.println(" znalezionych sieci");
    for (int i = 0; i < n; ++i)
    {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");
}

////////////polaczenie z wifi/////////////////
void connect_wifi(){
  
      u8g2.setFont(u8g2_font_6x10_tf);
  
      char x=0;
      WiFi.begin(data.ssid, data.pass);
      
      u8g2.drawStr(0, 0, "Laczenie");
      
      while (WiFi.status() != WL_CONNECTED && x<7) {
        u8g2.drawStr(x*11, 10, "*");
        u8g2.sendBuffer();
        delay(1000);
        x++;
      }
      if (x<7){
        u8g2.drawStr(0, 20, "Polaczony");
        u8g2.sendBuffer();
        Serial.print("Polaczony\n");
      }
      else {
        Serial.print("Blad polaczenia\n");
        u8g2.drawStr(0, 20, "Blad polaczenia");
        u8g2.sendBuffer();
      }
      delay(2000);
}
