#include <jni.h>
#include <vulkan/vulkan.h>

#include <sstream>
#include <string>
#include <vector>

namespace {

std::string vkResultToString(VkResult result) {
    switch (result) {
        case VK_SUCCESS:
            return "VK_SUCCESS";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_UNKNOWN:
            return "VK_ERROR_UNKNOWN";
        default:
            return "VK_ERROR(code=" + std::to_string(result) + ")";
    }
}

std::string formatVersion(uint32_t version) {
    std::ostringstream out;
    out << VK_API_VERSION_MAJOR(version) << "."
        << VK_API_VERSION_MINOR(version) << "."
        << VK_API_VERSION_PATCH(version);
    return out.str();
}

}  // namespace

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_llmandroid_NativeBridge_runVulkanCheck(JNIEnv* env, jobject /*thiz*/) {
    std::ostringstream out;

    uint32_t instanceVersion = VK_API_VERSION_1_0;
    auto enumerateInstanceVersion =
            reinterpret_cast<PFN_vkEnumerateInstanceVersion>(
                    vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion"));
    if (enumerateInstanceVersion != nullptr) {
        const VkResult versionResult = enumerateInstanceVersion(&instanceVersion);
        if (versionResult != VK_SUCCESS) {
            out << "Failed to query Vulkan instance version: " << vkResultToString(versionResult)
                << "\n";
            return env->NewStringUTF(out.str().c_str());
        }
    }
    out << "Vulkan instance version: " << formatVersion(instanceVersion) << "\n\n";

    uint32_t extensionCount = 0;
    VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    if (result != VK_SUCCESS) {
        out << "Failed to enumerate instance extensions: " << vkResultToString(result);
        return env->NewStringUTF(out.str().c_str());
    }

    std::vector<VkExtensionProperties> extensions(extensionCount);
    result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    if (result != VK_SUCCESS) {
        out << "Failed to read instance extensions: " << vkResultToString(result);
        return env->NewStringUTF(out.str().c_str());
    }

    out << "Instance extensions (" << extensionCount << "):" << "\n";
    bool hasSurface = false;
    bool hasAndroidSurface = false;
    for (const auto& ext : extensions) {
        out << " - " << ext.extensionName << "\n";
        if (std::string(ext.extensionName) == VK_KHR_SURFACE_EXTENSION_NAME) {
            hasSurface = true;
        }
        if (std::string(ext.extensionName) == VK_KHR_ANDROID_SURFACE_EXTENSION_NAME) {
            hasAndroidSurface = true;
        }
    }
    out << "\nRequired extension check:" << "\n";
    out << " - " << VK_KHR_SURFACE_EXTENSION_NAME << ": "
        << (hasSurface ? "SUPPORTED" : "MISSING") << "\n";
    out << " - " << VK_KHR_ANDROID_SURFACE_EXTENSION_NAME << ": "
        << (hasAndroidSurface ? "SUPPORTED" : "MISSING") << "\n\n";

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VulkanDiagnostic";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "None";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = instanceVersion;

    const char* enabledExtensions[2];
    uint32_t enabledExtensionCount = 0;
    if (hasSurface) {
        enabledExtensions[enabledExtensionCount++] = VK_KHR_SURFACE_EXTENSION_NAME;
    }
    if (hasAndroidSurface) {
        enabledExtensions[enabledExtensionCount++] = VK_KHR_ANDROID_SURFACE_EXTENSION_NAME;
    }

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = enabledExtensionCount;
    createInfo.ppEnabledExtensionNames = enabledExtensions;

    VkInstance instance = VK_NULL_HANDLE;
    result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        out << "Failed to create Vulkan instance: " << vkResultToString(result);
        return env->NewStringUTF(out.str().c_str());
    }

    uint32_t physicalDeviceCount = 0;
    result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
    if (result != VK_SUCCESS) {
        out << "Failed to enumerate physical devices: " << vkResultToString(result);
        vkDestroyInstance(instance, nullptr);
        return env->NewStringUTF(out.str().c_str());
    }

    out << "Physical devices: " << physicalDeviceCount << "\n";
    if (physicalDeviceCount > 0) {
        std::vector<VkPhysicalDevice> devices(physicalDeviceCount);
        result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, devices.data());
        if (result == VK_SUCCESS) {
            VkPhysicalDeviceProperties props{};
            vkGetPhysicalDeviceProperties(devices[0], &props);

            out << "\nFirst physical device:" << "\n";
            out << " - deviceName: " << props.deviceName << "\n";
            out << " - vendorID: 0x" << std::hex << props.vendorID << std::dec << "\n";
            out << " - deviceID: 0x" << std::hex << props.deviceID << std::dec << "\n";
            out << " - apiVersion: " << formatVersion(props.apiVersion) << "\n";
        } else {
            out << "Failed to read physical device list: " << vkResultToString(result) << "\n";
        }
    }

    vkDestroyInstance(instance, nullptr);

    return env->NewStringUTF(out.str().c_str());
}
