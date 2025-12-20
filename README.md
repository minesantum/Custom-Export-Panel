# Custom Export Panel - Plugin para OBS Studio

Plugin personalizado para OBS Studio que agrega un panel de exportación con nombres de archivo personalizados y advertencias visuales cuando un archivo ya existe.

## Características

- 🎯 Panel de exportación personalizado integrado en OBS
- ⚠️ Advertencia visual cuando un archivo con el mismo nombre ya existe
- 📝 Nombres de archivo personalizables
- 🎨 Interfaz integrada con Qt6

## Instalación

### Desde Releases (Recomendado)

1. Ve a la [página de Releases](https://github.com/minesantum/Custom-Export-Panel/releases)
2. Descarga la última versión de `custom-export-panel.dll`
3. Copia el archivo a tu carpeta de plugins de OBS:
   - Windows: `C:\Program Files\obs-studio\obs-plugins\64bit\`
4. Reinicia OBS Studio

### Compilación Manual

Si prefieres compilar el plugin tú mismo:

1. **Requisitos:**
   - CMake 3.16 o superior
   - Visual Studio 2019 o superior
   - Qt 6.8.3
   - OBS Studio 32.0.4 (binarios y headers)

2. **Compilar:**
   ```bash
   # Edita compile.bat con las rutas correctas a tus dependencias
   compile.bat
   ```

3. **Instalar:**
   ```bash
   install_plugin.bat
   ```

## Uso

1. Abre OBS Studio
2. El panel de exportación personalizado aparecerá en la interfaz
3. Configura tu nombre de archivo personalizado
4. Si un archivo con ese nombre ya existe, verás una advertencia visual

## Configuración

Puedes crear un archivo de configuración en:
```
%APPDATA%\obs-studio\plugin_config\custom-export-panel\export_config.ini
```

Ver `export_config.ini.example` para opciones disponibles.

## Desarrollo

### Estructura del Proyecto

```
Custom Export Panel/
├── .github/
│   └── workflows/
│       └── build-and-release.yml  # GitHub Actions workflow
├── src/
│   ├── plugin-main.cpp            # Punto de entrada del plugin
│   ├── custom-export-dock.hpp     # Header del panel
│   └── custom-export-dock.cpp     # Implementación del panel
├── include/                        # Headers de OBS
├── libs/                          # Bibliotecas de OBS
├── CMakeLists.txt                 # Configuración de CMake
└── compile.bat                    # Script de compilación
```

### Crear una Nueva Release

Ver [RELEASES.md](RELEASES.md) para instrucciones detalladas sobre cómo crear releases automáticas.

**Resumen rápido:**
```bash
git tag v1.0.0
git push origin v1.0.0
```

GitHub Actions automáticamente compilará y publicará la release.

## Tecnologías Utilizadas

- **C++17** - Lenguaje principal
- **Qt6** - Framework de interfaz gráfica
- **CMake** - Sistema de compilación
- **OBS Studio API** - Integración con OBS

## Contribuir

Las contribuciones son bienvenidas! Por favor:

1. Fork el repositorio
2. Crea una rama para tu feature (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

## Licencia

Este proyecto es de código abierto y está disponible bajo una licencia permisiva.

## Creador

**DonKolia**

## Soporte

Si encuentras algún problema o tienes sugerencias:
- Abre un [Issue](https://github.com/minesantum/Custom-Export-Panel/issues)
- Revisa los [Releases](https://github.com/minesantum/Custom-Export-Panel/releases) para la última versión

---

⭐ Si este plugin te resulta útil, considera darle una estrella al repositorio!
