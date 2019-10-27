
#include <ArduinoJson.h>


#define holed 31
#define xoled 99

int course[xoled] = {0};


#define btc_width 10
#define btc_height 17

////////////Ikona BTC//////////////
static unsigned char btc_bits[] = {
   0x50, 0x00, 0x50, 0x00, 0xff, 0x00, 0x04, 0x01, 0x04, 0x02, 0x04, 0x02,
   0x04, 0x02, 0x04, 0x01, 0xfc, 0x00, 0x04, 0x01, 0x04, 0x02, 0x04, 0x02,
   0x04, 0x02, 0x04, 0x02, 0xff, 0x03, 0x50, 0x00, 0x50, 0x00 };

void oled_btcicon(char x, char y){
  u8g2.drawXBMP(x, y, btc_width, btc_height, btc_bits);
}

/////////////Pobranie danych z sieci////////
String getdata(){
  String j = "";
  if (WiFi.status() == WL_CONNECTED) { 
        
    HTTPClient http; 
    http.begin("http://api.noweenergie.org/index.php?VisuoBtc/price/"+String(xoled+2));  
    int httpCode = http.GET();             
    if (httpCode > 0) {                    
      j = http.getString();
	  Serial.println("dane :"+j+":");   
    }
    http.end();                           
  }
  return j;
}


///////////Wyszukiwanie minimum///////
int mini(int *t){
	//przyjecie jako minimum pierwszego elementu
	int min = t[0];
	//przegladanie wszystkich elementow
	for (int x=0; x<xoled; x++){
		//znaleziono nowe minimum
		if (t[x] < min && t[x] != -2147483648){
			min = t[x];
		}
	}
	return min;
}

///////////Wyszukiwanie maksimum///////
int maxi(int *t){
	//przyjecie jako minimum pierwszego elementu
	int max = t[0];
	//przegladanie wszystkich elementow
	for (int x=0; x<xoled; x++){
		//znaleziono nowe maksimum
		if (t[x] > max){
			max = t[x];
		}
	}
	return max;
}


//////////obliczanie ile pikseli na jednostke///////
int pkty(float pkt, int min, int datay){
	//obliczenie punktu y i odbicie w osi x bo 0.0 jest u gory ekranu
	return int(holed-((datay-min) * pkt));
}

///////dodanie do tablicy//////////////
void addtoarray(int *a, int element){

	//przesuniecie o jeden do tylu
	for(int x=0; x<xoled-1; x++){
		a[x]=a[x+1];
	}
	//dodanie nowego na ostatnim miejscu
	a[xoled-1] = element;
}


/////////////czyszczenie danych////////////////////////
void clear_course(int *a){
  for(int x=0;x<xoled;x++){
    a[x]=-2147483648;
  }
}


//////////////Pokaz dane na ekranie////////////////////
void show_data(int *course){
	
	//DynamicJsonBuffer  jsonBuffer;
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(getdata());

	 if (root.success()){
			//dane przychodza od najnowszych do najstarszych, by wykres byl od lewej do prawej czytamy od tylu
			for(int x=xoled; x>0; x--){
				int price = (int)root["price"][x];
				//Serial.println("pobrano kurs :"+String(price)+":");
				addtoarray(course,price);
			}
	 }

	//aktualna cena
	int pricenow =(int)root["pricenow"];

	int data_show[xoled];

	//wyszukanie minimow i maksimow
	float max = maxi(course);
	float min = mini(course);
	//obliczanie wysokosci jednostki danych na wykresie
	float pkt = (float)(holed/(max-min));

	//Serial.println("max:"+String(max)+" min:"+String(min)+" pkt:"+String(pkt));

	//przygotowanie danych
	for (int x=0; x<xoled; x++){
		int y = pkty(pkt,min,course[x]);
		data_show[x] = y;
		//Serial.println("kurs "+String(course[x])+" "+String(x)+" "+String(y));
	}

	//wyswietlenie danych
	u8g2.clearBuffer();
	for (int x=0; x<xoled-1; x++){
		u8g2.drawLine(x, data_show[x], x+1, data_show[x+1]);
	}

    //przygotowanie aktualnej ceny w formacie dla ekranu
	char buf[10];
    sprintf (buf, "%d", pricenow);
    //napis na ekran
    u8g2.setFont(u8g2_font_4x6_tf);
    oled_btcicon(115,1);
	//u8g2.drawStr(105,10, "BTC");
	u8g2.drawStr(105,24, buf);

	//wyslanie danych na ekran
	u8g2.sendBuffer();


}
