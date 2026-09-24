# BlackHoleSim


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
