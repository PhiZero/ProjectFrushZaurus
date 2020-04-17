#include <game_manager.h>

void gameMode::setStrip(Adafruit_NeoPixel *strip)
{
  stripGame = strip;
  stripGame->setBrightness(LED_BRIGHTNESS);

  accelerator.initialize();
  base = accelerator.getAccelerationZ();
}

void gameMode::startup()
{
  stripGame->clear();
  stripGame->show();
  for (int strip_num = 2; strip_num >= 0; strip_num--)
  {
    for (int i = 0; i < NUM_PIXELS; i++)
      stripGame->setPixelColor(strip_num * NUM_PIXELS + i, stripGame->Color(BASE_COLOR));
    showReset(1000);
  }
}

void gameMode::showReset(int wait)
{
  stripGame->show();
  delay(wait);
  stripGame->clear();
  stripGame->show();
}

void gameMode::judge(int wait, bool us, int stripNum)
{
  int judgeTimes = wait / 10;
  stripGame->show();
  receive = accelerator.getAccelerationZ();
  for(int i = 0; i < judgeTimes; i++)
  {
    if(abs(abs(base) - abs(receive)) >= 2500)
    {
      judgeDaigo = true;
      if(stripNum >= 25 && stripNum <= 26)
       sucess();
      else
       failure(stripNum);
     return;
    }
    if (wait <= 10)
    {
      if(us)
      {
        delayMicroseconds(wait);
      }else{
        delay(wait);
      }
      stripGame->clear();
      stripGame->show();
      break;
    }
    if(us)
    {
      delayMicroseconds(wait / 2);
      if(i == 1)  break;
    }else{
      delay(10);
    }
  }
  stripGame->clear();
  stripGame->show();
}

void gameMode::previw(int speed)
{
  startup();
  for (int strip_num = 0; strip_num < NUM_STRIP; strip_num++)
  {
    for (int i = 0; i < NUM_PIXELS; i++)
      stripGame->setPixelColor(strip_num * NUM_PIXELS + i, stripGame->Color(BASE_COLOR));
    showReset(speed);
  }
}

void gameMode::constantVelocity(int speed)
{
  constantVelocity(speed, false);
}

void gameMode::constantVelocity(int speed, bool us)
{
  startup();
  for (int strip_num = 0; strip_num < NUM_STRIP; strip_num++)
  {
    for (int i = 0; i < NUM_PIXELS; i++)
      stripGame->setPixelColor(strip_num * NUM_PIXELS + i, stripGame->Color(BASE_COLOR));
    judge(speed, us, strip_num);
    if(judgeDaigo)
    {
      judgeDaigo = false;
      return;
    }
  }
}

int gameMode::randomSpeed()
{
  int x = random(1, 100);
  int func = (int)pow(x, 2) + random(10, 50) * x + random(10, 200);
  return func;
}

void gameMode::delayTime(int time)
{
  int delayTimes = time / 10;
  for(int i = 0; i < delayTimes; i++)
  {
    val = digitalRead(INTERRUPT_PIN);
    if(val == HIGH)
      break;
    delay(10);
  }
}

void gameMode::acceleration()
{
  startup();
  for (int strip_num = 0; strip_num < NUM_STRIP; strip_num++)
  {
    for (int i = 0; i < NUM_PIXELS; i++)
      stripGame->setPixelColor(strip_num * NUM_PIXELS + i, stripGame->Color(BASE_COLOR));
    judge(randomSpeed(), true, strip_num);
    if(judgeDaigo)
    {
      judgeDaigo = false;
      return;
    }
  }
}

void gameMode::sucess()
{
  for (int l = 0; l < 5; l++)
  {
    for (int i = 0; i < ALL_PIXELS; i++)
      stripGame->setPixelColor(i, stripGame->Color(SUCESS_COLOR));
    showReset(1000);
    delay(500);
  }
}

void gameMode::failure(int strip_num)
{
  for (int l = 0; l < 5; l++)
  {
    for (int i = 0; i < NUM_PIXELS; i++)
      stripGame->setPixelColor(strip_num * NUM_PIXELS + i, stripGame->Color(FAILURE_COLOR));
    showReset(1000);
    delay(500);
  }
}

void gameMode::rainbowStrip()
{
  rainbowStrip(10, 1);
}

void gameMode::rainbowStrip(int time)
{
  rainbowStrip(time, 1);
}

void gameMode::rainbowStrip(int time, int multiple)
{
  delayTime(10);
  if(val == HIGH)
    return;
  for (long firstPixelHue = 0; firstPixelHue < HUE_CIRCLE; firstPixelHue += 256 * multiple)
  {
    for (uint16_t i = 0; i < ALL_PIXELS; i += NUM_PIXELS)
    {
      int pixelHue = firstPixelHue + (i * HUE_CIRCLE / ALL_PIXELS);
      for (uint16_t l = i; l < i + NUM_PIXELS; l++)
        stripGame->setPixelColor(l, stripGame->gamma32(stripGame->ColorHSV(pixelHue)));
    }
    stripGame->show();
    delayTime(time);
    if(val == HIGH)
      return;
    Serial.println("now rainbow");
  }
}
