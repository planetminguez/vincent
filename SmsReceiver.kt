package com.example.autoreply

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.SharedPreferences
import android.telephony.SmsManager
import android.telephony.SmsMessage
import android.widget.Toast

class SmsReceiver : BroadcastReceiver() {

    override fun onReceive(context: Context, intent: Intent) {
        val sharedPreferences: SharedPreferences =
            context.getSharedPreferences("AutoReplyPrefs", Context.MODE_PRIVATE)

        val isAutoReplyEnabled = sharedPreferences.getBoolean("autoReplyEnabled", false)
        val replyMessage = sharedPreferences.getString("replyMessage", "I am busy right now. I'll get back to you soon!")

        if (!isAutoReplyEnabled) return

        if (intent.action == "android.provider.Telephony.SMS_RECEIVED") {
            val bundle = intent.extras
            if (bundle != null) {
                val pdus = bundle.get("pdus") as Array<*>
                val format = bundle.getString("format")
                pdus.forEach { pdu ->
                    val sms = SmsMessage.createFromPdu(pdu as ByteArray, format)
                    val sender = sms.originatingAddress

                    // Send auto-reply
                    if (sender != null) {
                        val smsManager = SmsManager.getDefault()
                        smsManager.sendTextMessage(sender, null, replyMessage, null, null)
                        Toast.makeText(context, "Auto-replied to $sender", Toast.LENGTH_SHORT).show()
                    }
                }
            }
        }
    }
}
