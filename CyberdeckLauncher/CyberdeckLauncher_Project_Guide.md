# Mini Cyberdeck - CyberdeckLauncher Project Guide

**Project status:** Planning / ready to start implementation  
**Updated:** October 16, 2026  
**Architecture decision:** Full-screen iPod-style home screen, with independent Qt applications.

## 1. Project vision

Create a small, personalized app environment for the Mini Cyberdeck, inspired by an iPod: a home screen displays app icons, tapping an icon opens that app full-screen, and a clear Home action returns to the launcher. The system runs on Raspberry Pi OS; **we are not building a new operating system**.

Visual direction: whimsical colorful 2000s / retro gadget / stationery style; frosted aqua (`#B3EAF0`), bubblegum pink (`#F6B4D2`), deep plum (`#513457`), warm cream (`#FFF8E9`); rounded controls, Fredoka headings, Nunito text, and optional small decorative accents. Plants and vines are accents, not the entire identity. Final launcher art and exact screen layout are still undecided.

## 2. What already exists: MiniNotes

MiniNotes is a separate C++ / Qt 6 Widgets program built with CMake. It has a Notes List, lined-paper editor, note creation, rename, saving/loading text files, and a multi-select Delete Notes dialog. Its custom QSS, SVG icons, and Fredoka/Nunito font assets are stored in its own project. Its entry point is `main.cpp`, and its main UI is `NotesWindow` with `NotebookTextEdit` for ruled paper.

MiniNotes remains independently buildable and runnable. The launcher **does not copy its code or turn it into a page inside the launcher**. This is an intentional decision that also lets future apps be developed independently.

**Known follow-up:** Note titles containing a slash (`/`) can fail because the title is currently used directly as part of a filename (e.g., `midterm 10/13`). Filename validation or a separate safe file identifier should be added before relying on unrestricted note names. Existing note saving is triggered by the editor's Back button; later, test what happens when an app is closed via its window controls or a system shutdown.

## 3. System architecture

```text
Raspberry Pi OS (Linux + desktop/session + device services)
    |
    +-- CyberdeckLauncher (Qt Widgets, home screen)
    |      +-- app icon grid
    |      +-- application launching / lifecycle
    |      +-- return-to-home behavior
    |      +-- eventual status UI (Wi-Fi, battery, time)
    |
    +-- MiniNotes (separate executable)
    +-- MiniTerminal (planned)
    +-- MiniBrowser (planned)
    +-- MiniFiles (planned)
    +-- MiniSettings (planned)
```

The launcher starts an external executable using **`QProcess`**. Each app owns its own window, code, data, and assets. `QProcess` reports launch failures and process exit so the launcher can respond. `QStackedWidget` may still be useful *inside* a single app, but is not how the launcher runs separate programs.

### Full-screen lifecycle

1. Raspberry Pi OS starts a graphical session and eventually starts CyberdeckLauncher.
2. The launcher displays a full-screen home screen.
3. The user taps Notes.
4. The launcher starts MiniNotes with `QProcess`, specifying its executable path **and working directory**.
5. MiniNotes is shown full-screen; the launcher is hidden or otherwise stays behind it.
6. The user uses MiniNotes normally. Its existing Back arrow remains **Editor -> Notes List**, not Home.
7. A separate Home/Exit control in MiniNotes closes that application cleanly; the launcher detects process exit and reappears.
8. If MiniNotes fails to start or crashes, the launcher should recover, show a useful error, and remain accessible.

**Implementation note:** Hiding the launcher only after confirming a successful start avoids leaving the user with a blank screen on launch failure. Exact full-screen/window behavior must be tested under the Raspberry Pi graphical environment. The Windows development build uses `.exe`; Linux uses a Linux-built executable, not the Windows `.exe`.

## 4. Recommended folder structure

```text
miniCyberdeck_APPS/
  MiniNotes/
    CMakeLists.txt
    main.cpp
    NotesWindow.h / NotesWindow.cpp
    NotebookTextEdit.h / NotebookTextEdit.cpp
    styles/notes.qss
    assets/icons/...
    assets/fonts/...
    notes/                   (currently runtime data; location to revisit)
    build/                   (generated; not source-controlled)
  CyberdeckLauncher/
    CMakeLists.txt
    main.cpp
    HomeWindow.h
    HomeWindow.cpp
    assets/icons/            (later)
    styles/launcher.qss      (later)
    build/                   (generated)
  MiniTerminal/              (future)
  MiniBrowser/               (future)
  MiniFiles/                 (future)
  MiniSettings/              (future)
```

Initially keep each app's CMake project separate. Shared theme resources or a common library can be introduced later, if duplication becomes painful.

## 5. Classes and responsibilities

| Class / component | Job |
| --- | --- |
| `QApplication` | Qt application event loop |
| `HomeWindow : QWidget` | Launcher home UI |
| `QGridLayout` | App icon arrangement |
| `QPushButton` | App launch actions |
| `QProcess` | Start and observe independent app processes |
| `QFileInfo`, `QDir` | Build safe paths and working directories |
| `QStackedWidget` | Optional internal pages within one application |
| `NotesWindow` | Existing MiniNotes UI, unchanged for initial launch test |

`HomeWindow` should own or otherwise manage the running child process. For a first prototype, launch only one child app at a time. Prevent accidental duplicate launches, and connect process error and finished signals so the launcher always knows when to restore Home.

## 6. Step-by-step implementation roadmap

### Phase 1 - Minimal launcher project
- [ ] Create `CyberdeckLauncher/` next to `MiniNotes/`.
- [ ] Add `main.cpp`, `HomeWindow.h`, `HomeWindow.cpp`, and `CMakeLists.txt`.
- [ ] Configure CMake for C++17 and Qt 6 Widgets.
- [ ] Implement a minimal `HomeWindow` subclass of `QWidget`.
- [ ] Build and run a simple window with one Notes button.

