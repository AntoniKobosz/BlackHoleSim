<img width="2560" height="1440" alt="screenshot004" src="https://github.com/user-attachments/assets/ba68a37c-2c6e-4804-adca-f58727bef59f" />

# BlackHoleSim
Simulating a Schwarzschild black hole in real time using C++ and GLSL with raylib.

[Watch the demo on YouTube](https://www.youtube.com/watch?v=-Qs2CsVH6nE)
## Installation

### 1. Install Git LFS

Ubuntu/Debian:

```bash
sudo apt install git-lfs
git lfs install
```
Windows: Install Git LFS and run
```
git lfs install
```
### 2. Clone the repository

```bash
git clone https://github.com/AntoniKobosz/BlackHoleSim.git
cd BlackHoleSim
```

### 3. Download large files using Git LFS

```bash
git lfs pull
```

### 4. Build with CMake

```bash
cmake -B build
cmake --build build
```
### 5. Run the application
Linux:
```bash
./build/SkyRay
```
Windows:
```
.\build\SkyRay.exe
```
## Credits
- Star map: [NASA/Deep Star Maps 2020](https://svs.gsfc.nasa.gov/4851/)
- Sky panorama: "Chiemsee bei Seebruck Luftbild" by [SimonWaldherr](https://commons.wikimedia.org/wiki/User:SimonWaldherr),
https://commons.wikimedia.org/wiki/File:Chiemsee_bei_Seebruck_Luftbild.jpg, 
licensed under CC BY-SA 4.0 (https://creativecommons.org/licenses/by-sa/4.0).
- Built with [raylib](https://www.raylib.com/) (zlib license)
## License
Code: MIT. Assets keep their own licenses (see Credits).
