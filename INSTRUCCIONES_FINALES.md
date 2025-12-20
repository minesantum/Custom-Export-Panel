# Instrucciones para Resolver Error 126

## El Problema
El plugin compila correctamente pero OBS no puede cargarlo (error 126 - LoadLibrary failed).
Esto indica una incompatibilidad de versiones de Qt.

## Solución Recomendada

### Opción 1: Usar Dependencies (Recomendado)
1. Descarga **Dependencies** desde: https://github.com/lucasg/Dependencies/releases
2. Abre `Dependencies.exe`
3. Arrastra el archivo `build\Release\custom-export-panel.dll` a la ventana
4. Busca DLLs marcadas en **ROJO** (no encontradas) o **AMARILLO** (versión incorrecta)
5. Anota cuáles DLLs Qt están causando problemas

### Opción 2: Simplificar el Plugin (Sin Qt UI)
Como alternativa, puedo crear una versión **sin interfaz gráfica** del plugin que:
- No use Qt para la UI
- Solo modifique la ruta de grabación mediante código
- Use un archivo de configuración simple (INI o JSON)
- Funcione con las librerías OBS existentes

Esta versión sería más simple y evitaría completamente los problemas de Qt.

## ¿Qué Prefieres?

1. **Investigar con Dependencies** para encontrar el problema exacto de Qt
2. **Crear versión sin UI** que funcione de inmediato pero sin panel visual

Responde con "1" o "2" para continuar.
