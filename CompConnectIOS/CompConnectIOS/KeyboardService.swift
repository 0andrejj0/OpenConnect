import SwiftUI

struct KeyboardService: View {

		@State var text: String = " "
		@FocusState var keyboardFocused: Bool
		
		var body: some View {
				TextField("", text: $text)
						.focused($keyboardFocused)
						.autocapitalization(.none)
						.onAppear {
								DispatchQueue.main.asyncAfter(deadline: .now() + 0.1) {
										keyboardFocused = true
								}
						}
						.onChange(of: text) { _ in
								text = " "
						}
		}
}
