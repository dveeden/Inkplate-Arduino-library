/*
   Weather station example for e-radionica.com Inkplate 6 plus
   For this example you will need only USB cable and Inkplate 6 plus.
   Select "Inkplate 6 plus(ESP32)" from Tools -> Board menu.
   Don't have "Inkplate 6 plus(ESP32)" option? Follow our tutorial and add it:
   https://e-radionica.com/en/blog/add-inkplate-6-to-arduino-ide/

   This example will show you how you can use Inkplate 6 plus to display API data,
   e.g. Metaweather public weather API

   IMPORTANT:
   Make sure to change your desired city, timezone and wifi credentials below
   Also have ArduinoJSON installed in your Arduino libraries

   Want to learn more about Inkplate? Visit www.inkplate.io
   Looking to get support? Write on our forums: http://forum.e-radionica.com/en/
   11 February 2021 by e-radionica.com
*/

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_INKPLATE6PLUS
#error "Wrong board selection for this example, please select Inkplate 6 plus in the boards menu."
#endif

// ---------- CHANGE HERE  -------------:

// Time zone for adding hours
int timeZone = 2;

// City search query
char city[128] = "ZAGREB";

// Change to your wifi ssid and password
char *ssid = "";
char *pass = "";

// ----------------------------------

// Include Inkplate library to the sketch
#include "Inkplate.h"

// Header file for easier code readability
#include "Network.h"

// Including fonts used
#include "Fonts/Roboto_Light_120.h"
#include "Fonts/Roboto_Light_36.h"
#include "Fonts/Roboto_Light_48.h"

// Including icons generated by the py file
#include "icons.h"

// Delay between API calls
#define DELAY_MS 60000 * 3

// Inkplate object
Inkplate display(INKPLATE_1BIT);

// All our network functions are in this object, see Network.h
Network network;

// Contants used for drawing icons
char abbrs[32][16] = {"sn", "sl", "h", "t", "hr", "lr", "s", "hc", "lc", "c"};
const uint8_t *logos[16] = {icon_sn, icon_sl, icon_h, icon_t, icon_hr, icon_lr, icon_s, icon_hc, icon_lc, icon_c};
const uint8_t *s_logos[16] = {icon_s_sn, icon_s_sl, icon_s_h,  icon_s_t,  icon_s_hr,
                              icon_s_lr, icon_s_s,  icon_s_hc, icon_s_lc, icon_s_c};

RTC_DATA_ATTR char abbr1[16];
RTC_DATA_ATTR char abbr2[16];
RTC_DATA_ATTR char abbr3[16];
RTC_DATA_ATTR char abbr4[16];

// Variables for storing temperature
RTC_DATA_ATTR char temps[8][4] = {
    "0F",
    "0F",
    "0F",
    "0F",
};

// Variables for storing days of the week
RTC_DATA_ATTR char days[8][4] = {
    "",
    "",
    "",
    "",
};

// Variable for counting partial refreshes, in rtc data, survives reset
RTC_DATA_ATTR unsigned refreshes = 0;

// Constant to determine when to full update
const int fullRefresh = 5;

// Variables for storing current time and weather info
RTC_DATA_ATTR char currentTemp[16] = "0F";
RTC_DATA_ATTR char currentWind[16] = "0m/s";

RTC_DATA_ATTR char currentTime[16] = "9:41";

RTC_DATA_ATTR char currentWeather[32] = "-";
RTC_DATA_ATTR char currentWeatherAbbr[8] = "th";

// functions defined below
void drawWeather();
void drawCurrent();
void drawTemps();
void drawCity();
void drawTime();

void setup()
{
    // Begin serial and display
    Serial.begin(115200);
    display.begin();

    if (refreshes == 0)
    {
        // Welcome screen
        display.setCursor(50, 290);
        display.setTextSize(3);
        display.print(F("Welcome to Inkplate 6 plus weather example!"));
        display.display();

        display.clearDisplay();
        // Wait a bit before proceeding
        delay(5000);
    }

    if (refreshes % fullRefresh == 0)
    {
        // Calling our begin from network.h file
        network.begin(city);

        // If city not found, do nothing
        if (network.location == -1)
        {
            display.setCursor(50, 290);
            display.setTextSize(3);
            display.print(F("City not in Metaweather Database"));
            display.display();
            while (1)
                ;
        }

        // Get all relevant data, see Network.cpp for info
        network.getTime(currentTime);
        network.getTime(currentTime);
        network.getDays(days[0], days[1], days[2], days[3]);
        network.getData(city, temps[0], temps[1], temps[2], temps[3], currentTemp, currentWind, currentTime,
                        currentWeather, currentWeatherAbbr, abbr1, abbr2, abbr3, abbr4);

        // Draw data, see functions below for info
        drawTime();
        drawWeather();
        drawCurrent();
        drawTemps();
        drawCity();

        display.display();
    }
    else
    {
        // Refresh only the clock
        network.getTime(currentTime);


        drawTime();
        drawWeather();
        drawCurrent();
        drawTemps();
        drawCity();

        display.partialUpdate();
    }

    ++refreshes;

    // Go to sleep
    esp_sleep_enable_timer_wakeup(1000L * DELAY_MS);
    (void)esp_deep_sleep_start();
}

void loop()
{
    // Never here
}

