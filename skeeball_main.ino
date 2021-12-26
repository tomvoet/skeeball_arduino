#include <RGBmatrixPanel.h>
#define CLK  8
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2


#define sensor1 A3
#define sensor2 A4
#define sensor3 A5


int loch[4] = { 0, 50, 100, 100 };

int treffer[4];


int counter = 0;
int highscore = 0;
int score = 0;
int wurfnr = 0;


const int StartPin = 12;
const int HighscorePin = 13;
const int sensor4 = 11;

int StartKnopfStatus = 0;
int HighscoreKnopfStatus = 0;


RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);



#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr

const char highscorestring[] PROGMEM = "HIGHSCORE!";
int16_t    textX                     = matrix.width(),
           textMin                   = sizeof(highscorestring) * -12,
           hue                       = 0;


const char pressstring[] PROGMEM = "Press";
int16_t    textY                 = matrix.height(),
           textMin2              = 8,
           hue2                  = 0;

const char startstring[] PROGMEM = "Start";


int8_t ball[3][4] = {
  {  3,  0,  1,  1 },
  { 17, 15,  1, -1 },
  { 27,  4, -1,  1 }
};

static const uint16_t PROGMEM ballcolor[3] = {
  0x0080,
  0x0002,
  0x1000
};



void setup() {
  Serial.begin(9600);
  pinMode(StartPin, INPUT_PULLUP);
  pinMode(HighscorePin, INPUT_PULLUP);
  pinMode(sensor4, INPUT);
  matrix.fillScreen(0);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setTextSize(1);
}



void loop() {
  StartKnopfStatus = digitalRead(StartPin);
  HighscoreKnopfStatus = digitalRead(HighscorePin);

  if (StartKnopfStatus == LOW)
  {
    counter -= counter;
    wurfnr = 0;
    score = 0;
    for (byte i = 0; i < 4; i++)
    {
      treffer[i] = 0;
    }
    GoAnimation();
    DisplayScore(score);
    CalcBall(wurfnr);
  }

  if (HighscoreKnopfStatus == LOW)
  {
    counter -= counter;
    DisplayHighscore(highscore);
  }

  if ( (!digitalRead(sensor1) || !digitalRead(sensor2) || !digitalRead(sensor3) || !digitalRead(sensor4)) && wurfnr < 4 )
  {
    counter -= counter;
    treffer[wurfnr] = AddScore(loch);
    score += (AddScore(loch) + checkCombo(treffer, wurfnr));
    wurfnr++;
    DisplayScore(score);
    CalcBall(wurfnr);
    if (wurfnr == 4)
    {
      highscore = checkHighscore(highscore, score);
    }
    delay(1000);
  }

  if (counter > 14000)
  {
    if (counter == 30000)
    {
      counter == 14200;
    }

    matrix.fillScreen(0);

    for (byte i = 0; i < 3; i++)
    {

      matrix.fillCircle(ball[i][0], ball[i][1], 5, pgm_read_word(&ballcolor[i]));

      ball[i][0] += ball[i][2];
      ball[i][1] += ball[i][3];

      if ((ball[i][0] == 0) || (ball[i][0] == (matrix.width() - 1)))
      {
        ball[i][2] *= -1;
      }
      if ((ball[i][1] == 0) || (ball[i][1] == (matrix.height() - 1)))
      {
        ball[i][3] *= -1;
      }
    }

    matrix.setTextColor(matrix.ColorHSV(hue2, 255, 255, true));
    matrix.setCursor(2, textY);
    matrix.print(F2(pressstring));
    matrix.setCursor(2 , textY + 16);
    matrix.print(F2(startstring));


    if ((--textY) < textMin) textY = matrix.height();
    hue2 += 7;
    if (hue2 >= 1536) hue2 -= 1536;

    delay(20);
    matrix.swapBuffers(false);
  }
  counter++;
  Serial.println(counter);
}



int checkHighscore(int highscore, int score)
{
  if (score > highscore)
  {
    matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));

    for (int i = 0; i < 330; ++i)
    {
      matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
      matrix.setTextSize(2);
      matrix.setTextColor(matrix.ColorHSV(hue, 255, 255, true));
      matrix.setCursor(textX, 1);
      matrix.print(F2(highscorestring));

      if ((--textX) < textMin) textX = matrix.width();
      hue += 7;
      if (hue >= 1536) hue -= 1536;

      matrix.swapBuffers(false);
    }


    DisplayHighscore(score);
    counter -= counter;

    return score;
  }
  else
  {
    return highscore;
  }
}

int AddScore(int loch[4])
{

  int selektion = !digitalRead(sensor1) + 2 * !digitalRead(sensor2) + 3 * !digitalRead(sensor3) + 4 * !digitalRead(sensor4);


  switch (selektion) {

    case 1:
      return loch[--selektion];

    case 2:
      return loch[--selektion];

    case 3:
      return loch[--selektion];

    case 4:
      return loch[--selektion];

    default:
    
      break;

  }
}

