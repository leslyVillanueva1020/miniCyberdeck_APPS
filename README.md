# 🌸 Mini Cyberdeck

### A whimsical Y2K-inspired handheld computer 💻

Mini Cyberdeck is a personal hardware and software project focused on building a small, portable Raspberry Pi computer with a colorful, retro-inspired interface.

Inspired by early 2000s technology, translucent electronics, digital stationery, and the simplicity of classic handheld devices, this project combines embedded hardware, C++, Qt, and custom UI design.

The goal is to create a tiny, functional computer with its own home screen and collection of independently developed applications.

## ✨ Project Goals

- Build a portable Raspberry Pi-powered cyberdeck with a touchscreen.
- Create a custom, full-screen, iPod-style application launcher.
- Develop lightweight applications using C++ and Qt.
- Support note-taking, terminal access, web browsing, and file management.
- Design a cohesive, whimsical Y2K-inspired interface.
- Integrate physical controls, including a dedicated Home button.
- Learn more about embedded systems, electronics, PCB design, and Linux application development.

## 📱 Planned Applications

| Application | Description | Status |
|---|---|---|
| 📓 MiniNotes | A digital notebook for creating, editing, renaming, saving, and deleting notes. | Functional; polishing |
| 🏠 CyberdeckLauncher | The central home screen for opening and managing applications. | In development |
| 💻 MiniTerminal | Terminal access for Linux commands. | Planned |
| 🌐 MiniBrowser | Web browsing and internet access. | Planned |
| 📁 MiniFiles | Browse and manage files. | Planned |
| ⚙️ Settings | Device preferences and customization. | Planned |

## 📓 MiniNotes

MiniNotes is the first application developed for Mini Cyberdeck.

Built with C++ and Qt Widgets, it provides a simple note-taking experience with a custom retro stationery interface.

### Features

- Create and save text notes locally.
- Open and edit existing notes.
- Rename notes.
- Delete multiple notes using a checkbox-based dialog.
- Custom lined notebook-paper editor.
- Pastel scrollbars, decorative labels, and SVG icons.
- Fredoka and Nunito typography.

### Design

The interface uses a soft pastel palette inspired by early 2000s stationery and translucent gadgets.

| Color | Hex |
|---|---|
| Frosted Aqua | `#B3EAF0` |
| Bubblegum Pink | `#F6B4D2` |
| Deep Plum | `#513457` |
| Warm Cream | `#FFF8E9` |
| Soft Pink | `#E8CED7` |

## 🏠 CyberdeckLauncher

CyberdeckLauncher will serve as the main interface for the device.

Rather than combining every feature into one large application, each Mini Cyberdeck app will remain an independent executable.

The launcher will use Qt's `QProcess` to start applications and manage their lifecycle.

### Planned Navigation

1. The device opens to the custom home screen.
2. The user selects an application.
3. The application opens in a full-screen interface.
4. A dedicated Home control provides a way to return to the launcher.
5. The user can select another application.

The physical Home button and application lifecycle behavior will be implemented and tested during integration.

## 🗂️ Repository Structure

```text
miniCyberdeck_APPS/
├── MiniNotes/
│   ├── main.cpp
│   ├── NotesWindow.cpp
│   ├── NotesWindow.h
│   ├── NotebookTextEdit.cpp
│   ├── NotebookTextEdit.h
│   ├── CMakeLists.txt
│   ├── assets/
│   └── styles/
│
├── CyberdeckLauncher/
│   ├── main.cpp
│   ├── HomeWindow.cpp
│   ├── HomeWindow.h
│   └── CMakeLists.txt
│
└── README.md
```

Additional application directories will be added as development continues.

## 🛠️ Technologies

**Software**
- C++17
- Qt 6 Widgets
- CMake
- Raspberry Pi OS (target platform)
- Git and GitHub

**Hardware**
- Raspberry Pi
- Touchscreen LCD
- Rechargeable battery and power-management hardware
- Custom enclosure and physical controls

Hardware integration is ongoing.

## 🚧 Development Roadmap

- [x] Build the initial MiniNotes application.
- [x] Implement note creation, editing, saving, renaming, and deletion.
- [x] Design the pastel MiniNotes interface.
- [x] Add custom icons, typography, and notebook lines.
- [ ] Complete CyberdeckLauncher navigation and application management.
- [ ] Integrate a dedicated physical Home button.
- [ ] Develop additional applications.
- [ ] Test applications on Raspberry Pi OS.
- [ ] Integrate the touchscreen and power hardware.
- [ ] Finalize the enclosure and device assembly.

## 🎨 Design Inspiration

Mini Cyberdeck draws inspiration from:

- Early 2000s portable electronics and digital organizers.
- Translucent and colorful retro gadgets.
- Pastel stationery, notebooks, and playful iconography.
- Soft, rounded typography.
- Small decorative details such as flowers, stickers, and vines.

The goal is to make the device feel personal, playful, and functional without sacrificing usability.

## 📷 Screenshots

Screenshots of MiniNotes and the CyberdeckLauncher will be added as development progresses.

## 🌷 Project Status

**Active development**

MiniNotes is functional, and work is continuing on the launcher, additional applications, and Raspberry Pi hardware integration.

This project is also a hands-on learning experience in C++, Qt, embedded systems, PCB design, and user-interface development.

---

*Made with curiosity, creativity, and lots of pink. ♡*
