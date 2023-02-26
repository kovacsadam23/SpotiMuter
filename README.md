# SpotiMuter - the legal Spotify "AdBlocker"
Ever been fed up with too much advertisements? Ever felt enraged after listening to the same ad every five minutes? Don't want to subscribe to one of their plans?
Well, I have a solution for you. It is called SpotiMuter.

** UPDATE INFOS DOWN BELOW **

# About
SpotiMuter is a simple console that mutes your device temporarily, and when the ad is over, it unmutes your PC.
The app can be used on any Windows computer, by running the .exe that can be found in the repository (.EXE folder).

**How to use**
- Download the latest [SpotiMuter.exe](https://github.com/kovacsadam23/SpotiMuter/.EXE/SpotiMuter.exe) file located in .EXE folder, then run it
- Build the project yourself ~~(Linking of the Ole32 library is required: "-lole32")~~ in CLion!

# UI and future updates
In its current state the app recognizes whether Spotify is running or not, and displays info on the standard output. When an ad comes, it mutes your system's output volume, and when it's over, it sets the volume to the level used before.
In the future:
- Spotify minimization fix

Thanks for reading, have a good ad-free time!

# Update information (26/02/2023)

- Project moved to CLion platform.
- Memory usage fix (std::string's garbage collector was not called properly by the OS, thus the memory consumption was very high)
- Some performance tweaks