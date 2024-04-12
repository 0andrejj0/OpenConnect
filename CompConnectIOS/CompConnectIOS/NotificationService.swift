import SwiftUI
import UserNotifications

struct DebugPanel: View {
				
		var body: some View {
				VStack(alignment: .leading) {
						Button {
								UNUserNotificationCenter.current().requestAuthorization(options: [.alert, .badge, .sound]) { success, error in
										if success {
												print("All set!")
										} else if let error {
												print(error.localizedDescription)
										}
								}
						} label: {
								HStack {
										Text("Request Permission")
										Spacer()
								}
						}
						
						Button {
								let content = UNMutableNotificationContent()
								let tInd: Int = .random(in: 0..<pushTitles.count)
								let sInd: Int = .random(in: 0..<pushSubtitles.count)
								content.title = pushTitles[tInd]
								content.subtitle = pushSubtitles[sInd]
								content.sound = UNNotificationSound.default

								let trigger = UNTimeIntervalNotificationTrigger(timeInterval: 10, repeats: false)
								let request = UNNotificationRequest(identifier: UUID().uuidString, content: content, trigger: trigger)

								UNUserNotificationCenter.current().add(request)
								
								client.send(string: pushTitles[tInd])
						} label: {
								HStack {
										Text("Push notification")
										Spacer()
								}
						}
						Spacer()
				}
				.padding()
				.navigationTitle("Debug")
		}
}

let pushTitles: [String] = [
		"Random push", "Debug push", "Just push", "Another push", "Lovable push", "This push"
]

let pushSubtitles: [String] = [
		"Brrr...", "Like it", "Go go go", "Posted cringe", "Кабан на подскоке"
]
