/*
   Inkplate_Picture_From_RAM example for soldered.com Inkplate 10
   For this example you will need USB cable and Inkplate 10.
   Select "Inkplate 10(ESP32)" from Tools -> Board menu.
   Don't have "Inkplate 10(ESP32)" option? Follow our tutorial and add it:
   https://e-radionica.com/en/blog/add-inkplate-6-to-arduino-ide/

   This example will show you how you can show picture loaded in RAM. 
   Inkplate can show picture in 1-bit or 3-bit mode.
   Inkplate will wake every 20 seconds change content on screen.

   Want to learn more about Inkplate? Visit www.inkplate.io
   Looking to get support? Write on our forums: http://forum.e-radionica.com/en/
   13 June 2022 by soldered.com
*/

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_INKPLATE10
#error "Wrong board selection for this example, please select Inkplate 10 in the boards menu."
#endif

#include "Inkplate.h"       // Include Inkplate library to the sketch
#include "picture1.h"       // Include .h files of 3 pictures. All three pictures were converted using Inkplate Image Converter
#include "picture2.h"
#include "picture3.h"       // This are headers in which are saved pictures that needs to be stored in RAM for showing.
                            // Any picture (in any format) can be converted in this type of header on this link:
                            // https://inkplate.io/home/image-converter/ just choose settings for your Inkplate
                            // and additional settings like dither and bit mode.

Inkplate display(INKPLATE_3BIT); // Create an object on Inkplate library and also set library into 3 Bit mode (gray)

void setup()
{   
    Serial.begin(115200);
    display.begin();        // Init Inkplate library (you should call this function ONLY ONCE)
    display.clearDisplay(); // Clear frame buffer of display
    
    display.drawImage(
        picture1, 50, 0, 1100,
        825); // Display picture from RAM  at location X=50, Y=0. It is also needed to specify width and height
              // of picture (1100x825 in this case). drawImage function iis overloaded function and this parameters
              // specify to use function which draws image on screen from internal RAM and this function calls other
              // functions (drawBitmap or drawBitmap3Bit depends in which mode is Inkplate (INKPLATE_3BIT or INKPLATE_1BIT))
              // which draws content from buffer in RAM.
    display.display(); // Refresh the screen with new picture
    
    delay(10000); // Wait a bit
    
    display.clearDisplay(); // Clear frame buffer of display
    
    display.drawBitmap3Bit(
       50, 0, picture2, 1100,
        825);  // This function is called automatically from drawImage if first parameter is
               // pointer to buffer in which picture is stored and Inkplate is in 3-bit mode.
               // In this example we are using this function only to show it's functionality 
               // and to show that this function can be used if you have problems with showing
               // 3-bit pictures on Inkplate.
    
    display.display(); // Refresh the screen with new picture
    
    delay(10000); // Wait a bit
    
    display.clearDisplay(); // Clear frame buffer of display
    
    display.drawBitmap3Bit(
        50, 0, picture3, 1100,
        825);  // Display selected picture at location X=0, Y=0. All three pictures have resolution of 1100x825 pixels

    display.display(); // Refresh the screen with new picture
}

void loop()
{
    // Nothing! If you use deep sleep, whole program should be in setup() because each time the board restarts, not in a
    // loop()! loop() must be empty!
}
