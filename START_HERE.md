# 🚀 БЫСТРЫЙ СТАРТ - PDF Editor

## ✅ Что у вас уже есть:
- Visual Studio 2022 ✅
- CMake 4.0.3 ✅
- Chocolatey ✅
- Git ✅

## ❌ Что нужно установить:
- Python (для установки Qt)
- Qt 6.5

---

## 🎯 ДВА ПРОСТЫХ ШАГА:

### ШАГ 1: Установите зависимости (5-10 минут)

Откройте PowerShell **КАК АДМИНИСТРАТОР** и выполните:

```powershell
cd "c:\Users\KeyWest\Desktop\PROJECTS\PDF Editor"

# Установка Python
choco install python -y

# Обновление переменных окружения
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")

# Установка aqt (Qt installer)
python -m pip install aqt

# Установка Qt 6.5.3 LTS
aqt install-qt windows desktop 6.5.3 win64_msvc2019_64 -O "$env:USERPROFILE\Qt"
```

**Это займет 10-15 минут, Qt большой (~2-3 GB)**

### ШАГ 2: Соберите проект (5-10 минут)

После установки Qt, в том же PowerShell:

```powershell
# Создать папку для сборки
mkdir build -Force
cd build

# Настроить проект
$env:CMAKE_PREFIX_PATH = "$env:USERPROFILE\Qt\6.5.3\msvc2019_64"
cmake .. -G "Visual Studio 17 2022" -A x64

# Собрать
cmake --build . --config Release

# Скопировать DLL Qt
cd Release
& "$env:USERPROFILE\Qt\6.5.3\msvc2019_64\bin\windeployqt.exe" PDFEditor.exe

# Запустить!
.\PDFEditor.exe
```

---

## 🔥 АВТОМАТИЧЕСКИЙ ВАРИАНТ (еще проще!):

Просто запустите эти 3 команды в PowerShell (Администратор):

```powershell
cd "c:\Users\KeyWest\Desktop\PROJECTS\PDF Editor"

# 1. Установите Python
choco install python -y

# 2. Закройте и откройте PowerShell заново, затем:
python -m pip install aqt
aqt install-qt windows desktop 6.5.3 win64_msvc2019_64 -O "$env:USERPROFILE\Qt"

# 3. Запустите скрипт сборки
.\build_project.bat
```

---

## 📤 ДЛЯ GITHUB:

Пока идет установка Qt, откройте браузер:

1. Перейдите: **https://github.com/new**
2. Repository name: `PDFEditor`
3. Нажмите "Create repository" (БЕЗ галочек!)
4. После создания выполните:
   ```bash
   git push -u origin main
   ```

---

## ⚡ БЫСТРЫЕ КОМАНДЫ:

**Проверить установку Python:**
```bash
python --version
```

**Проверить установку Qt:**
```bash
dir "$env:USERPROFILE\Qt\6.5.3"
```

**Пересобрать проект:**
```bash
cd build
cmake --build . --config Release
```

---

## 🐛 Возможные проблемы:

**"Python not found"**
- Закройте и откройте PowerShell после установки
- Или перезагрузите компьютер

**"Qt not found"**
- Проверьте путь: `$env:USERPROFILE\Qt\6.5.3\msvc2019_64`
- Если установка прервалась, запустите команду aqt снова

**"MSVC not found"**
- Убедитесь, что в Visual Studio Installer установлен "Desktop development with C++"

---

## 🎉 ПОСЛЕ СБОРКИ:

Приложение будет здесь:
```
c:\Users\KeyWest\Desktop\PROJECTS\PDF Editor\build\Release\PDFEditor.exe
```

Запустите и наслаждайтесь 240 FPS! 🚀
