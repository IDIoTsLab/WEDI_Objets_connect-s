#include <PololuLedStrip.h>
#include <Wire.h>
#include <VL53L0X.h>

// Function declaration
void set_led(rgb_color color);
double hue2rgb(double p, double q, double t);
void hslToRgb(double hue, double saturation, double lightness, uint8_t& red, uint8_t& green, uint8_t& blue);
void distancesToRgb(int luminosity, int selected_color, rgb_color& color);


// Variables déclaration
VL53L0X sensor;
int data = 0;
int MapLuminosity = 0;
int MapColor = 0;

const int boutonPin = 2;

bool selection = false;


// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<12> ledStrip;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 57
rgb_color colors[LED_COUNT];

void setup()
{
  // Start up the serial port, for communication with the PC.
  Serial.begin(115200);
  Serial.println("Ready to receive colors.");

  Wire.begin();
  pinMode(boutonPin, INPUT_PULLUP);

  sensor.setMeasurementTimingBudget(200000);

  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1) {}
  }

  sensor.startContinuous();
}

void loop()
{
  // Si le bouton est pressé (LOW car pull-up activée)
  if (digitalRead(boutonPin) == LOW) {
    // Attendre que le bouton soit relâché pour éviter les multiples changements rapides
    while(digitalRead(boutonPin) == LOW);
    // Inverser l'état
    selection = !selection;
  }

  data = sensor.readRangeContinuousMillimeters();

  if (data != 8190){
    if (selection == false){
    MapLuminosity = map(data, 120, 300, 0, 100); 
    if (MapLuminosity < 0){ MapLuminosity = 0; }
    if (MapLuminosity > 100){ MapLuminosity = 100; }
    Serial.println("MapLuminosity :");
    Serial.print(MapLuminosity);}

    else {
      MapColor = map (data, 120, 500, 0, 100);
      if (MapColor < 0){ MapColor = 0; } 
      if (MapColor > 100){ MapColor = 100; }
      Serial.println("MapColor :");
      Serial.print(MapColor);
    }
   }

  else  { Serial.println("out of range"); }

  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  Serial.println();


// seb
  rgb_color color;

  distancesToRgb(MapLuminosity, MapColor, color);
  set_led(color);
  delay(50);
}




/*

  // If any digit is received, we will go into integer parsing mode
  // until all three calls to parseInt return an interger or time out.
  if (Serial.available())
  {
    char c = Serial.peek();
    if (!(c >= '0' && c <= '9'))
    {
      Serial.read(); // Discard non-digit character
    }
    else
    {
      // Read the color from the computer.
      rgb_color color;

      int luminosity = Serial.parseInt();
      int hue = Serial.parseInt();

      Serial.print("lum = ");
      Serial.println(luminosity);
      Serial.print("hue = ");
      Serial.println(hue);

      distancesToRgb(hue, luminosity, color);

      Serial.print(color.red);
      Serial.print(color.green);
      Serial.println(color.blue);

      set_led(color);
    }
  }
}*/

// Write the color to the RGB LED Strip
void set_led(rgb_color color)
{
  for(uint16_t i = 0; i < LED_COUNT; i++)
  {
    colors[i] = color;
  }
  // Write to the LED strip.
  ledStrip.write(colors, LED_COUNT);
}

double hue2rgb(double p, double q, double t)
{
	if (t < 0) t += 1;
	if (t > 1) t -= 1;
	if (t < 1 / 6.0) return p + (q - p) * 6 * t;
	if (t < 1 / 2.0) return q;
	if (t < 2 / 3.0) return p + (q - p) * (2 / 3.0 - t) * 6;
	return p;
}

void hslToRgb(double hue, double saturation, double lightness, rgb_color& color)
//TODO : passer en % (0 - 100)
{
	double r, g, b;

	if (saturation == 0)
	{
		r = g = b = lightness; // achromatic
	}
	else
	{
		auto q = lightness < 0.5 ? lightness * (1 + saturation) : lightness + saturation - lightness * saturation;
		auto p = 2 * lightness - q;
		r = hue2rgb(p, q, hue + 1 / 3.0);
		g = hue2rgb(p, q, hue);
		b = hue2rgb(p, q, hue - 1 / 3.0);
	}

	color.red = static_cast<uint8_t>(r * 255);
	color.green = static_cast<uint8_t>(g * 255);
	color.blue = static_cast<uint8_t>(b * 255);
}

void distancesToRgb(int luminosity, int selected_color, rgb_color& color)
//TODO : find clear names (luminosity and selected_color...)
{
	double r, g, b;
  double hue = selected_color / 100.0;
  double saturation = 1.;
  double lightness = luminosity / 100.0;

	if (saturation == 0)
	{
		r = g = b = lightness; // achromatic
	}
	else
	{
		auto q = lightness < 0.5 ? lightness * (1 + saturation) : lightness + saturation - lightness * saturation;
		auto p = 2 * lightness - q;
		r = hue2rgb(p, q, hue + 1 / 3.0);
		g = hue2rgb(p, q, hue);
		b = hue2rgb(p, q, hue - 1 / 3.0);
	}

	color.red = static_cast<uint8_t>(r * 255);
	color.green = static_cast<uint8_t>(g * 255);
	color.blue = static_cast<uint8_t>(b * 255);
}

/*
int getLuninosity(){
  return MapLuminosity;}
*/