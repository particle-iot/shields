#define SLCD_INIT	0xA3
#define SLCD_INIT_ACK	0xA5
#define SLCD_INIT_DONE	0xAA

//WorkingMode Commands or Responses
#define SLCD_CONTROL_HEADER	0x9F
#define SLCD_CHAR_HEADER	0xFE
#define SLCD_CURSOR_HEADER	0xFF
#define SLCD_CURSOR_ACK		0x5A

#define SLCD_RETURN_HOME	0x61
#define SLCD_DISPLAY_OFF	0x63
#define SLCD_DISPLAY_ON		0x64
#define SLCD_CLEAR_DISPLAY	0x65
#define SLCD_CURSOR_OFF		0x66
#define SLCD_CURSOR_ON		0x67
#define SLCD_BLINK_OFF		0x68
#define SLCD_BLINK_ON		0x69
#define SLCD_SCROLL_LEFT	0x6C
#define SLCD_SCROLL_RIGHT	0x72
#define SLCD_NO_AUTO_SCROLL	0x6A
#define SLCD_AUTO_SCROLL	0x6D
#define SLCD_LEFT_TO_RIGHT	0x70
#define SLCD_RIGHT_TO_LEFT	0x71
#define SLCD_POWER_ON    	0x83
#define SLCD_POWER_OFF  	0x82
#define SLCD_INVALIDCOMMAND	0x46
#define SLCD_BACKLIGHT_ON	0x81
#define SLCD_BACKLIGHT_OFF	0x80
 
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  
  delay(5000);
  
  Serial.println("Starting the set-up process");
  delay(10);
  Serial1.write(SLCD_CONTROL_HEADER);   
  Serial1.write(SLCD_POWER_OFF); 
  delay(10);
  Serial1.write(SLCD_CONTROL_HEADER);   
  Serial1.write(SLCD_POWER_ON);
  delay(10);
  Serial1.write(SLCD_INIT_ACK);
  while(1)
  {
      if (Serial1.available() > 0 && Serial1.read()==SLCD_INIT_DONE) {
          Serial.println("It's talking!");
          break;
      }
  }
  delay(2);
  display("hello, world!");
  Serial.println("Hello, world!");
}
 
void loop() {
  if(Serial1.available()) {
    char c = Serial1.read();
    Serial.println("It's talking!");
    Serial.print(c);
    delay(100);
  }
}
  
void display(const char b[])
{
    Serial1.write(SLCD_CHAR_HEADER);
    Serial1.write(b);
}
