struct SensorType
{
    const char name[15]; const byte type; const byte rev;
};

const SensorType led_gate = {"Led gate", 1, 1};

const SensorType volt_amp_sensor = {"VoltsAmps", 2, 1};

const SensorType relay_switch = {"RelaySwitch", 3, 1};


void Blink(int pin,SensorType sensor_type)
{
    digitalWrite(pin, HIGH);
    delay(4000);
    digitalWrite(pin, LOW);
    delay(1000);
    for (int i = 0; i < sensor_type.type; i++)
    {
        digitalWrite(pin, HIGH);
        delay(500);
        digitalWrite(pin, LOW);
        delay(500);
    }
    delay(2000);
    for (int i = 0; i < sensor_type.rev; i++)
    {
        digitalWrite(pin, HIGH);
        delay(500);
        digitalWrite(pin, LOW);
        delay(500);
    }
    delay(2000);
    digitalWrite(pin,HIGH);
    delay(4000);
    digitalWrite(pin,LOW);
    delay(500);
}

