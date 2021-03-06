
const int SOFT_POT_PIN_1 = A0; // Pin connected to softpot
const int SOFT_POT_PIN_2 = A1;
const int SOFT_POT_PIN_3 = A2;

const int TOL = 35; //TODO: Verify this tolerance value

int softPotString_1[] = {960, 750, 570, 440, 0, 0};
int softPotString_2[] = {940, 730, 0, 400, 730, 0};
int softPotString_3[] = {0, 730, 0, 0, 0, 0};

int chordName = -1;

String SCREEN_FEEDBACK;

const int G_CHORD = 0;
const int C_CHORD = 1;
const int D_CHORD = 2;
const int F_CHORD = 3;
const int Am_CHORD = 4;
const int Em_CHORD = 5;

void setup()
{
  Serial.begin(9600);
  pinMode(SOFT_POT_PIN_1, INPUT);
  pinMode(SOFT_POT_PIN_2, INPUT);
  pinMode(SOFT_POT_PIN_3, INPUT);

  //calibrate();
}

void loop()
{
  chordName = 1;
  if (chordName != -1)
  {
    getSensorFeedback(chordName);
  }
}

void sensorSetup()
{
  pinMode(SOFT_POT_PIN_1, INPUT);
  pinMode(SOFT_POT_PIN_2, INPUT);
  pinMode(SOFT_POT_PIN_3, INPUT);
}

void calibrate()
{
  calibrateFirstSensor();
  calibrateSecondSensor();
  calibrateThirdSensor();
}

void calibrateFirstSensor()
{
  int softPotADC;

  for (int i = 0; i < 6; i++)
  {
    softPotADC = 0;
    while (softPotADC == 0)
    {
      softPotADC = analogRead(SOFT_POT_PIN_1);
    }
    softPotString_1[i] = softPotADC;
    delay(1000);
  }
  Serial.print("Done calibrating 1");
}

void calibrateSecondSensor()
{
  int softPotADC;

  for (int i = 0; i < 6; i++)
  {
    softPotADC = 0;
    while (softPotADC == 0)
    {
      softPotADC = analogRead(SOFT_POT_PIN_2);
    }
    softPotString_2[i] = softPotADC;
    delay(1000);
  }
  Serial.print("Done calibrating 2");
}

void calibrateThirdSensor()
{
  int softPotADC;

  for (int i = 0; i < 6; i++)
  {
    softPotADC = 0;
    while (softPotADC == 0)
    {
      softPotADC = analogRead(SOFT_POT_PIN_3);
    }
    softPotString_3[i] = softPotADC;
    delay(1000);
  }
  Serial.print("Done calibrating 3");
}

bool getSensorFeedback(int expectedChord)
{
  int softPotADC[] = {
    analogRead(SOFT_POT_PIN_1), analogRead(SOFT_POT_PIN_2), analogRead(SOFT_POT_PIN_3)
  };

  int softPotADC1 = analogRead(SOFT_POT_PIN_1);
  int softPotADC2 = analogRead(SOFT_POT_PIN_2);
  int softPotADC3 = analogRead(SOFT_POT_PIN_3);
  Serial.println(softPotADC3);
  delay(100);
  bool success = evaluateChord(expectedChord, softPotADC1, softPotADC2, softPotADC3);
  /*
  if(success)
  {
    delay(100);
    success = evaluateChord(expectedChord, softPotADC[0], softPotADC[1], softPotADC[2]);
  }
  */

  if(success)
  {
    SCREEN_FEEDBACK = "Good Job";
    Serial.println("Good Job");
    return true;
  }
  else
  {
    findError(expectedChord, softPotADC1, softPotADC2, softPotADC3);
    return false;
  }
}

bool evaluateChord(int expectedChord, int sensorValue1, int sensorValue2, int sensorValue3)
{
  switch (expectedChord) {
    // G chord
    case G_CHORD:
      if (abs(sensorValue1) == 0 && abs(sensorValue2 - softPotString_2[4]) < TOL && abs(sensorValue3 - ((softPotString_3[0] + softPotString_3[5]) / 2)) < TOL)
      {
        return true;
      }
      return false;
    // C chord
    //TODO
    case C_CHORD:
      Serial.println(sensorValue3);
      if (abs(sensorValue1 - softPotString_1[1]) < TOL && abs(sensorValue2 - softPotString_2[1]) < TOL && abs(sensorValue3 - softPotString_3[1]) < TOL)
        return true;
      return false;
    // D chord
    case D_CHORD:
      if (abs(sensorValue1) == 0 && abs(sensorValue2 - ((softPotString_2[0] + softPotString_2[2]) / 2)) < TOL && abs(sensorValue3 - softPotString_3[1]) < TOL)
        return true;
      return false;
    // F chord
    case F_CHORD:
      if (abs(sensorValue1 - softPotString_1[1]) < TOL && abs(sensorValue2 - softPotString_2[2]) < TOL && abs(sensorValue3 - ( (softPotString_3[3] + softPotString_3[4]) / 2)) < TOL)
        return true;
      return false;
    // Am Chord
    case Am_CHORD:
      if (abs(sensorValue1 - softPotString_1[1]) < TOL && abs(sensorValue2 - ((softPotString_2[2] + softPotString_2[3]) / 2)) < TOL && abs(sensorValue3) == 0)
        return true;
      return false;
    // Em chord
    case Em_CHORD:
      if (abs(sensorValue1) == 0 && abs(sensorValue2 - ((softPotString_2[3] + softPotString_2[4]) / 2)) < TOL && abs(sensorValue3) == 0)
        return true;
      return false;
    default:
      return false;
  }
}

