# Password Manager API (Drogon-based)

A lightweight and beginner-friendly web-based password manager API built using **C++** and the **Drogon web framework**.  
This project is designed for educational purposes, showcasing how to build a simple REST API with encrypted password storage in a JSON file.

---

## Features

- **Account creation** with unique API keys
- **Add, view, and delete passwords** via HTTP endpoints
- **XOR-based encryption** for password storage (for demonstration only)
- **JSON file-based storage** (no database required)
- **Instruction page** served at `http://localhost:5555/`

---

## Disclaimer

This project is for **learning purposes only**.  
The XOR encryption used here is **not secure** for production environments.

---

## Installation & Setup

### **Install Tools**

```bash
sudo yay -Syu git cmake g++ uuid-dev openssl libssl-dev zlib1g-dev
```

### **Install Drogon**

```bash
git clone https://github.com/drogonframework/drogon.git
cd drogon
git submodule update --init
mkdir build && cd build
cmake ..
make && sudo make install
```

### **Check Installation and version**

```bash
drogon_ctl --version
```

### Clone & Build the Project

```bash
git clone https://github.com/safal-gautamm/pwdmanager_api.git
cd pwdmanager_api
mkdir build && cd build
cmake ..
make
```

### Run the API

```bash
./pwdmanager
```

If successful you will see:

```text
============================================
  Password Manager API is running!
  Access it at:  http://localhost:5555/
  Press Ctrl + C to stop the server.
============================================
```

Then open browser at:

```text
http://localhost:5555/
```

## API Endpoints

### Create Account

```bash
curl -X POST "http://localhost:5555/create?name=YOUR_NAME"
```

### Add Password

```bash
curl -X POST "http://localhost:5555/add?apikey=API_KEY&site=SITE.COM&username=USER&password=PASSWORD"
```
### Update Site

```bash
curl -X PATCH "http://localhost:5555//update?apikey=API_KEY&site=SITE.COM&username=USER&password=PASSWORD"
```

### View all passwords

```bash
curl "http://localhost:5555/view?name=YOUR_NAME&apikey=YOUR_APIKEY"
```

### Delete all password

```bash
curl -X DELETE "http://localhost:5555/delete?name=YOUR_NAME&apikey=YOUR_APIKEY"
```

All data are stored on [`passwords.json`](./passwords.json) file.
