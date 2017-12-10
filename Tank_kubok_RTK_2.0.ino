#include <PS2X_lib.h>
#include <iarduino_MultiServo.h>     //  Подключаем библиотеку
iarduino_MultiServo MSS;   
#define PS2_DAT    13 
#define PS2_CMD    9
#define PS2_SEL    3
#define PS2_CLK    A2
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false


PS2X ps2x;

int speed = 255;

int error = 0;
byte type = 0;
byte vibrate = 0;

int enA_LF = 5;
int in1_LF = 7;
int in2_LF = 2;


int enB_RF = 6;
int in3_RF = 4;
int in4_RF = 8;


void setup(){
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
 // MSS.servoSet(15, SERVO_SG90);     //  Авто   позиционирование сервопривода (Подключён к выводу 15. Название сервопривода)                 //  Если ко всем выводам подключены одинаковые сервоприводы, то допускается вызвать функцию servoSet() один раз, указав вместо номера вывода: SERVO                    //  - Пример_2: MSS.servoSet(SERVO_ALL,180,140,480); // Ко всем выводам подключены одинаковые сервоприводы, параметры которых заданы вручную
 // MSS.begin();        
Serial.begin(57600);
  
  delay(300);  //добавляем паузу, чтобы дать беспроводному ps2 модулю время для включения
   
  //ТУТ ИЗМЕНЕНИЯ ДЛЯ ВЕРСИИ v1.6!!! **************БУДЬТЕ ВНИМАТЕЛЬНЫ*************
  
  //установка пинов и настроек: GamePad(clock, command, attention, data, Pressures?, Rumble?) проверка ошибок
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    
    // Сонтроллера найден, всё настроено удачно
    Serial.print("Found Controller, configured successful ");
    
    Serial.print("pressures = ");
  if (pressures)
    Serial.println("true ");
  else
    Serial.println("false");
  Serial.print("rumble = ");
  if (rumble)
    Serial.println("true)");
  else
    Serial.println("false");

    // Попробуйте все кнопки, при нажатии X контроллера будет вибрировать, чем дольше нажатие, тем быстрее вибрация
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    
    // Зажмите L1 или R1, будут отображаться показания аналоговых стикеров
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    
    // Дополнительно: Зайдите на www.billporter.info узнайте об обновлениях или сообщите об ошибке
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    // Контроллер не найден, проверьте провода, посмотрите readme.txt для включения функции выявления ошибок. Посетите www.billporter.info и узнайте об известных проблемах
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    // Контроллер найден, но не реагирует на команды. Посмотрите readme.txt для включения функции выявления ошибок. Посетите www.billporter.info и узнайте об известных проблемах
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    // Контроллер не принимает режим Pressures, возможно он им не поддерживается.
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
//  Serial.print(ps2x.Analog(1), HEX);
  
  type = ps2x.readType(); 
  switch(type) {
    case 0:
      // Найден неизвестный тип контроллера
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      //Найден DualShock контроллер
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      //Найден DualShock контроллер
      Serial.print("GuitarHero Controller found ");
      break;
  case 3:
      // Найден Беспроводной Sony DualShock контроллер
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
   }
pinMode(enA_LF, OUTPUT);
pinMode(enB_RF, OUTPUT);

pinMode(in1_LF, OUTPUT);
pinMode(in2_LF, OUTPUT);

pinMode(in3_RF, OUTPUT);
pinMode(in4_RF, OUTPUT);



}
void move_backward()
{
digitalWrite(in1_LF, HIGH);
digitalWrite(in2_LF, LOW);

digitalWrite(in3_RF, HIGH);
digitalWrite(in4_RF, LOW);


analogWrite(enA_LF,speed );
analogWrite(enB_RF, speed);

}
void move_laser_on()
{
  digitalWrite(11, HIGH);
  digitalWrite(10, HIGH);
}
void move_laser_off()
{
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
}
void move_forward()
{
digitalWrite(in1_LF, LOW);
digitalWrite(in2_LF, HIGH);

digitalWrite(in3_RF, LOW);
digitalWrite(in4_RF, HIGH);


analogWrite(enA_LF, speed);
analogWrite(enB_RF, speed);

}

void move_right()
{
digitalWrite(in1_LF, LOW);
digitalWrite(in2_LF, HIGH);

digitalWrite(in3_RF, HIGH);
digitalWrite(in4_RF, LOW);

analogWrite(enA_LF, speed);
analogWrite(enB_RF, speed);

}

void move_left()
{
digitalWrite(in1_LF, HIGH);
digitalWrite(in2_LF, LOW);

digitalWrite(in3_RF, LOW);
digitalWrite(in4_RF, HIGH);


analogWrite(enA_LF, speed);
analogWrite(enB_RF, speed);

}

void move_stop()
{
digitalWrite(in1_LF, LOW);
digitalWrite(in2_LF, LOW);

digitalWrite(in3_RF, LOW);
digitalWrite(in4_RF, LOW);


analogWrite(enA_LF, 0);
analogWrite(enB_RF, 0);

}