void findError(int expectedChord, int sensorValue1, int sensorValue2, int sensorValue3)
{
  // Go through every case for each possible chord
  // Three cases:
  // 1. User has not placed fingers on any strings
  // 2. User has placed fingers on some of the correct notes
  // 3. User has placed fingers on incorrect notes
  switch(expectedChord) {
    case G_CHORD:
      if (abs(sensorValue1) != 0)
      {
        SCREEN_FEEDBACK = "Check the first fret";
      }
      if(abs(sensorValue2 - softPotString_2[4]) > TOL)
      {
        SCREEN_FEEDBACK = SCREEN_FEEDBACK + "Check the second fret";
      }
      if(abs(sensorValue3 - ((softPotString_3[0] + softPotString_3[5]) / 2)) > TOL)
      {
        SCREEN_FEEDBACK = SCREEN_FEEDBACK + "Check the third fret";
      }
      break;
    // C chord
    //TODO
    case C_CHORD:
      if (abs(sensorValue1 - softPotString_1[1]) > TOL)
      {
        SCREEN_FEEDBACK = "Check the first fret";
      }
      if(abs(sensorValue2 - softPotString_2[1]) > TOL)
      {
        SCREEN_FEEDBACK = SCREEN_FEEDBACK + "Check the second fret";
      }
      if(abs(sensorValue3 - softPotString_3[1]) > TOL)
      {
        SCREEN_FEEDBACK = SCREEN_FEEDBACK + "Check the third fret";
        //Serial.println(softPotString_3[1]);
        //Serial.println(sensorValue3);
      }
      break;
    // D chord
    case D_CHORD:
      if (abs(sensorValue1) != 0)
      {
        SCREEN_FEEDBACK = "Check the first fret";
      }
      if(abs(sensorValue2 - ((softPotString_2[0] + softPotString_2[2]) / 2)) > TOL)
      {
        SCREEN_FEEDBACK = SCREEN_FEEDBACK + "Check the second fret";
      }
      if(abs(sensorValue3 - softPotString_3[1]) > TOL)
      {
        SCREEN_FEEDBACK = SCREEN_FEEDBACK + "Check the third fret";
      }
      break;
    // F chord
    case F_CHORD:
      if (abs(sensorValue1 - softPotString_1[1]) > TOL)
      {
        SCREEN_FEEDBACK = "Check the first fret";
      }
      if(abs(sensorValue2 - softPotString_2[2]) > TOL)
      {
        SCREEN_FEEDBACK = SCREEN_FEEDBACK + "Check the second fret";
      }
      if(abs(sensorValue3 - ( (softPotString_3[3] + softPotString_3[4]) / 2)) > TOL)
      {
        SCREEN_FEEDBACK = SCREEN_FEEDBACK + "Check the third fret";
      }
      break;
    // Am Chord
    case Am_CHORD:
      if (abs(sensorValue1 - softPotString_1[1]) > TOL)
      {
        SCREEN_FEEDBACK = "Check the first fret";
      }
      if(abs(sensorValue2 - ((softPotString_2[2] + softPotString_2[3]) / 2)) > TOL)
      {
        SCREEN_FEEDBACK = SCREEN_FEEDBACK + "Check the second fret";
      }
      if(abs(sensorValue3) != 0)
      {
        SCREEN_FEEDBACK = SCREEN_FEEDBACK + "Check the third fret";
      }
      break;
    // Em chord
    case Em_CHORD:
      if (abs(sensorValue1) != 0)
      {
        SCREEN_FEEDBACK = "Check the first fret";
      }
      if(abs(sensorValue2 - ((softPotString_2[3] + softPotString_2[4]) / 2)) > TOL)
      {
        SCREEN_FEEDBACK = SCREEN_FEEDBACK + "Check the second fret";
      }
      if(abs(sensorValue3) != 0)
      {
        SCREEN_FEEDBACK = SCREEN_FEEDBACK + "Check the third fret";
      }
      break;
  }
  Serial.println(SCREEN_FEEDBACK);
}

