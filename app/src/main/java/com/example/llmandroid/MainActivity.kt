package com.example.llmandroid

import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val runButton: Button = findViewById(R.id.runVulkanCheckButton)
        val resultText: TextView = findViewById(R.id.resultText)

        runButton.setOnClickListener {
            resultText.text = NativeBridge.runVulkanCheck()
        }
    }
}