void loop() {
   //MSS.servoWrite(15,50);             //  Повернуть сервопривод, подключённый к 15 выводу, на 0°
  // delay(3000);    
  if(error == 1) //Цикл «loop» пропускается если джойстик не найден
    return; 
  
  if(type == 2){ //Guitar Hero контроллер
    ps2x.read_gamepad();          //считывание данных с контроллера 
   
    if(ps2x.ButtonPressed(GREEN_FRET))
      Serial.println("Green Fret Pressed");
    if(ps2x.ButtonPressed(RED_FRET))
      Serial.println("Red Fret Pressed");
    if(ps2x.ButtonPressed(YELLOW_FRET))
      Serial.println("Yellow Fret Pressed");
    if(ps2x.ButtonPressed(BLUE_FRET))
      Serial.println("Blue Fret Pressed");
    if(ps2x.ButtonPressed(ORANGE_FRET))
      Serial.println("Orange Fret Pressed"); 

    if(ps2x.ButtonPressed(STAR_POWER))
      Serial.println("Star Power Command");
    
    if(ps2x.Button(UP_STRUM))          //will be TRUE as long as button is pressed
      Serial.println("Up Strum");
    if(ps2x.Button(DOWN_STRUM))
      Serial.println("DOWN Strum");
 
    if(ps2x.Button(PSB_START))         //will be TRUE as long as button is pressed
      Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held");
    
    if(ps2x.Button(ORANGE_FRET)) {     // print stick value IF TRUE
      Serial.print("Wammy Bar Position:");
      Serial.println(ps2x.Analog(WHAMMY_BAR), DEC); 
    } 
  }
  else { //DualShock контроллер
    ps2x.read_gamepad(false, vibrate); //считывание данных с джойстика и установка скорости вибрации
    
    if(ps2x.Button(PSB_START))         //будет TRUE пока кнопка нажата
      Serial.println("Start is being held"); // Start нажат
    if(ps2x.Button(PSB_SELECT))
      Serial.println("Select is being held"); // Select нажат

    if(ps2x.Button(PSB_TRIANGLE)) {      //будет TRUE пока кнопка нажата
      Serial.print("Up held this hard: "); // ВВЕРХ нажато, сила нажатия:
      Serial.println(ps2x.Analog(PSAB_TRIANGLE), DEC);
      move_forward();
    }
    else {
      move_stop();
    }
    if(ps2x.Button(PSB_CROSS)) {      //будет TRUE пока кнопка нажата
      Serial.print("Up held this hard: "); // ВВЕРХ нажато, сила нажатия:
      Serial.println(ps2x.Analog(PSAB_CROSS), DEC);
      move_backward();
    }
    if(ps2x.Button(PSB_PAD_RIGHT)){
      Serial.print("Right held this hard: "); // ВПРАВО нажато, сила нажатия:
      Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
      move_right();
    }
    if(ps2x.Button(PSB_PAD_LEFT)){
      Serial.print("LEFT held this hard: "); // ВЛЕВО нажато, сила нажатия:
      Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
      move_left();
    }
  if(ps2x.Button(PSB_CIRCLE)){
      Serial.print("LASER ON "); // ВНИЗ нажато, сила нажатия:
      Serial.println(ps2x.Analog(PSAB_CIRCLE), DEC);
      move_laser_on();
    }     
    else {
      move_laser_off();
    }

    vibrate = ps2x.Analog(PSAB_CROSS);  //Скорость вибрации устанавливаеться в зависимости от силы нажатия кнопки (X)
    if (ps2x.NewButtonState()) {        //будет TRUE если какая то кнопка изменила свой статус (Вкл. на Выкл. или Выкл. на Вкл.)
      if(ps2x.Button(PSB_L3))
        Serial.println("L3 pressed"); // L3 нажата
      if(ps2x.Button(PSB_R3))
        Serial.println("R3 pressed"); // R3 нажата
      if(ps2x.Button(PSB_L2))
        Serial.println("L2 pressed"); // L2 нажата
      if(ps2x.Button(PSB_R2))
        Serial.println("R2 pressed"); // R2 нажата
      if(ps2x.Button(PSB_TRIANGLE)) 
        Serial.println("Triangle pressed"); // Треугольник нажат 
    }

    else {
    }
    if(ps2x.NewButtonState(PSB_CROSS)) //будет TRUE если кнопка в НАЖАТОМ СОСТОЯНИИ нажата ИЛИ отпущена
      Serial.println("X just changed"); // X изменил статус
    if(ps2x.ButtonReleased(PSB_SQUARE)) //будет TRUE если кнопка в ОТПУЩЕНОМ состоянии
      Serial.println("Square just released"); // Квадрат отпущен

    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) { // Отображает показания стикеров
      Serial.print("Stick Values:");
      Serial.print(ps2x.Analog(PSS_LY), DEC); // Ось Y Левого стикера
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_LX), DEC); // Ось X Левого стикера
      Serial.print(",");
      Serial.print(ps2x.Analog(PSS_RY), DEC); // Ось Y Правого стикера
      Serial.print(",");
      Serial.println(ps2x.Analog(PSS_RX), DEC); // Ось X Правого стикера
    }     
  }
  delay(50);  
}
