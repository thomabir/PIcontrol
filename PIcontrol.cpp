#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

#include "AutoZeroSample.h"

class Actuator {
 public:
  Actuator(char* szDescription);
  ~Actuator();
  void init();
  double read();
  void move(double value);
  void close();

 private:
  int iD;
  char szDescription[1024];
};

Actuator::Actuator(char* szDescription) {
  std::strcpy(this->szDescription, szDescription);
}

Actuator::~Actuator() { close(); }

void Actuator::init() {
  std::cout << "Connecting to " << this->szDescription << std::endl;
  iD = PI_ConnectUSB(this->szDescription);

  // identify controller
  char szIDN[200];
  PI_qIDN(iD, szIDN, 199);
  std::cout << "qIDN returned: " << szIDN << std::endl;

  // find available axes
  char szAxes[1024];
  PI_qSAI(iD, szAxes, 1024);
  std::cout << "Available axes: " << szAxes << std::endl;

  // find units that qPOS and MOV use
  char szUnits[1024];
  PI_qPUN(iD, "2", szUnits, 1024);
  std::cout << "Units: " << szUnits << std::endl;

  // enable servo
  const int iEnable = 1;
  PI_SVO(iD, "2", &iEnable);

  // query servo status and print
  int iServoStatus = 0;
  PI_qSVO(iD, "2", &iServoStatus);
  std::cout << "Servo status: " << iServoStatus << std::endl;
}

double Actuator::read() {
  double value = 0;
  PI_qPOS(iD, "2", &value);
  return value;
}

void Actuator::move(double value) {
  const double dValue = value;
  PI_MOV(iD, "2", &dValue);
}

void Actuator::close() { PI_CloseConnection(iD); }

int main() {
  // connect to actuator
  // char szDescription[1024] = "PI E-727 Controller SN 0122042007";
  char szDescription[1024] = "PI E-727 Controller SN 0122040101";
  Actuator tiptilt1(szDescription);
  tiptilt1.init();

  // read actuator value
  double value = tiptilt1.read();
  std::cout << "Actuator value: " << value << std::endl;

  // move actuator
  tiptilt1.move(10.);

  // wait 10 ms
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // read actuator value
  value = tiptilt1.read();
  std::cout << "Actuator value: " << value << std::endl;

  // close connection
  tiptilt1.close();

  return 0;
}