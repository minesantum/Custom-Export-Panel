# Sistema de Compilación y Releases Automáticas

Este repositorio está configurado con GitHub Actions para compilar automáticamente el plugin y crear releases.

## Cómo Crear una Nueva Release

### Método 1: Usando Tags (Recomendado)

1. **Asegúrate de que todos tus cambios están commiteados:**
   ```bash
   git add .
   git commit -m "Descripción de los cambios"
   ```

2. **Crea un tag con la versión:**
   ```bash
   git tag v1.0.0
   ```
   
   El formato debe ser `v` seguido del número de versión (ej: `v1.0.0`, `v1.1.0`, `v2.0.0`)

3. **Sube el tag a GitHub:**
   ```bash
   git push origin v1.0.0
   ```

4. **GitHub Actions automáticamente:**
   - Descargará las dependencias necesarias (OBS Studio y Qt6)
   - Compilará el plugin
   - Creará una release en GitHub
   - Subirá el archivo `.dll` compilado
   - Creará un archivo ZIP con todo lo necesario

### Método 2: Ejecución Manual

También puedes ejecutar el workflow manualmente desde GitHub:

1. Ve a tu repositorio en GitHub
2. Click en la pestaña "Actions"
3. Selecciona "Build and Release" en el menú izquierdo
4. Click en "Run workflow"
5. Selecciona la rama y click en "Run workflow"

## Estructura de la Release

Cada release incluirá:

- `custom-export-panel.dll` - El plugin compilado listo para usar
- `custom-export-panel-vX.X.X.zip` - Paquete completo con:
  - El DLL del plugin
  - Archivo de configuración de ejemplo (si existe)
  - README con instrucciones de instalación

## Versionado Semántico

Se recomienda usar [Versionado Semántico](https://semver.org/lang/es/):

- **MAJOR** (v**1**.0.0): Cambios incompatibles con versiones anteriores
- **MINOR** (v1.**1**.0): Nueva funcionalidad compatible con versiones anteriores
- **PATCH** (v1.0.**1**): Correcciones de bugs compatibles con versiones anteriores

## Ejemplos de Uso

### Crear tu primera release:
```bash
git tag v1.0.0
git push origin v1.0.0
```

### Crear una actualización con nuevas características:
```bash
git tag v1.1.0
git push origin v1.1.0
```

### Crear una corrección de bugs:
```bash
git tag v1.0.1
git push origin v1.0.1
```

## Verificar el Estado de la Compilación

1. Ve a la pestaña "Actions" en GitHub
2. Verás el progreso de la compilación en tiempo real
3. Si algo falla, podrás ver los logs detallados

## Notas Importantes

- El workflow se ejecuta en Windows (necesario para compilar plugins de OBS)
- La compilación tarda aproximadamente 5-10 minutos
- Las releases se crean automáticamente y no son borradores
- Puedes editar la descripción de la release después de que se cree

## Creador

Plugin creado por **DonKolia**