void GoAnimation()
{
  for (int i = 0; i < 9; ++i)
  {

    matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
    matrix.setTextSize(1);
    matrix.setCursor(8, 4);
    matrix.setTextColor(matrix.Color333(9, 9, 9));
    matrix.print("Los");

    matrix.drawCircle(4, 3, i % 3, matrix.Color333(9, 9, 9));
    matrix.drawCircle(4, 12, i % 3, matrix.Color333(9, 9, 9));
    matrix.drawCircle(27, 3, i % 3, matrix.Color333(9, 9, 9));
    matrix.drawCircle(27, 12, i % 3, matrix.Color333(9, 9, 9));
    matrix.drawCircle(4, 3, (i % 3) + 1, matrix.Color333(9, 9, 9));
    matrix.drawCircle(4, 12, (i % 3) + 1, matrix.Color333(9, 9, 9));
    matrix.drawCircle(27, 3, (i % 3) + 1, matrix.Color333(9, 9, 9));
    matrix.drawCircle(27, 12, (i % 3) + 1, matrix.Color333(9, 9, 9));
    delay(133);

  }
}


void CalcBall(int wurfnr)
{

  switch (wurfnr)
  {

    case 0:
      matrix.fillRect(22, 8, 3, 3, matrix.Color333(5, 4, 5));
      matrix.fillRect(27, 8, 3, 3, matrix.Color333(5, 4, 5));
      matrix.fillRect(22, 12, 3, 3, matrix.Color333(5, 4, 5));
      matrix.fillRect(27, 12, 3, 3, matrix.Color333(5, 4, 5));
      break;

    case 1:
      matrix.fillRect(22, 8, 3, 3, matrix.Color333(5, 4, 5));
      matrix.fillRect(27, 8, 3, 3, matrix.Color333(5, 4, 5));
      matrix.fillRect(22, 12, 3, 3, matrix.Color333(5, 4, 5));
      break;

    case 2:
      matrix.fillRect(22, 8, 3, 3, matrix.Color333(5, 4, 5));
      matrix.fillRect(27, 8, 3, 3, matrix.Color333(5, 4, 5));
      break;

    case 3:
      matrix.fillRect(22, 8, 3, 3, matrix.Color333(5, 4, 5));
      break;

    case 4:

      break;

    default:

      break;

  }
}

int checkCombo(int treffer[4], int wurfnr)
{

  if (wurfnr != 0)
  {
    switch (wurfnr)

    {
      case 3:
        if (treffer[3] == treffer[2] && treffer[1] == treffer[0])
        {
          ComboAnimation();
          return (treffer[3] * 3);
        }
        else if (treffer[3] == treffer[2] && treffer[2] == treffer[1])
        {
          ComboAnimation();
          return (treffer[3] * 2);
        }
        else if (treffer[3] == treffer[2])
        {
          ComboAnimation();
          return (treffer[3]);
        }
        else
        {
          return 0;
        }
      case 2:
        if (treffer[2] == treffer[0] && treffer[1] == treffer[0])
        {
          ComboAnimation();
          return (treffer[2] * 2);
        }
        else if (treffer[2] == treffer[1])
        {
          ComboAnimation();
          return (treffer[2]);
        }
        else
        {
          return 0;
        }
      case 1:
        if (treffer[1] == treffer[0])
        {
          ComboAnimation();
          return (treffer[1]);
        }
        else return 0;
      default:
        return 0;

    }
  }
  else
  {
    return 0;
  }
}

void ComboAnimation()
{
  const char combostring[] PROGMEM     = "Combo Bonus!";
  int16_t    textX                     = matrix.width(),
             textMin                   = sizeof(combostring) * -12,
             hue                       = rand() % 1536;
  for (int i = 0; i < 100; i++)
  {
    matrix.fillScreen(0);
    matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
    matrix.setTextSize(1);
    matrix.setTextColor(matrix.ColorHSV(hue, 255, 255, true));
    matrix.setCursor(textX, 4);
    matrix.print(combostring);

    if ((--textX) < textMin) textX = matrix.width();
    hue += 7;
    if (hue >= 1536) hue -= 1536;

    matrix.swapBuffers(false);
    
  }
}

void DisplayHighscore(int highscore)
{

  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
  matrix.setTextSize(1);
  matrix.setCursor(1, 0);
  matrix.setTextColor(matrix.Color333(9, 0, 0));
  matrix.print("Best:");
  matrix.setCursor(1, 9);
  matrix.setTextColor(matrix.Color333(0, 7, 7));
  matrix.print(highscore);

}

void DisplayScore(int score)
{

  matrix.fillRect(0, 0, 32, 16, matrix.Color333(0, 0, 0));
  matrix.setTextSize(1);
  matrix.setCursor(1, 0);
  matrix.setTextColor(matrix.Color333(9, 0, 0));
  matrix.print("Score");
  matrix.setCursor(1, 9);
  matrix.setTextColor(matrix.Color333(0, 7, 7));
  matrix.print(score);

}
