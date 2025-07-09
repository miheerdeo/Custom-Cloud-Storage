
Custom Cloud Storage

Custom Cloud Backup Storage
This project provides a simple and lightweight cloud-based photo backup system designed to run on a local or personal server. It enables automatic or manual uploading of image files (such as photos from a mobile device) to a centralized storage location over a network.

Key Features:

1) HTTP-based File Upload: Accepts file uploads using standard HTTP POST requests.

2) Local Network Accessibility: Works over local Wi-Fi, making it fast and secure within your own environment.

3) Automatic Backup Support: Compatible with automation tools or scripts (like shell scripts or mobile automation apps) for background photo uploads.

4) Simple File Handling: Uploaded files are saved directly into a specified directory on the server.

5) Minimal Dependencies: Designed to be lightweight and easy to deploy with minimal external libraries.

Use Case:

Ideal for personal photo backups, this system can be set up on a Raspberry Pi, WSL (Windows Subsystem for Linux), or any Linux-based system to create a private photo vault, avoiding third-party cloud storage providers.

Libraries and Tools Used
1. C++ filesystem

Popular feature of C++20 used for efficient file Handling and directory creation.

2. cpp-httplib

A single-header C++ HTTP server and client library.

Handles incoming HTTP requests (GET, POST) and routes them to appropriate handlers.

Provides built-in support for file uploads.

It is lightweight with minimal Dependencies required than other libraries like boost.beast,libcurl etc.It is a header only library.

3. WSL (Windows Subsystem for Linux) (Optional)

Used to run the server in a Linux environment on a Windows system.

Helpful for users developing or hosting the server without a native Linux machine.

4. Termux (for Android clients) 

Used to automate photo backups from an Android device.

Allows scripting with shell scripts that interact with the server over HTTP.

5. Automation Tools (Optional)(Paid)

Tools like Macrodroid or Tasker can be used to trigger automatic uploads based on events (e.g., Wi-Fi connected, photo taken, etc.).



 Usage
1. Allow Access to WSL Server on Local Network
1) Open PowerShell and run ipconfig
2)Note your local IPv4 address under Wi-Fi
3)In Administrator PowerShell run command to allow the request from the mobbile through firewall.

Command:-
  New-NetFirewallRule -DisplayName "WSL HTTP Port 8080" -Direction Inbound -Protocol TCP -LocalPort 8080 -Action Allow

2. Setup Termux on Android
1)Download Termux,Termux api from Fdroid

2)Run:
pkg update
pkg install termux-api curl
termux-setup-storage

3)Allow Termux storage permission from Android system settings.

3. Create Upload Script
1) nano ~/upload_photos_action.sh

2) Write script to scan DCIM/Camera and upload .jpg files to your WSL server.

#!/data/data/com.termux/files/usr/bin/bash

UPLOAD_URL="http://<your-ip>:8080/upload"
CAMERA_DIR="/sdcard/DCIM/Camera"
LOG_FILE="$HOME/.uploaded_files.log"

touch "$LOG_FILE"

for file in "$CAMERA_DIR"/*.jpg; do
  [ -e "$file" ] || continue
  if ! grep -q "$file" "$LOG_FILE"; then
    if curl -s -F "file=@${file}" "$UPLOAD_URL"; then
      echo "$file" >> "$LOG_FILE"
    fi
  fi
done

termux-notification --title "Upload Complete" --content "All new photos uploaded." --id 100

3) chmod +x ~/upload_photos_action.sh

4. Create Wi-Fi SSID Checker and Trigger Script
1) mkdir -p ~/shortcuts

2) nano ~/shortcuts/check_wifi_and_prompt.sh

3) Write script to:

  1)Get current SSID

  2)Compare with home SSID

  3)Trigger upload_photos_action.sh if matched

  4)Send notification

  Script:-
  #!/data/data/com.termux/files/usr/bin/bash

    LOG="$HOME/check_wifi_debug.log"
    echo "==== $(date) ====" >> "$LOG"

    HOME_SSID="Miheers 5G"
    CURRENT_SSID=$(termux-wifi-connectioninfo | grep '"ssid"' | cut -d '"' -f4)

    echo "Expected SSID: [$HOME_SSID]" >> "$LOG"
    echo "Current SSID : [$CURRENT_SSID]" >> "$LOG"

    if [ "$CURRENT_SSID" != "$HOME_SSID" ]; then
    echo "SSID mismatch. Skipping upload." >> "$LOG"
    termux-notification --title "Not on $HOME_SSID" --content     "Upload skipped." --id 101
    exit 0
    fi

    echo "SSID matched. Uploading photos..." >> "$LOG"
    bash ~/upload_photos_action.sh


  5)chmod +x ~/shortcuts/check_wifi_and_prompt.sh

5. Install Termux Widget
1) pkg install termux-widget

2) Long-press home screen → Add Termux Widget

3) Select check_wifi_and_prompt.sh script from the widget

6. Manually Test Server from Termux
1) curl http://<your_ip>:8080

2) curl -F "file=@/sdcard/DCIM/Camera/photo.jpg" http:// <your_ip>:8080/upload

Screenshots:-
1)
https://github.com/miheerdeo/Custom-Cloud-Storage/blob/3600c0be2c265a414ddd6c32d51c93e55a869d72/1000001185.jpg
2)
https://github.com/miheerdeo/Custom-Cloud-Storage/blob/a60dc659f97bc047b3c6dccecbad07966c3cf5a7/1000001186.jpg
3)
https://github.com/miheerdeo/Custom-Cloud-Storage/blob/3e1366713bbb91799a4f6e838b5f26cf52c0fc17/1000001189.jpg
4)
https://github.com/miheerdeo/Custom-Cloud-Storage/blob/377986578e3751725208debe1e82f43ee9edd37b/Screenshot%202025-07-09%20154331.png




