#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(
  U8G2_R0,
  U8X8_PIN_NONE
);

const int PIN_BOTON_IZQ = 18;
const int PIN_BOTON_DER = 19;
const int PIN_BUZZER = 5;

bool estado_ant_izq = HIGH;
bool estado_ant_der = HIGH;

unsigned long ultimoGolpe = 0;

void dibujarBongoCat(int x, int y, bool izq, bool der)
{
  // =====================
  // MESA
  // =====================
  u8g2.drawLine(0, 23, 127, 23);

  // =====================
  // CABEZA
  // =====================
  u8g2.drawEllipse(x, y, 18, 10);

  // Orejas
  u8g2.drawLine(x - 11, y - 7, x - 17, y - 13);
  u8g2.drawLine(x - 17, y - 13, x - 6, y - 10);

  u8g2.drawLine(x + 11, y - 7, x + 17, y - 13);
  u8g2.drawLine(x + 17, y - 13, x + 6, y - 10);

  // Ojos
  u8g2.drawDisc(x - 5, y - 1, 1);
  u8g2.drawDisc(x + 5, y - 1, 1);

  // Cachetes
  u8g2.drawPixel(x - 9, y + 2);
  u8g2.drawPixel(x + 9, y + 2);

  // Nariz
  u8g2.drawPixel(x, y + 1);

  // Boca kawaii
  u8g2.drawLine(x - 2, y + 3, x, y + 5);
  u8g2.drawLine(x, y + 5, x + 2, y + 3);

  // Bigotes
  u8g2.drawLine(x - 12, y + 1, x - 17, y);
  u8g2.drawLine(x - 12, y + 3, x - 17, y + 4);

  u8g2.drawLine(x + 12, y + 1, x + 17, y);
  u8g2.drawLine(x + 12, y + 3, x + 17, y + 4);

  // =====================
  // PATA IZQUIERDA
  // =====================
  if (izq)
  {
    // Golpeando mesa
    u8g2.drawBox(x - 16, 17, 6, 6);
  }
  else
  {
    // Debajo de mesa
    u8g2.drawBox(x - 16, 25, 6, 5);
  }

  // =====================
  // PATA DERECHA
  // =====================
  if (der)
  {
    // Golpeando mesa
    u8g2.drawBox(x + 10, 17, 6, 6);
  }
  else
  {
    // Debajo de mesa
    u8g2.drawBox(x + 10, 25, 6, 5);
  }
}

void setup()
{
  pinMode(PIN_BOTON_IZQ, INPUT_PULLUP);
  pinMode(PIN_BOTON_DER, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);

  Wire.begin(21, 22);

  u8g2.begin();
}

void loop()
{
  bool b_izq =
    (digitalRead(PIN_BOTON_IZQ) == LOW);

  bool b_der =
    (digitalRead(PIN_BOTON_DER) == LOW);

  // Sonido botón izquierdo
  if (b_izq && estado_ant_izq == HIGH)
  {
    tone(PIN_BUZZER, 587, 35);
    ultimoGolpe = millis();
  }

  // Sonido botón derecho
  if (b_der && estado_ant_der == HIGH)
  {
    tone(PIN_BUZZER, 659, 35);
    ultimoGolpe = millis();
  }

  estado_ant_izq =
    digitalRead(PIN_BOTON_IZQ);

  estado_ant_der =
    digitalRead(PIN_BOTON_DER);

  // Rebote de cabeza
  bool rebotando =
    (millis() - ultimoGolpe < 80);

  int yCabeza = 14;

  if (rebotando)
  {
    yCabeza = 16;
  }

  u8g2.clearBuffer();

  dibujarBongoCat(
    64,
    yCabeza,
    b_izq,
    b_der
  );

  u8g2.sendBuffer();

  delay(10);
}
