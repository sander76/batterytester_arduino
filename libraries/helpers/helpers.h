void send_identity(char name[], char version[])
{
    Serial.print("{i:n:");
    Serial.print(name);
    Serial.print(":v:");
    Serial.print(version);
    Serial.println("}");
}
