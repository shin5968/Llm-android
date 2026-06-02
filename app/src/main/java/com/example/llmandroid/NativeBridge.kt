package com.example.llmandroid

object NativeBridge {
    init {
        System.loadLibrary("vulkancheck")
    }

    external fun runVulkanCheck(): String
}