// Function for drawing weather info
void drawWeather()
{
    // Searching for weather state abbreviation
    for (int i = 0; i < 10; ++i)
    {
        // If found draw specified icon
        if (strcmp(abbrs[i], currentWeatherAbbr) == 0)
            display.drawBitmap(63, 63, logos[i], 152, 152, BLACK);
    }

    // Draw weather state
    display.setTextColor(BLACK, WHITE);
    display.setFont(&Roboto_Light_36);
    display.setTextSize(1);
    display.setCursor(50, 340);
    display.println(currentWeather);
}

// Function for drawing current time
void drawTime()
{
    // Drawing current time
    display.setTextColor(BLACK, WHITE);
    display.setFont(&Roboto_Light_36);
    display.setTextSize(1);

    display.setCursor(1024 - 20 * strlen(currentTime), 35);
    display.println(currentTime);
}

// Function for drawing city name
void drawCity()
{
    // Drawing city name
    display.setTextColor(BLACK, WHITE);
    display.setFont(&Roboto_Light_36);
    display.setTextSize(1);

    display.setCursor(512 - 9 * strlen(city), 700);
    display.println(city);
}

// Function for drawing temperatures
void drawTemps()
{
    // Drawing 4 black rectangles in which temperatures will be written
    int rectWidth = 190;
    int rectSpacing = (1024 - rectWidth * 4) / 5;

    display.fillRect(1 * rectSpacing + 0 * rectWidth, 370, rectWidth, 280, BLACK);
    display.fillRect(2 * rectSpacing + 1 * rectWidth, 370, rectWidth, 280, BLACK);
    display.fillRect(3 * rectSpacing + 2 * rectWidth, 370, rectWidth, 280, BLACK);
    display.fillRect(4 * rectSpacing + 3 * rectWidth, 370, rectWidth, 280, BLACK);

    int textMargin = 6;

    display.setFont(&Roboto_Light_48);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);

    display.setCursor(1 * rectSpacing + 0 * rectWidth + textMargin, 380 + textMargin + 40);
    display.println(days[0]);

    display.setCursor(2 * rectSpacing + 1 * rectWidth + textMargin, 380 + textMargin + 40);
    display.println(days[1]);

    display.setCursor(3 * rectSpacing + 2 * rectWidth + textMargin, 380 + textMargin + 40);
    display.println(days[2]);

    display.setCursor(4 * rectSpacing + 3 * rectWidth + textMargin, 380 + textMargin + 40);
    display.println(days[3]);

    // Drawing temperature values into black rectangles
    display.setFont(&Roboto_Light_48);
    display.setTextSize(1);
    display.setTextColor(WHITE, BLACK);

    display.setCursor(1 * rectSpacing + 0 * rectWidth + textMargin, 380 + textMargin + 120);
    display.print(temps[0]);
    display.println(F("C"));

    display.setCursor(2 * rectSpacing + 1 * rectWidth + textMargin, 380 + textMargin + 120);
    display.print(temps[1]);
    display.println(F("C"));

    display.setCursor(3 * rectSpacing + 2 * rectWidth + textMargin, 380 + textMargin + 120);
    display.print(temps[2]);
    display.println(F("C"));

    display.setCursor(4 * rectSpacing + 3 * rectWidth + textMargin, 380 + textMargin + 120);
    display.print(temps[3]);
    display.println(F("C"));

    for (int i = 0; i < 18; ++i)
    {
        // If found draw specified icon
        if (strcmp(abbr1, abbrs[i]) == 0)
            display.drawBitmap(1 * rectSpacing + 0 * rectWidth + textMargin, 380 + textMargin + 150, s_logos[i], 48, 48,
                               WHITE, BLACK);
    }

    for (int i = 0; i < 18; ++i)
    {
        // If found draw specified icon
        if (strcmp(abbr2, abbrs[i]) == 0)
            display.drawBitmap(2 * rectSpacing + 1 * rectWidth + textMargin, 380 + textMargin + 150, s_logos[i], 48, 48,
                               WHITE, BLACK);
    }

    for (int i = 0; i < 18; ++i)
    {
        // If found draw specified icon
        if (strcmp(abbr3, abbrs[i]) == 0)
            display.drawBitmap(3 * rectSpacing + 2 * rectWidth + textMargin, 380 + textMargin + 150, s_logos[i], 48, 48,
                               WHITE, BLACK);
    }

    for (int i = 0; i < 18; ++i)
    {
        // If found draw specified icon
        if (strcmp(abbr4, abbrs[i]) == 0)
            display.drawBitmap(4 * rectSpacing + 3 * rectWidth + textMargin, 380 + textMargin + 150, s_logos[i], 48, 48,
                               WHITE, BLACK);
    }
}

// Current weather drawing function
void drawCurrent()
{
    // Drawing current information

    // Temperature:
    display.setFont(&Roboto_Light_120);
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);

    display.setCursor(308, 190);
    display.print(currentTemp);

    int x = display.getCursorX();
    int y = display.getCursorY();

    display.setFont(&Roboto_Light_48);
    display.setTextSize(1);

    display.setCursor(x, y);
    display.println(F("C"));

    // Wind:
    display.setFont(&Roboto_Light_120);
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);

    display.setCursor(604, 190);
    display.print(currentWind);

    x = display.getCursorX();
    y = display.getCursorY();

    display.setFont(&Roboto_Light_48);
    display.setTextSize(1);

    display.setCursor(x, y);
    display.println(F("m/s"));

    // Labels underneath
    display.setFont(&Roboto_Light_36);
    display.setTextSize(1);

    display.setCursor(270, 264);
    display.println(F("TEMPERATURE"));

    display.setCursor(630, 264);
    display.println(F("WIND SPEED"));
}
