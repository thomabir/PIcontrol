#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

#include "AutoZeroSample.h"

bool AutoZeroIfNeeded(int ID, const std::string axis) {
  BOOL bAutoZeroed;

  if (!PI_qATZ(ID, axis.c_str(), &bAutoZeroed)) {
    return false;
  }

  if (!bAutoZeroed) {
    // if needed, autozero the axis
    std::cout << "AutoZero axis " << axis << "..." << std::endl;

    BOOL bUseDefaultVoltageArray[1];
    bUseDefaultVoltageArray[0] = TRUE;

    if (!PI_ATZ(ID, axis.c_str(), NULL, bUseDefaultVoltageArray)) {
      return false;
    }

    // Wait until the autozero move is done.
    BOOL bFlag = FALSE;

    while (bFlag != TRUE) {
      if (!PI_IsControllerReady(ID, &bFlag)) {
        return false;
      }
    }
  }

  std::cout << "AutoZero finished successfully" << std::endl;

  return true;
}

class Actuator {
 public:
  Actuator(char* szDescription);
  ~Actuator();
  void init();
  double read();
  void move(double value);
  void close();
  void autozero();

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

// run autozero procedure
void Actuator::autozero() {
  if (!AutoZeroIfNeeded(this->iD, "2")) {
    std::cout << "Autozero failed" << std::endl;
  }
}

void Actuator::close() { PI_CloseConnection(iD); }

int main() {
  char szDescription1[1024] = "0122040101";
  Actuator tiptilt1(szDescription1);
  char szDescription2[1024] = "0122042007";
  Actuator tiptilt2(szDescription2);

  tiptilt1.init();
  tiptilt2.init();

  // autozero actuators (use if actuator is not moving)
  // tiptilt1.autozero();
  // tiptilt2.autozero();

  // read actuator values
  double value = tiptilt1.read();
  double value2 = tiptilt2.read();
  std::cout << "Actuator1 value: " << value << std::endl;
  std::cout << "Actuator2 value: " << value2 << std::endl;

  // wait 100 ms
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // move actuators
  tiptilt1.move(0.);
  tiptilt2.move(0.);

  // wait 100 ms
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // read actuator values
  value = tiptilt1.read();
  std::cout << "Actuator1 value: " << value << std::endl;
  value2 = tiptilt2.read();
  std::cout << "Actuator2 value: " << value2 << std::endl;

  // close connection
  tiptilt1.close();

  return 0;
}