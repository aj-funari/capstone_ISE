# Welcome to the Demonstration Branch of our Capstone Project!
This branch is focused on the demonstration portion of the uMove Capstone project from 2023. 
In this branch there are a few items of note. 

I have included the exact version of Godot that I used to create the video game for our project, this is the Godot_v3.5.1-stable_win64.exe folder. Godot is a fully self contained program and as such I believe that downloading this directly from our GitHub should work for other users. If not the download can be found [here](https://github.com/godotengine/godot/releases)

Inside this folder is a file called [godot_3.5.1-stable_rpi4_export-template_lto.bin](https://github.com/aj-funari/capstone_ISE/blob/demonstration/Godot_v3.5.1-stable_win64.exe/godot_3.5.1-stable_rpi4_export-template_lto.bin "godot_3.5.1-stable_rpi4_export-template_lto.bin")
**It is critical that this file is used as the export template when exporting for Raspberry Pi, as it is not natively supported by Godot.** The file can be found at [this](https://github.com/hiulit/Unofficial-Godot-Engine-Raspberry-Pi) GitHub Repo

When attempting to work on the video game, take note that **GLES2 MUST BE USED** as Raspberry Pi cannot run GLES3. 

All art assets should be contained within each project folder. (Wearable Demo and Wearable Demo Desert), if problems occur all assets used can be found in the Kenney folders, which were gotten from [Kenney.nl](https://www.kenney.nl/)

Lastly, the middleware is included and is called BLE_ReadData.py This uses a python library called PyKey and can be found [here](https://github.com/gauthsvenkat/pyKey). The comments in the file should be sufficient in understanding our code. 
