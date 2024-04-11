import SwiftUI

struct EntryView: View {
		var body: some View {
				NavigationView {
						VStack {
								Text("Это приложение для интеграции с компьютером")
										.font(.system(size: 36))
								Spacer()
										.frame(height: 60)
								Text("Отсканировать QR-код")
										.frame(width: 300, height: 34)
										.background(.tertiary)
										.cornerRadius(12)
								NavigationLink {
										ListDevices()
								} label: {
										Text("Выбрать устройство из списка")
												.frame(width: 300, height: 34)
												.background(.tertiary)
												.cornerRadius(12)
												.foregroundColor(.primary)
								}
						}
				}
		}
}
