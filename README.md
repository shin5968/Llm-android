# Vulkan Diagnostic Android APK

Minimal Android test app (Kotlin + NDK C++) for checking Vulkan feasibility on target devices (including Samsung Xclipse 940 class GPUs).

## What the app does

On launch, the app shows:
- **Run Vulkan Check** button
- A text result area

When you tap the button, native Vulkan code:
- Enumerates Vulkan instance version
- Lists instance extensions
- Checks support for:
  - `VK_KHR_surface`
  - `VK_KHR_android_surface`
- Creates a Vulkan instance (with available required extensions)
- Enumerates physical devices
- Prints first physical device properties:
  - `vendorID`
  - `deviceID`
  - `deviceName`
  - `apiVersion`
- Returns human-readable errors if Vulkan calls fail

## Build in Android Studio

1. Open `/tmp/workspace/shin5968/Llm-android` in Android Studio.
2. Install SDK components when prompted:
   - Android SDK Platform 33
   - Android SDK Build-Tools
   - NDK (side-by-side)
   - CMake
3. Let Gradle sync complete.
4. Build with **Build > Make Project**.
5. Run on a physical device.

## CLI build

```sh
cd /tmp/workspace/shin5968/Llm-android
gradle assembleDebug
```

Debug APK output:

`/tmp/workspace/shin5968/Llm-android/app/build/outputs/apk/debug/app-debug.apk`

## Example output

```text
Vulkan instance version: 1.3.275

Instance extensions (20):
 - VK_KHR_surface
 - VK_KHR_android_surface
 ...

Required extension check:
 - VK_KHR_surface: SUPPORTED
 - VK_KHR_android_surface: SUPPORTED

Physical devices: 1

First physical device:
 - deviceName: ...
 - vendorID: 0x...
 - deviceID: 0x...
 - apiVersion: 1.3.x
```