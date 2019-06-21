#include <SD.h>
#include <Adafruit_EPD.h>
#include <Adafruit_GFX.h>    // Core graphics library



#define SD_CS       5
#define SRAM_CS     6
#define EPD_CS      9
#define EPD_DC      10

#define SCLK        24
#define MISO        22

#define EPD_RESET   -1 // can set to -1 and share with microcontroller Reset!
#define EPD_BUSY    -1 // can set to -1 to not use a pin (will wait a fixed delay)

//Adafruit_SSD1675 display(250, 122, EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);
Adafruit_SSD1675 display(250, 122, 23, SCLK, EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, MISO, EPD_BUSY);

// (int width, int height, int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, int8_t CS, int8_t SRCS, int8_t MISO, int8_t BUSY=-1)
// (int width, int height, int8_t DC, int8_t RST, int8_t CS, int8_t SRCS, int8_t BUSY=-1)

bool drawBitmap = false;

void setup() {
//  Serial.begin(115200);
  //while (!Serial);
  Serial.println("2.13 inch EInk Featherwing test");

//  Serial.print("Initializing SD card...");
//  if (!SD.begin(SD_CS)) {
//    Serial.println("failed!");
//  } else {
//    drawBitmap = true;
//  }
//
//  File root = SD.open("/");
//  printDirectory(root, 0);
//  Serial.println("done!");

//  Serial.println("begin");
  display.begin();
  display.setTextWrap(true);
  display.setTextSize(1);
}

void loop() {
  Serial.println("TEST");
//  display.begin();
//  display.clearBuffer();
//  display.setCursor(10, 10);
//  display.setTextColor(EPD_BLACK);
//  display.print("Get as much education as you can. Nobody can take that away from you");
////  epd.setCursor(50, 70);
////  epd.setTextColor(EPD_BLACK);
////  epd.print("--Eben Upton");
//  display.display();
//  display.clearDisplay();
//  display.fillScreen(EPD_INVERSE);
  display.drawPixel(10,10, EPD_WHITE);
  display.display();
  delay(60*1000);
}

//void printDirectory(File dir, int numTabs) {
//  while (true) {
//
//    File entry =  dir.openNextFile();
//    if (! entry) {
//      // no more files
//      break;
//    }
//    for (uint8_t i = 0; i < numTabs; i++) {
//      Serial.print('\t');
//    }
//    Serial.print(entry.name());
//    if (entry.isDirectory()) {
//      Serial.println("/");
//      printDirectory(entry, numTabs + 1);
//    } else {
//      // files have sizes, directories do not
//      Serial.print("\t\t");
//      Serial.println(entry.size(), DEC);
//    }
//    entry.close();
//  }
//}
