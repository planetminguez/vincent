import SwiftUI

@main
struct AutoReplyApp: App {
    @StateObject private var autoReplyManager = AutoReplyManager()

    var body: some Scene {
        WindowGroup {
            ContentView()
                .environmentObject(autoReplyManager)
        }
    }
}

struct ContentView: View {
    @EnvironmentObject var autoReplyManager: AutoReplyManager
    @State private var replyMessage: String = "Thank you for your message. I'll get back to you soon!"

    var body: some View {
        VStack(spacing: 20) {
            Toggle("Enable Auto-Reply", isOn: $autoReplyManager.isAutoReplyEnabled)
                .toggleStyle(SwitchToggleStyle())
                .padding()

            TextField("Custom Reply Message", text: $replyMessage)
                .textFieldStyle(RoundedBorderTextFieldStyle())
                .padding()

            Button("Update Reply Message") {
                autoReplyManager.setReplyMessage(replyMessage)
            }
            .padding()
        }
        .frame(width: 400, height: 200)
        .padding()
    }
}

class AutoReplyManager: ObservableObject {
    @Published var isAutoReplyEnabled: Bool = false {
        didSet {
            if isAutoReplyEnabled {
                startMonitoring()
            } else {
                stopMonitoring()
            }
        }
    }

    private var replyMessage: String = "Thank you for your message. I'll get back to you soon!"

    func setReplyMessage(_ message: String) {
        replyMessage = message
    }

    private func startMonitoring() {
        // Start monitoring messages (e.g., via Mail)
        DispatchQueue.global().async {
            while self.isAutoReplyEnabled {
                self.checkAndReplyToMessages()
                sleep(5) // Check for new messages every 5 seconds
            }
        }
    }

    private func stopMonitoring() {
        // Stop monitoring logic if necessary
    }

    private func checkAndReplyToMessages() {
        // Run AppleScript to check and reply to new emails
        let script = """
        tell application "Mail"
            set unreadMessages to messages of inbox whose read status is false
            repeat with msg in unreadMessages
                set content of msg to "Thank you for your email!
