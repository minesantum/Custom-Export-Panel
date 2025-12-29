# Custom Export Panel - OBS Studio Plugin

Custom plugin for OBS Studio that adds an export panel with custom filenames and visual warnings when a file already exists.

## Features

- 🎯 Custom export panel integrated into OBS
- 📂 **Native File Explorer**: Tree view with icons and details. Supports **Double Click** to open files, **Drag & Drop**, and a full context menu (Open, Show in Explorer, Rename, Delete).
- ⚠️ Visual warning when a file with the same name already exists
- 📝 Customizable filenames
- 🛡️ **Safe Renaming**: Rename your files without worrying about accidentally deleting the extension.
- 🎨 Interface integrated with Qt6

## Installation

### From Releases (Recommended)

1. Go to the [Releases page](https://github.com/minesantum/Custom-Export-Panel/releases)
2. Download the latest version of `custom-export-panel.dll`
3. Copy the file to your OBS plugins folder:
   - Windows: `C:\Program Files\obs-studio\obs-plugins\64bit\`
4. Restart OBS Studio

### Manual Compilation

If you prefer to compile the plugin yourself:

1. **Requirements:**
   - CMake 3.16 or higher
   - Visual Studio 2019 or higher
   - Qt 6.8.3
   - OBS Studio 32.0.4 (binaries and headers)

2. **Compile:**
   ```bash
   # Edit compile.bat with the correct paths to your dependencies
   compile.bat
   ```

3. **Install:**
   ```bash
   install_plugin.bat
   ```

## Usage

1. Open OBS Studio
2. The custom export panel will appear in the interface
3. Configure your custom filename
4. If a file with that name already exists, you will see a visual warning

## Configuration

You can create a configuration file at:
```
%APPDATA%\obs-studio\plugin_config\custom-export-panel\export_config.ini
```

See `export_config.ini.example` for available options.

## Development

### Project Structure

```
Custom Export Panel/
├── .github/
├── .github/
│   └── workflows/
│       └── build-and-release.yml  # GitHub Actions workflow
├── src/
│   ├── plugin-main.cpp            # Plugin entry point
│   ├── custom-export-dock.hpp     # Panel header
│   └── custom-export-dock.cpp     # Panel implementation
├── include/                        # OBS Headers
├── libs/                          # OBS Libraries
├── CMakeLists.txt                 # CMake Configuration
└── compile.bat                    # Compilation script
```

### Create a New Release

See [RELEASES.md](RELEASES.md) for detailed instructions on how to create automatic releases.

**Quick summary:**
```bash
git tag v1.0.0
git push origin v1.0.0
```

GitHub Actions will automatically compile and publish the release.

## Technologies Used

- **C++17** - Main language
- **Qt6** - Graphical interface framework
- **CMake** - Build system
- **OBS Studio API** - OBS Integration

## License

This project is open source and is available under a permissive license.

## Creator

**DonKolia**

## Support

If you find any issues or have suggestions:
- Open an [Issue](https://github.com/minesantum/Custom-Export-Panel/issues)
- Check [Releases](https://github.com/minesantum/Custom-Export-Panel/releases) for the latest version

---

⭐ If you find this plugin useful, consider giving the repository a star!
