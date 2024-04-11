import SwiftUI

struct ListFeatures: View {
		
		@State var featuress: [Feature] = features
		
		
		var body: some View {
				NavigationView {
						Form {
								Toggle(featuress[0].name, isOn: $featuress[0].isOn)
								Toggle(featuress[1].name, isOn: $featuress[1].isOn)
								NavigationLink { } label: {
										Text(featuress[2].name)
								}
						}
				}
		}
}


struct Feature {
		var name: String
		var isOn: Bool
}


var features: [Feature] = [
		.init(name: "Синхронизация буфера обмена", isOn: true),
		.init(name: "Синхронизация уведомлений", isOn: true),
		.init(name: "Ввод с клавиатуры", isOn: true)
]
