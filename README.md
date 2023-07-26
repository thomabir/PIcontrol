# PIcontrol
A simple C++ program to control a PI E-727 piezo controller. It sets up the connection to a controller given its serial number, performs a movement, reads the sensor position, and closes the connection.

## Installation (Linux)
- Clone this repository.
  ```sh
  git clone https://github.com/thomabir/PIcontrol.git
  ```
- Optain the software bundle from PI, referred to as `pi-drivers/` in the following.
  The files can be downloaded from the PI website or found on the installation CD that came with the stage.
- Install the PI GCS2 C++ library from `pi-drivers/`:
  ```sh
  cd pi-drivers/Linux/PI_Application_Software
  sudo ./INSTALL
  ```
  This should install, among other things, the `lipi_pi_gcs2.so` library
- Copy some files from `pi-drivers/` into `PIcontrol` (The files are not included in this repository because of licensing issues):
  ```sh
  cp pi-drivers/Development/C++/Samples/E-727/AutoZeroSample/AutoZeroSample.h PIcontrol/
  cp pi-drivers/Linux/PI_Application_Software/libpi_pi_gcs2_x86_64/include/PI_GCS2_DLL.h PIcontrol/
  ```

## Build
```sh
g++ PIcontrol.cpp -lpi_pi_gcs2
```

## Run
```sh
./a.out
```

## License
PIcontrol is licensed under the MIT License, see [LICENSE](LICENSE) for more information.