**Done when:** The new project builds independently and shows a clickable button.

### Phase 2 - Launch the existing MiniNotes
- [ ] Learn `QProcess` and choose how `HomeWindow` owns the process.
- [ ] Resolve MiniNotes' executable path (Windows for local development, Linux on Pi).
- [ ] Set MiniNotes' working directory to its project/runtime folder so its relative `styles/`, `assets/`, and `notes/` paths work.
- [ ] Connect Notes button click to a launch function.
- [ ] Handle `errorOccurred`, `started`, and `finished` signals appropriately.
- [ ] Prevent multiple MiniNotes instances from rapid repeated clicks.

**Done when:** Clicking Notes launches the current MiniNotes app with its styles, icons, fonts, and saved notes intact.

### Phase 3 - iPod-style navigation
- [ ] Run the launcher full-screen.
- [ ] Launch MiniNotes full-screen (initially a small change in MiniNotes' window setup).
- [ ] Hide launcher while MiniNotes is running, and restore it when MiniNotes exits.
- [ ] Add a distinct Home/Exit action in MiniNotes; preserve the existing Back arrow for internal navigation.
- [ ] Verify note saving when returning Home. Consider saving on close or when content changes, not only when Back is clicked.
- [ ] Test launch failure, app crash, repeated opens, and returning Home.

**Done when:** Home -> Notes -> Home feels like switching between apps on an iPod.

### Phase 4 - Home screen design
- [ ] Agree on screen dimensions and touch target sizes using the actual display.
- [ ] Create an app icon grid (start with Notes, then placeholders for planned apps).
- [ ] Add aqua/pink/cream/plum theme, Fredoka/Nunito, rounded retro-gadget buttons.
- [ ] Decide whether to make or license original icon art; avoid directly copying others' Pinterest artwork.
- [ ] Add restrained decorative details (small stickers or vines) after the layout works.
- [ ] Make the grid adaptable to small screen dimensions and test finger interaction.

**Done when:** The home screen is usable and visually consistent with MiniNotes.

### Phase 5 - Add apps incrementally
- [ ] **MiniTerminal:** decide whether to launch an existing terminal emulator first or build a custom terminal UI. Real terminal emulation is more than a text box.
- [ ] **MiniBrowser:** test available RAM, graphics support, screen size, and web-engine feasibility; consider launching an existing browser first.
- [ ] **MiniFiles:** basic file browsing and safe file operations.
- [ ] **MiniSettings:** device preferences and selected status information.
- [ ] Register each finished app with the launcher, with its own path and working directory.

**Done when:** Multiple independently built apps launch and return Home reliably.

### Phase 6 - Raspberry Pi deployment
- [ ] Build the launcher and apps for Raspberry Pi OS (native or appropriate cross-build).
- [ ] Install Qt and required plugins, fonts, SVG support, and browser dependencies as needed.
- [ ] Replace fragile source-relative paths with app-relative paths or Qt resources where appropriate.
- [ ] Store user notes in a stable writable data directory, separate from installed application binaries.
- [ ] Test display orientation, full-screen behavior, touch input, keyboard input, and performance.
- [ ] Configure autostart in the graphical session after the launcher is reliable.
- [ ] Test shutdown, restart, loss of power, and recovery behavior.

**Done when:** The device boots to the launcher and can use its apps without a development terminal.

## 7. First coding session checklist

1. Make the `CyberdeckLauncher` folder and four starter files.
2. Write the CMake configuration for a Qt 6 Widgets executable.
3. Create `QApplication` and show `HomeWindow` from `main.cpp`.
4. Make `HomeWindow` inherit from `QWidget` and add a layout.
5. Add a Notes button and verify its click signal using a debug message.
6. Only then introduce `QProcess` to open MiniNotes.

**Mentoring approach:** Build in small increments; explain Qt and CMake concepts; let the user implement code when possible, with complete examples when explicitly requested.

## 8. Technical decisions and cautions

- **Independent apps:** Yes; separate processes, not pages in one giant program.
- **OS:** Raspberry Pi OS underneath; launcher is an app, not a replacement OS.
- **Navigation:** Full-screen Home and full-screen apps; distinct Home vs in-app Back.
- **Asset paths:** MiniNotes currently expects working-directory-relative files. Explicitly set `QProcess::setWorkingDirectory()` during the prototype; improve packaging later.
- **App lifecycle:** Prefer one active child app initially; monitor process signals and handle failures.
- **Saving:** Verify behavior when closing MiniNotes without using its editor Back button.
- **Filename validation:** Address invalid filename characters, especially `/`, before final deployment.
- **Display constraints:** Exact physical display size/resolution and final hardware behavior must be verified; don't hardcode desktop-window assumptions.
- **Battery / status UI:** Wi-Fi and battery indicators are future integrations, not yet implemented; battery reporting may depend on the actual power hardware and its software interface.
- **Scope:** Home launcher first, then one working app integration, then more apps, then visual polish and device deployment.

## 9. Open decisions for later

- Exact Home screen icon artwork, background art, and decorative accents.
- Home/Exit control location and behavior inside MiniNotes.
- Whether the launcher remains hidden or visible underneath child apps during transitions.
- Exact Pi OS graphical session / window management behavior.
- Terminal and browser implementation choices after hardware performance testing.
- App installation paths and user-data storage conventions.

## 10. Definition of success

On the Mini Cyberdeck, powering on reaches a personalized full-screen home screen. Tapping Notes opens the existing MiniNotes with its content and visual theme. The user can leave Notes and return Home without losing work. The same mechanism can launch other independently developed apps, making the device feel like a small, coherent, colorful personal gadget.
