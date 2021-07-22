#include "BtConnection.h"

/*
    BT module connection:
    - pin D5(GPIO14) <=> TXD
    - pin D6(GPIO12) <=> RXD

*/
SoftwareSerial btChannel(14, 12);

BtConnection::BtConnection() {
  Serial.println("Setting Bluetooth...");
  while (!Serial) {};

  btChannel.begin(9600);

  /* check if it is there */
  btChannel.print("AT");
  delay(1000);

  /* set the name */
  btChannel.print("AT+NAMEgioIOT"); // Set the name to isiXX
  delay(1000);

  /* get the version */
  // btChannel.print("AT+VERSION");
  // delay(1000);

  /* get the name */
  // btChannel.print("AT+NAME");
  // delay(1000);

  /* get the name */
  // btChannel.print("AT+ADDR");
  // delay(1000);

  Serial.println("BT Ready!");

}

void BtConnection::sendStatus(String str) {
  /* reading data from BT to Serial
    if (btChannel.available())
    Serial.write(btChannel.read());*/

  /* reading data from the Serial to BT */
  btChannel.println(str);
}



String BtConnection::receiveStatus() {
   return btChannel.readString();
}
