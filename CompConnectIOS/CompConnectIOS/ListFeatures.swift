import SwiftUI

struct ListFeatures: View {
		
		@Environment(\.dismiss) var dismiss
		@State var featuress: [Feature] = [
				.init(name: "Синхронизация буфера обмена", isOn: true),
				.init(name: "Синхронизация уведомлений", isOn: true),
				.init(name: "Ввод с клавиатуры", isOn: true)
		]
		
		var body: some View {
				Form {
						Toggle(featuress[0].name, isOn: $featuress[0].isOn)
						Toggle(featuress[1].name, isOn: $featuress[1].isOn)
						NavigationLink {
								KeyboardService()
						} label: {
								Text(featuress[2].name)
						}
						Button("Отключиться") {
								dismiss()
						}
						.foregroundColor(.red)
				}
				.navigationBarTitle("Основные")
				.navigationBarBackButtonHidden(true)
				.toolbar {
						NavigationLink {
								DebugPanel()
						} label: {
								Image(systemName: "gear")
						}
				}
		}
}


struct Feature {
		var name: String
		var isOn: Bool
}
