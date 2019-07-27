# Arduino_Fish_Lamp

Help from these websites:

About OLED
https://randomnerdtutorials.com/esp32-built-in-oled-ssd1306/
https://randomnerdtutorials.com/esp32-ssd1306-oled-display-arduino-ide/

About Wifi and Clock
http://blog.daum.net/_blog/BlogTypeView.do?blogid=0ZPt7&articleno=2683&categoryId=7&regdt=20181013201431


Change these were very important to me in OLED test. 
Wire.begin(5, 4); // add
0X3C, false, false);  // modify

