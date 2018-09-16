

#include <hidboot.h>
#include <usbhub.h>


int x; // mouse x
int y; // mouse y

static int xspeed = 2;
static int yspeed = 2;

static int pin_xleft = 13;  //HQPulse - Amouse Pin4
static int pin_xright = 12; //HPulse - Amouse Pin2
static int pin_yup = 11; //VQpulse - Amouse Pin3
static int pin_ydown = 10; //Vpulse - Amouse Pin1
static int pin_lb = 9; //Amouse Pin6
static int pin_rb = 8; //Amouse Pin9
static int pin_mb = 7; //Amouse Pin8

//Amouse Pinout http://old.pinouts.ru/Inputs/AmigaMouseJoy_pinout.shtml


void send_down() {  
  digitalWrite(pin_ydown, HIGH);
  delay(yspeed);
  digitalWrite(pin_yup, HIGH);
  delay(yspeed);
  digitalWrite(pin_ydown, LOW);
  delay(yspeed);
  digitalWrite(pin_yup, LOW);
  delay(yspeed);
}

void send_up() {
  digitalWrite(pin_yup, HIGH);
  delay(yspeed);
  digitalWrite(pin_ydown, HIGH);
  delay(yspeed);
  digitalWrite(pin_yup, LOW);
  delay(yspeed);
  digitalWrite(pin_ydown, LOW);
  delay(yspeed);
}

void send_left() {
  digitalWrite(pin_xleft, HIGH);
  delay(yspeed);
  digitalWrite(pin_xright, HIGH);
  delay(yspeed);
  digitalWrite(pin_xleft, LOW);
  delay(yspeed);
  digitalWrite(pin_xright, LOW);
  delay(yspeed);
}

void send_right() {
  digitalWrite(pin_xright, HIGH);
  delay(yspeed);
  digitalWrite(pin_xleft, HIGH);
  delay(yspeed);
  digitalWrite(pin_xright, LOW);
  delay(yspeed);
  digitalWrite(pin_xleft, LOW);
  delay(yspeed);
}

class MouseRptParser : public MouseReportParser
{
protected:
void OnMouseMove (MOUSEINFO *mi);
void OnLeftButtonUp (MOUSEINFO *mi);
void OnLeftButtonDown (MOUSEINFO *mi);
void OnRightButtonUp (MOUSEINFO *mi);
void OnRightButtonDown (MOUSEINFO *mi);
void OnMiddleButtonUp (MOUSEINFO *mi);
void OnMiddleButtonDown (MOUSEINFO *mi);
};
void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{

x = mi->dX;
y = mi->dY;

if (x < -1) {
  send_left();
  Serial.print("Delta x= ");
      }

if (x > 1)  {
  send_right();
  Serial.print("Delta x= ");
      }
      
if (y < -1) {
send_up(); 
Serial.print("Delta y= ");
      }

if (y > 1)  {
send_down(); 
Serial.print("Delta y= ");
      }
};


void MouseRptParser::OnLeftButtonUp (MOUSEINFO *mi)
{

Serial.println("L Butt Up");
 digitalWrite(pin_lb, HIGH);

};
void MouseRptParser::OnLeftButtonDown (MOUSEINFO *mi)
{

Serial.println("L Butt Dn");
 digitalWrite(pin_lb, LOW);

};
void MouseRptParser::OnRightButtonUp (MOUSEINFO *mi)
{

Serial.println("R Butt Up");
 digitalWrite(pin_rb, HIGH);

};
void MouseRptParser::OnRightButtonDown (MOUSEINFO *mi)
{

Serial.println("R Butt Dn");
 digitalWrite(pin_rb, LOW);

};
void MouseRptParser::OnMiddleButtonUp (MOUSEINFO *mi)
{

Serial.println("M Butt Up");
 digitalWrite(pin_mb, HIGH);

};
void MouseRptParser::OnMiddleButtonDown (MOUSEINFO *mi)
{

Serial.println("M Butt Dn");
 digitalWrite(pin_mb, LOW);
  
};

USB Usb;
USBHub Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);

uint32_t next_time;

MouseRptParser Prs;

void setup()
{
Serial.begin(57600);

  pinMode(pin_xleft, OUTPUT);
  pinMode(pin_xright, OUTPUT);
  pinMode(pin_yup, OUTPUT);
  pinMode(pin_ydown, OUTPUT);
  pinMode(pin_lb, OUTPUT);
  pinMode(pin_rb, OUTPUT);
  pinMode(pin_mb, OUTPUT);
  
  digitalWrite(pin_lb, HIGH);
  digitalWrite(pin_rb, HIGH);
  digitalWrite(pin_mb, HIGH);
  
  
 
 


#if !defined(__MIPSEL__)
while (!Serial); // Wait for serial port to connect – used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif

Serial.println("Start");

if (Usb.Init() == -1)

Serial.println("OSC did not start.");

delay( 200 );

next_time = millis() + 5000;

HidMouse.SetReportParser(0, &Prs);
}

void loop()
{
Usb.Task();
}
