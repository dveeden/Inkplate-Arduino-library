/*
<<<<<<< HEAD:examples/Inkplate10/Projects/Inkplate10_News_API/Inkplate10_News_API.ino
    Inkplate10_News_API example for Soldered Inkplate 10
=======
    News API example for e-radionica.com Inkplate 10
>>>>>>> 93efc99f1d39c8fc1fa726ac84022b59e384def1:examples/Inkplate10/Projects/News_API_Example/News_API_Example.ino
    For this example you will need only USB cable and Inkplate 10.
    Select "e-radionica Inkplate10" or "Soldered Inkplate10" from Tools -> Board menu.
    Don't have "e-radionica Inkplate10" or "Soldered Inkplate10" option? Follow our tutorial and add it:
    https://soldered.com/learn/add-inkplate-6-board-definition-to-arduino-ide/

    This example will show you how you can use Inkplate 10 to display API data.
    Here we use News API to get headline news and short description and display
    them on the Inkplate screen. For this you will need an API key which you can obtain
    here: https://newsdata.io

    IMPORTANT:
    Make sure to change your timezone and wifi credentials below
    Also have ArduinoJSON installed in your Arduino libraries, download here: https://arduinojson.org/

    Want to learn more about Inkplate? Visit www.inkplate.io
<<<<<<< HEAD:examples/Inkplate10/Projects/Inkplate10_News_API/Inkplate10_News_API.ino
    Looking to get support? Write on our forums: https://forum.soldered.com/
    28 July 2020 by Soldered
=======
    Looking to get support? Write on our forums: http://forum.e-radionica.com/en/
    29 June 2022 by Soldered
>>>>>>> 93efc99f1d39c8fc1fa726ac84022b59e384def1:examples/Inkplate10/Projects/News_API_Example/News_API_Example.ino
*/

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#if !defined(ARDUINO_INKPLATE10) && !defined(ARDUINO_INKPLATE10V2)
#error "Wrong board selection for this example, please select e-radionica Inkplate10 or Soldered Inkplate10 in the boards menu."
#endif

//---------- CHANGE HERE  -------------:

// Put in your ssid and password
char ssid[] = "";
char pass[] = "";
<<<<<<< HEAD:examples/Inkplate10/Projects/Inkplate10_News_API/Inkplate10_News_API.ino
char api_key_news[] = ""; //You can obtain one here: https://newsapi.org/
=======
char api_key_news[] = ""; //You can obtain one here: https://newsdata.io
>>>>>>> 93efc99f1d39c8fc1fa726ac84022b59e384def1:examples/Inkplate10/Projects/News_API_Example/News_API_Example.ino

//----------------------------------

// Include Inkplate library to the sketch
#include "Inkplate.h"

// Our networking functions, declared in Network.cpp
#include "Network.h"
#include "Inter12pt7b.h"
#include "GT_Pressura16pt7b.h"

// create object with all networking functions
Network network;

// create display object
Inkplate display(INKPLATE_1BIT);

// Delay between API calls in miliseconds (first 60 represents minutes so you can change to your need)
#define DELAY_MS (uint32_t)60 * 60 * 1000

// Variable for counting partial refreshes
RTC_DATA_ATTR unsigned refreshes = 0;

// Constant to determine when to full update
const int fullRefresh = 20;

int nentities = 0; // Number of entities

void setup()
{
    // Begin serial communitcation with PC
    Serial.begin(115200);
    // Initial display settings
    display.begin();
    display.setTextWrap(false);

    // Our begin function
    network.begin();
    struct news *entities; // Create pointer to struct array where are news contained
    entities = network.getData(); // Create an API call and get data
    drawNews(entities); // Call function drawNews and show new on display
    display.display(); // Display all on display
    ++refreshes;

    // Go to sleep before checking again
    esp_sleep_enable_timer_wakeup(1000 * DELAY_MS);
    (void)esp_deep_sleep_start();
}

void drawNews(struct news *entities)
{
    display.setRotation(3); // Set rotation of display to set it in portrait mode
    uint8_t coll = 0; // Counter for collumns
    uint16_t y = 32;  // Offset from top for first collumn
    uint8_t rows = 0; // Counter for printed rows of text
    int i = 0; // Counter for entities
    while (coll < 3) // Print three collumns
    {
        display.setCursor(10 + 274 * coll, y); // Set cursor accordingly to collumn and height of printed text
        display.setFont(&GT_Pressura16pt7b); // Set font for titile
        uint16_t cnt = 0; // Counter for printed characters
        while (*(entities[i].title + cnt) != '\0') // Print until stop character
        {  // Check if characters are printed to end of line, if yes set new line
            if (display.getCursorX() > 274 * coll + 230 || (*(entities[i].title + cnt) == ' ' && display.getCursorX() > 274 * coll + 210))
            {   // Check if characters are printed to end of line, if yes set new line
                *(entities[i].title + cnt) == ' ' ? cnt++ : 0; // Do not print space on begining of line
                rows++; // Increase row index
                y += 32; // Increase height for new line by 32 pixels
                display.setCursor(10 + 274 * coll, y); // Set cursor on beggining of next line
            }
            
            if (display.getCursorY() > 1160) // If printed to bottom of page, create new collumn
            {
                coll++; // Increase collumn index
                y = 32; //  Offset from top for next collumn
                display.setCursor(10 + 274 * coll, y); // Set cursor on beggining of next collumn
            }
            display.print(*(entities[i].title + cnt)); // Print one character
            cnt++; // Increase counter for characters
        }
        y = y + 40; //Offset from title to content
        display.setCursor(10 + 274 * coll, y); // Set cursor to print content
        display.print("  "); // Print few spaces or one tabulator
        
        cnt = 0; // Set characters counter to 0

        display.setFont(&Inter12pt7b); // Set font for content
        while (*(entities[i].content + cnt) != '\0') // Print until stop character
        { // Check if characters are printed to end of line, if yes set new line
            if (display.getCursorX() > 274 * coll + 230 || (*(entities[i].content + cnt) == ' ' && display.getCursorX() > 274 * coll + 210))
            { // Check if characters are printed to end of line, if yes set new line
                *(entities[i].content + cnt) == ' ' ? cnt++ : 0; // Do not print space on begining of line
                rows++; // Increase row index
                y += 26; // Increase height for new line by 26 pixels
                display.setCursor(10 + 274 * coll, y); // Set cursor on beggining of next line
            } 
            
            if (display.getCursorY() > 1180) // If printed to bottom of page, create new collumn
            {
                coll++; // Increase collumn index
                y = 32; //  Offset from top for next collumn
                display.setCursor(10 + 274 * coll, y); //Set cursor on beggining of next collumn
            }
            display.print(*(entities[i].content + cnt)); // Print one character
            cnt++; // Increase counter for characters
        }
        y += 48; // Make offset for next new
        i++; // Increase index of news
        if(i >= nentities) // If index of news is greater than number of new, break out of while loop
        {
          break;
        }
    }

}

void loop()
{
    // Never here
}
